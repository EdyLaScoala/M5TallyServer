from flask import Flask, request, jsonify, send_file
import subprocess
import os
import shutil
import threading
import time
import uuid
import logging
from flask_cors import CORS

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler("server.log"),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# Configuration
TEMPLATE_PATH = "./template_1.5.ino"
SKETCH_PATH = "./sketch"
FQBN = "esp32:esp32:m5stack_atoms3"
BUILD_PATH = "build"
LIBRARIES_PATH = "/workspaces/M5TallyServer/Arduino/libraries"
OUTPUT_BINARY = "/workspaces/M5TallyServer/build/sketch.ino.merged.bin"

# Lock to prevent multiple jobs from running simultaneously
job_lock = threading.Lock()
job_status = {}  # To track the status of each job

def update_job_progress(job_id, progress_msg):
    """Update job status with progress information"""
    if job_id in job_status:
        job_status[job_id]["progress"] = progress_msg
        logger.info(f"Job {job_id}: {progress_msg}")

def compile_sketch_background(job_id, data):
    """
    Compiles the sketch in the background with progress updates.
    """
    global job_status

    try:
        # Update status to processing
        job_status[job_id] = {"status": "processing", "progress": "Starting compilation..."}
        
        # Clear previous build data
        update_job_progress(job_id, "Cleaning previous build data...")
        if os.path.exists(BUILD_PATH):
            shutil.rmtree(BUILD_PATH)
        
        # Extract parameters with validation
        param1 = str(data.get("param1", ""))
        param2 = str(data.get("param2", ""))
        param3 = str(data.get("param3", ""))
        param4 = str(data.get("param4", "1"))  # Default to camera 1 if not provided

        # Validate parameters
        if not param1.strip():
            job_status[job_id] = {"status": "failed", "error": "WiFi SSID cannot be empty"}
            return
            
        # Validate IP address
        ip_octets = param3.split(".")
        if len(ip_octets) != 4:
            job_status[job_id] = {"status": "failed", "error": "Invalid IP address format"}
            return
            
        for octet in ip_octets:
            try:
                value = int(octet)
                if value < 0 or value > 255:
                    job_status[job_id] = {"status": "failed", "error": "IP address octets must be between 0 and 255"}
                    return
            except ValueError:
                job_status[job_id] = {"status": "failed", "error": "IP address must contain numeric values"}
                return
        
        # Validate camera number (simple validation - should be a number)
        try:
            camera_num = int(param4)
            if camera_num < 0:
                job_status[job_id] = {"status": "failed", "error": "Camera number must be a non-negative integer"}
                return
        except ValueError:
            job_status[job_id] = {"status": "failed", "error": "Camera number must be a valid integer"}
            return
            
        # Generate the sketch
        update_job_progress(job_id, "Generating sketch from template...")
        
        # Check if template exists
        if not os.path.exists(TEMPLATE_PATH):
            job_status[job_id] = {"status": "failed", "error": f"Template file not found at {TEMPLATE_PATH}"}
            return
            
        with open(TEMPLATE_PATH, 'r') as template_file:
            try:
                sketch_code = template_file.read().format(
                    ssid=param1,
                    passwd=param2,
                    ip_octet1=ip_octets[0],
                    ip_octet2=ip_octets[1],
                    ip_octet3=ip_octets[2],
                    ip_octet4=ip_octets[3],
                    camNumber=param4
                )
            except KeyError as e:
                job_status[job_id] = {"status": "failed", "error": f"Template formatting error: {str(e)}"}
                return
            except Exception as e:
                job_status[job_id] = {"status": "failed", "error": f"Error processing template: {str(e)}"}
                return

        # Create sketch directory and file
        update_job_progress(job_id, "Writing sketch file...")
        try:
            os.makedirs(SKETCH_PATH, exist_ok=True)
            sketch_file_path = os.path.join(SKETCH_PATH, "sketch.ino")
            with open(sketch_file_path, 'w') as sketch_file:
                sketch_file.write(sketch_code)
        except Exception as e:
            job_status[job_id] = {"status": "failed", "error": f"Failed to write sketch file: {str(e)}"}
            return

        # Prepare build directory
        update_job_progress(job_id, "Preparing build environment...")
        os.makedirs(BUILD_PATH, exist_ok=True)
        
        # Check if Arduino CLI is available
        try:
            version_cmd = ["arduino-cli", "version"]
            version_result = subprocess.run(version_cmd, capture_output=True, text=True, check=True)
            logger.info(f"Arduino CLI version: {version_result.stdout.strip()}")
        except subprocess.CalledProcessError as e:
            job_status[job_id] = {"status": "failed", "error": "Arduino CLI not available or not working properly"}
            return
        except FileNotFoundError:
            job_status[job_id] = {"status": "failed", "error": "Arduino CLI not found in PATH"}
            return
            
        # Check if libraries directory exists
        if not os.path.exists(LIBRARIES_PATH):
            job_status[job_id] = {
                "status": "failed", 
                "error": f"Libraries directory not found at {LIBRARIES_PATH}"
            }
            return
            
        # Compile using Arduino CLI
        update_job_progress(job_id, "Compiling sketch (this may take a while)...")
        compile_cmd = [
            "arduino-cli", "compile",
            "--fqbn", FQBN,
            "--build-path", BUILD_PATH,
            "--libraries", LIBRARIES_PATH,
            "--verbose",  # Add verbose output for better debugging
            SKETCH_PATH
        ]

        try:
            process = subprocess.Popen(
                compile_cmd, 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
                universal_newlines=True
            )
            
            # Process output in real-time to provide progress updates
            while True:
                output_line = process.stdout.readline()
                if output_line == '' and process.poll() is not None:
                    break
                if output_line:
                    line = output_line.strip()
                    if "Compiling" in line or "Linking" in line or "Building" in line:
                        update_job_progress(job_id, line)
            
            # Get the final return code
            return_code = process.poll()
            if return_code != 0:
                error_output = process.stderr.read()
                job_status[job_id] = {"status": "failed", "error": f"Compilation failed with code {return_code}: {error_output}"}
                return
                
        except Exception as e:
            job_status[job_id] = {"status": "failed", "error": f"Error during compilation: {str(e)}"}
            return

        # Verify binary file exists
        update_job_progress(job_id, "Checking binary output...")
        if not os.path.exists(OUTPUT_BINARY):
            job_status[job_id] = {"status": "failed", "error": "Compiled binary file not found"}
            return
        
        update_job_progress(job_id, "Compilation completed successfully")
        job_status[job_id] = {
            "status": "completed", 
            "file_path": OUTPUT_BINARY,
            "progress": "Ready for download"
        }

    except subprocess.CalledProcessError as e:
        error_msg = e.stderr if hasattr(e, 'stderr') else str(e)
        job_status[job_id] = {"status": "failed", "error": f"Compilation process error: {error_msg}"}
        logger.error(f"Compilation error for job {job_id}: {error_msg}")

    except Exception as e:
        job_status[job_id] = {"status": "failed", "error": str(e)}
        logger.error(f"Unexpected error for job {job_id}: {str(e)}")

    # Clean up job status after some time (e.g., 30 minutes)
    cleanup_timer = threading.Timer(1800, lambda: job_status.pop(job_id, None))
    cleanup_timer.daemon = True
    cleanup_timer.start()


