from flask import Flask, request
import serial

app = Flask(__name__)
arduino = serial.Serial('/dev/cu.usbmodem1101', 9600)  # Adjust to your Arduino's port

@app.route('/command', methods=['POST'])
def send_command():
    data = request.json
    if 'command' in data:
        arduino.write(data['command'].encode())
        return "Command sent!", 200
    return "Invalid request", 400

if __name__ == '__main__':
    app.run(debug=True)
