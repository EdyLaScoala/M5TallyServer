from flask import Flask, request, jsonify
import subprocess
import os

app = Flask(__name__)

TEMPLATE_PATH = "./template.ino"
SKETCH_PATH = "./templates"
FQBN = "esp32:esp32:m5stack_atom_s3"

@app.route('/compile', methods=['POST'])
def compile_sketch():
    data = request.json
    param1 = data.get("param1")
    param2 = data.get("param2")
    param3 = data.get("param3")

    # Generate the sketch
    with open(TEMPLATE_PATH, 'r') as template_file:
        sketch_code = template_file.read().format(param1=param1, param2=param2, param3=param3)

    os.makedirs(SKETCH_PATH, exist_ok=True)
    sketch_file_path = os.path.join(SKETCH_PATH, "sketch.ino")
    with open(sketch_file_path, 'w') as sketch_file:
        sketch_file.write(sketch_code)

    # Compile using Arduino CLI
    compile_cmd = [
        "arduino-cli", "compile",
        "--fqbn", FQBN,
        SKETCH_PATH
    ]

    try:
        result = subprocess.run(compile_cmd, capture_output=True, text=True, check=True)
        return jsonify({"status": "success", "output": result.stdout})
    except subprocess.CalledProcessError as e:
        return jsonify({"status": "error", "error": e.stderr}), 500

if __name__ == '__main__':
    app.run(debug=True)
