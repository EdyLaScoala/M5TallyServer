import requests
import json
import time

url = "https://reimagined-space-funicular-v6vjpx575797f6xww-5000.app.github.dev/compile"
status_url = "https://reimagined-space-funicular-v6vjpx575797f6xww-5000.app.github.dev/status/"
download_url = "https://reimagined-space-funicular-v6vjpx575797f6xww-5000.app.github.dev/download/"
data = {
    "param1": "wifi",
    "param2": "password",
    "param3": "192.168.31.145"
}

headers = {'Content-Type': 'application/json'}

# Debugging: Print the request data
print("Request data:", json.dumps(data, indent=4))

try:
    # Step 1: Send the initial request to start compilation
    response = requests.post(url, json=data, headers=headers, timeout=30)  # Increased timeout
    response.raise_for_status()  # Raise HTTPError for bad responses (4xx or 5xx)

    # Assume the server returns a job ID to track the compilation
    response_data = response.json()
    job_id = response_data.get("job_id")
    if not job_id:
        print("Error: Server did not return a job ID.")
        exit(1)

    print(f"Compilation started. Job ID: {job_id}")

    # Step 2: Poll the server for the status of the compilation
    while True:
        try:
            status_response = requests.get(f"{status_url}{job_id}", headers=headers, timeout=10)
            status_response.raise_for_status()
            status_data = status_response.json()

            if status_data.get("status") == "completed":
                print("Compilation completed. Downloading the file...")
                break
            elif status_data.get("status") == "failed":
                print("Compilation failed:", status_data.get("error"))
                exit(1)
            else:
                print("Compilation in progress. Waiting...")
                time.sleep(10)  # Wait for 10 seconds before polling again
        except requests.exceptions.Timeout:
            print("Status check timed out. Retrying...")

    # Step 3: Download the compiled file
    file_response = requests.get(f"{download_url}{job_id}", headers=headers, timeout=180)
    file_response.raise_for_status()

    with open("sketch.bin", 'wb') as f:
        f.write(file_response.content)
    print("File downloaded successfully!")

except requests.exceptions.HTTPError as e:
    print(f"HTTP error occurred: {e}")
    print(f"Response content: {response.text if 'response' in locals() else 'No response'}")
except requests.exceptions.RequestException as e:
    print(f"An error occurred: {e}")
