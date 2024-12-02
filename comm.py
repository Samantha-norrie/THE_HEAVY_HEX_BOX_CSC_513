import serial
import time
import sys
from qiskit import *
import numpy as np
from PyQt5.QtWidgets import QApplication
from fe import TheHeavyHexBoxFE

app = QApplication(sys.argv)
main_window = TheHeavyHexBoxFE()
main_window.show()
sys.exit(app.exec_())
# try:
    
#     while True:
#         # Read data from Arduino
#         if arduino.in_waiting > 0:
#             message = arduino.readline().decode('utf-8').strip()
#             print(f"Arduino: {message}")

#         # Send a command to Arduino
#         command = input("Enter a command for Arduino (or type 'exit'): ")
#         if command.lower() == 'exit':
#             break
#         arduino.write(('6').encode('utf-8'))  # Send command
# finally:
#     arduino.close()