@app.route('/compile', methods=['POST'])
def compile_sketch():
    """Start a new compilation job"""
    global job_lock, job_status

    try:
        # Parse JSON data from request
        data = request.get_json()
        if not data:
            return jsonify({"status": "error", "error": "No data provided"}), 400
            
        logger.info(f"Received compilation request with data: {data}")
        
        # Check if a job is already running
        if job_lock.locked():
            return jsonify({
                "status": "error", 
                "error": "A compilation job is already in progress. Please wait."
            }), 429

        with job_lock:
            # Generate a unique job ID
            job_id = str(uuid.uuid4())
            job_status[job_id] = {"status": "pending"}

            # Start the compilation in a background thread
            thread = threading.Thread(target=compile_sketch_background, args=(job_id, data))
            thread.daemon = True  # Make thread daemon so it doesn't block app shutdown
            thread.start()

            logger.info(f"Started compilation job with ID: {job_id}")
            # Return the job ID immediately
            return jsonify({"status": "compiling", "job_id": job_id}), 202
            
    except Exception as e:
        logger.error(f"Error handling compilation request: {str(e)}")
        return jsonify({"status": "error", "error": str(e)}), 500


@app.route('/status/<job_id>', methods=['GET'])
def get_status(job_id):
    """Get the status of a compilation job"""
    global job_status
    
    try:
        status = job_status.get(job_id)
        if status is None:
            return jsonify({"status": "not_found", "error": "Job not found"}), 404
            
        return jsonify(status)
        
    except Exception as e:
        logger.error(f"Error retrieving status for job {job_id}: {str(e)}")
        return jsonify({"status": "error", "error": str(e)}), 500


