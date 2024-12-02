from qiskit import *
from qiskit.quantum_info.operators import Operator
from qiskit.circuit import CircuitInstruction, Instruction, Qubit
import numpy as np
import math
import subprocess
import sys
import os
import tempfile

NATIVE_GATES = ["cz", "rz", "sx", "x"]
def create_string_for_serial(qiskit_data, transpilation):
    formatted_string = ""
    for i in range(0, len(qiskit_data)):
        current_gate = qiskit_data[i]
        print("transpilation", transpilation)
        print(current_gate.operation.name in NATIVE_GATES)
        if not transpilation and not current_gate.operation.name in NATIVE_GATES:
            formatted_string = formatted_string + "V "
        num_qubits = current_gate.operation.num_qubits
        print(current_gate)
        if num_qubits == 1:
            print(current_gate.qubits)
            formatted_string = formatted_string + "C " + str(current_gate.qubits[0].index) + " T \n" 
        elif num_qubits == 2:
            formatted_string = formatted_string + "C " + str(current_gate.qubits[0].index) + " T " + str(current_gate.qubits[1].index) + "\n"
        elif num_qubits == 3:
            formatted_string = formatted_string + "C " + str(current_gate.qubits[0].index) + " " + str(current_gate.qubits[1].index) + " T " + str(current_gate.qubits[2].index) + "\n"
        else:
            return formatted_string
    formatted_string = formatted_string + "D"
    return formatted_string

def process_circuit_received(qc_string):
    qc_code_list = qc_string.split('\n')

    qc_string_formatted = ""

    end_of_imports_found = False
    for i in range(0, len(qc_code_list)):
        if not end_of_imports_found and "import numpy as np" in qc_code_list[i]:
            qc_string_formatted = qc_string_formatted + qc_code_list[i] + "\ndef main():\n"
            end_of_imports_found = True
        elif end_of_imports_found:
            qc_string_formatted = qc_string_formatted + "   " + qc_code_list[i]  + "\n"
        else:
            qc_string_formatted = qc_string_formatted + qc_code_list[i] + "\n"
    qc_string_formatted = qc_string_formatted + "   " + "print(qc.data)" + "\nmain()"

    circuit_details = []

    # Use a temporary file to execute the code securely
    with tempfile.NamedTemporaryFile(delete=False, suffix='.py') as temp_file:
        temp_file.write(qc_string_formatted.encode('utf-8'))
        temp_file_name = temp_file.name
    try:
        # Execute the code in the temporary file using a subprocess
        result = subprocess.run([sys.executable, temp_file_name], capture_output=True, text=True, timeout=5)

        output = result.stdout

        circuit_details = eval(output)
    except Exception as e:
        raise NameError
    finally:

        # Ensure the temporary file is deleted after execution
        os.remove(temp_file_name)

    return circuit_details