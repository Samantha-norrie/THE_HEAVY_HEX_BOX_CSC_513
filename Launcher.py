import sys
from qiskit import *
import numpy as np
from PyQt5.QtWidgets import QApplication
from CodeEditor import TheHeavyHexBoxFE

app = QApplication(sys.argv)
main_window = TheHeavyHexBoxFE()
main_window.show()
sys.exit(app.exec_())

