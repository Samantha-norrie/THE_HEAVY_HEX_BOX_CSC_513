import io
import serial
import time
from PyQt5.QtWidgets import (
    QMainWindow, QTextEdit, QPushButton, QVBoxLayout, QWidget, QCheckBox, QMessageBox, QComboBox, QHBoxLayout
)
from PyQt5.QtGui import QTextCursor, QIcon
from PyQt5.QtCore import QSize
from QiskitUtils import process_circuit_received, create_string_for_serial, simulate_noise


class TheHeavyHexBoxFE(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("The Heavy-Hex Box")
        self.setGeometry(200, 200, 800, 600)

        self.main_layout = QVBoxLayout()

        # Example dropdown
        self.dropdown = QComboBox(self)
        self.dropdown.addItems(["Custom circuit", "No transpilation needed + no violation example", "Transpilation needed + no violation example", "No transpilation needed + violation example"])
        self.dropdown.currentIndexChanged.connect(self.on_dropdown_change)
        self.main_layout.addWidget(self.dropdown)

        # Code input
        self.code_input = QTextEdit(self)
        self.code_input.setText("from qiskit import *\nimport numpy as np\nqc = QuantumCircuit(12)\n# Insert code below \n")
        self.code_input.moveCursor(QTextCursor.End)
        self.main_layout.addWidget(self.code_input)

        # Transpilation checkbox
        self.debug_checkbox = QCheckBox("Enable Transpilation", self)
        self.debug_checkbox.stateChanged.connect(self.toggle_transpilation)
        self.main_layout.addWidget(self.debug_checkbox)

        # Noise checkbox
        self.noise_checkbox = QCheckBox("Enable Noise", self)
        self.noise_checkbox.stateChanged.connect(self.toggle_noise)
        self.main_layout.addWidget(self.noise_checkbox)


        # Info modal
        info_button_layout = QHBoxLayout()
        self.info_button = QPushButton(self)
        self.info_button.setIcon(QIcon("icon.png"))
        self.info_button.setIconSize(QSize(24, 24))
        self.info_button.clicked.connect(self.show_info_modal)

        info_button_layout.addWidget(self.info_button)
        info_button_layout.addStretch()
        self.main_layout.addLayout(info_button_layout)

        # Execute button
        self.execute_button = QPushButton("Execute", self)
        self.execute_button.clicked.connect(self.execute_code)
        self.main_layout.addWidget(self.execute_button)

        # Output display
        self.output_display = QTextEdit(self)
        self.output_display.setReadOnly(True)
        self.main_layout.addWidget(self.output_display)

        # Container for main layout
        container = QWidget()
        container.setLayout(self.main_layout)
        self.setCentralWidget(container)

        # Checkbox boolean flags
        self.noise = False
        self.transpilation = False

    def on_dropdown_change(self, index):
        # default
        if index == 0:
            self.code_input.setText(
                "from qiskit import *\nimport numpy as np\nqc = QuantumCircuit(12)\n# Insert code below \n"
            )
        # no transpilation needed, no violation
        elif index == 1:
            self.code_input.setText(
                "from qiskit import *\nimport numpy as np\nqc = QuantumCircuit(12)\n# Insert code below \nqc.x(0)\nqc.x(1)\nqc.cz(1, 4)\nqc.cz(7, 2)"
            )
        # transpilation needed, no violation
        elif index == 2: 
            self.code_input.setText(
                "from qiskit import *\nimport numpy as np\nqc = QuantumCircuit(12)\n# Insert code below \nqc.x(0)\nqc.x(1)\nqc.cx(1, 4)\nqc.cz(7, 2)"
            )
        # transpilation needed, violation
        elif index == 3: 
            self.code_input.setText(
                "from qiskit import *\nimport numpy as np\nqc = QuantumCircuit(12)\n# Insert code below \nqc.x(0)\nqc.x(1)\nqc.cx(1, 4)\nqc.cz(2, 7)"
            )
    def toggle_transpilation(self, state):
        if state == 2:
            self.transpilation = True
        else:
            self.transpilation = False

    def toggle_noise(self, state):
        if state == 2:
            self.noise = True
        else:
            self.noise = False

    def execute_code(self):
        user_code = self.code_input.toPlainText()

        output = io.StringIO()
        
        # CHANGE PORT HERE
        arduino = serial.Serial(port='/dev/cu.usbmodem1101', baudrate=9600, timeout=1)
        time.sleep(2)
        try:
            qiskit_data = process_circuit_received(user_code, output)
            print(qiskit_data)

            op_with_noise = None
            if self.noise:
                op_with_noise = simulate_noise(qiskit_data)

            formatted = create_string_for_serial(qiskit_data, output, self.transpilation, op_with_noise)
            print(formatted)
            arduino.write((formatted).encode('utf-8'))
        except Exception as e:
            output.write(f"Error: {e}")

        # Display the output
        self.output_display.setPlainText(output.getvalue())
        output.close()

    def show_info_modal(self):
        info_message = QMessageBox(self)
        info_message.setWindowTitle("About the Heavy-Hex Box")
        # TODO
        info_message.setText(
            '''The Heavy-Hex Box allows you work with a real quantum computer while receiving feedback in real time. To get started with the system, please enter code into code box or generate sample code using the dropdown.
            If you are not seeing any feedback from the Heavy-Hex Box, please ensure that
            \n-You have entered the correct port into CodeEditor.py
            \n-You have not received any feedback from the console
            \n No LEDs or wires connected to the Heavy-Hex Box have been damaged
            '''
        )
        info_message.setStandardButtons(QMessageBox.Ok)
        info_message.exec_()