from flask import Flask, request, jsonify, send_file
import subprocess
import os
import shutil
import threading
import time
import uuid  # Import the uuid module

app = Flask(__name__)

TEMPLATE_PATH = "./template.ino"
SKETCH_PATH = "./sketch"
FQBN = "esp32:esp32:m5stack_atoms3"
BUILD_PATH = "build"

# Lock to prevent multiple jobs from running simultaneously
job_lock = threading.Lock()
job_status = {}  # To track the status of each job

def compile_sketch_background(job_id, data):
    """
    Compiles the sketch in the background.
    """
    global job_status

    try:
        # Clear previous build data
        if os.path.exists(BUILD_PATH):
            shutil.rmtree(BUILD_PATH)
            print("Successfully cleared previous build data!")

        param1 = str(data.get("param1", ""))
        param2 = str(data.get("param2", ""))
        param3 = str(data.get("param3", ""))
        param4 = str(data.get("param4", ""))

        ip_octets = param3.split(".")
        if len(ip_octets) != 4:
            job_status[job_id] = {"status": "failed", "error": "Invalid IP address format"}
            return

        # Generate the sketch
        with open(TEMPLATE_PATH, 'r') as template_file:
            sketch_code = template_file.read().format(
                ssid=param1,
                passwd=param2,
                ip_octet1=ip_octets[0],
                ip_octet2=ip_octets[1],
                ip_octet3=ip_octets[2],
                ip_octet4=ip_octets[3],
                camNumber=param4
            )

        os.makedirs(SKETCH_PATH, exist_ok=True)
        sketch_file_path = os.path.join(SKETCH_PATH, "sketch.ino")
        print(f"Creating sketch file at: {os.path.abspath(sketch_file_path)}")
        with open(sketch_file_path, 'w') as sketch_file:
            sketch_file.write(sketch_code)

        # Compile using Arduino CLI
        os.makedirs(BUILD_PATH, exist_ok=True)
        print(f"Compiling sketch from directory: {os.path.abspath(SKETCH_PATH)}")
        compile_cmd = [
            "arduino-cli", "compile",
            "--fqbn", FQBN,
            "--build-path", BUILD_PATH,
            "--libraries", "/workspaces/M5Tally/Arduino/libraries",
            SKETCH_PATH
        ]

        result = subprocess.run(compile_cmd, capture_output=True, text=True, check=True)

        # Find the binary file
        binary_file_path = os.path.join(BUILD_PATH, "sketch.ino.bin")
        print(f"Binary file expected at: {os.path.abspath(binary_file_path)}")

        job_status[job_id] = {"status": "completed", "file_path": binary_file_path}

    except subprocess.CalledProcessError as e:
        job_status[job_id] = {"status": "failed", "error": e.stderr}
        print(f"Compilation error: {e.stderr}")

    except Exception as e:
        job_status[job_id] = {"status": "failed", "error": str(e)}
        print(f"Unexpected error: {str(e)}")


@app.route('/compile', methods=['POST'])
def compile_sketch():
    global job_lock, job_status

    # Check if a job is already running
    if job_lock.locked():
        return jsonify({"status": "error", "error": "A compilation job is already in progress. Please wait."}), 429

    with job_lock:
        # Generate a unique job ID
        job_id = str(uuid.uuid4())
        job_status[job_id] = {"status": "pending"}

        data = request.get_json()
        print(f"Received data: {data}")

        # Start the compilation in a background thread
        thread = threading.Thread(target=compile_sketch_background, args=(job_id, data))
        thread.start()

        # Return the job ID immediately
        return jsonify({"status": "compiling", "job_id": job_id}), 202


@app.route('/status/<job_id>', methods=['GET'])
def get_status(job_id):
    global job_status
    status = job_status.get(job_id)
    if status is None:
        return jsonify({"status": "not_found"}), 404
    return jsonify(status)


@app.route('/download/<job_id>', methods=['GET'])
def download_file(job_id):
    global job_status
    status = job_status.get(job_id)

    if status is None or status.get("status") != "completed":
        return jsonify({"status": "not_ready"}), 400

    file_path = "/workspaces/M5Tally/build/sketch.ino.merged.bin"
    if not file_path or not os.path.exists(file_path):
        return jsonify({"status": "file_not_found"}), 500

    return send_file(file_path, as_attachment=True, download_name="sketch.bin")


if __name__ == '__main__':
    app.run(debug=True)