@app.route('/download/<job_id>', methods=['GET'])
def download_file(job_id):
    """Download the compiled binary file"""
    global job_status
    
    try:
        status = job_status.get(job_id)

        if status is None:
            return jsonify({"status": "not_found", "error": "Job not found"}), 404
            
        if status.get("status") != "completed":
            return jsonify({
                "status": "not_ready", 
                "error": "File not ready for download", 
                "current_status": status.get("status")
            }), 400

        file_path = status.get("file_path") or OUTPUT_BINARY
        
        if not os.path.exists(file_path):
            return jsonify({"status": "file_not_found", "error": "Binary file not found"}), 500

        logger.info(f"Sending file {file_path} for download (job_id: {job_id})")
        return send_file(
            file_path, 
            as_attachment=True, 
            download_name="sketch.bin",
            mimetype="application/octet-stream"
        )
        
    except Exception as e:
        logger.error(f"Error downloading file for job {job_id}: {str(e)}")
        return jsonify({"status": "error", "error": str(e)}), 500


@app.route('/cancel/<job_id>', methods=['POST'])
def cancel_job(job_id):
    """Cancel a running compilation job"""
    global job_status
    
    try:
        if job_id not in job_status:
            return jsonify({"status": "not_found", "error": "Job not found"}), 404
            
        # We can't truly cancel the subprocess easily,
        # but we can mark it as cancelled for the frontend
        current_status = job_status[job_id].get("status")
        
        if current_status in ["completed", "failed"]:
            return jsonify({
                "status": "error", 
                "error": f"Cannot cancel job in '{current_status}' state"
            }), 400
            
        job_status[job_id] = {"status": "cancelled", "error": "Job cancelled by user"}
        logger.info(f"Job {job_id} marked as cancelled")
        
        return jsonify({"status": "cancelled"}), 200
        
    except Exception as e:
        logger.error(f"Error cancelling job {job_id}: {str(e)}")
        return jsonify({"status": "error", "error": str(e)}), 500


@app.route('/health', methods=['GET'])
def health_check():
    """Simple health check endpoint"""
    return jsonify({
        "status": "ok",
        "arduino_cli": os.system("which arduino-cli") == 0,
        "active_jobs": len(job_status)
    })


if __name__ == '__main__':
    # Ensure directories exist
    os.makedirs(SKETCH_PATH, exist_ok=True)
    os.makedirs(BUILD_PATH, exist_ok=True)
    
    # Log server startup
    logger.info("WiFi Compiler Server starting up...")
    
    # Start the Flask app
    app.run(host='0.0.0.0', port=5000, debug=True)