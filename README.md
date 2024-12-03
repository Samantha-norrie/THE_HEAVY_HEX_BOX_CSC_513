# The Heavy-Hex Box

The Heavy-Hex Box (HHB) is designed to allow you to work with and receive visual feedback from real quantum computers in real time.
## Steps
- Plug the HHB hardware into your device.
- Navigate to \textit{CodeEditor.py} and ensure that the port that the HHB is on is being properly connected to through \textit{Serial} (seen in Figure~\ref{fig7}). A comment in the code denotes where the port value should be modified if necessary.
- Launch the HHB code editor by running CodeEditor.py.
- Enter in the Qiskit code for the desired quantum program below the comment in the code box. The code entered must follow the following rules:
  - code above the comment must not be modified
  - all quantum gates must be appended to the \textit{qc} variable
        \item Control and target operations must only be done on control and target qubits respectively. See Figure~\ref{fig3} to see how each qubit index maps to an LED on the HHB
    \end{itemize}
    Alternatively, sample code can also be generated using the example dropdown at the top of the editor. Figure~\ref{fig5} shows the \textit{transpilation needed + no violation} example in the editor.
    \item Check the \textit{Enable Transpilation} checkbox if you would like transpilation to applied your quantum program. If transpilation is disabled, only the following gates can be used~\cite{nativeGates}:
    \begin{itemize}
        \item CONTROLLED-Z (CZ)
        \item ROTATIONAL-Z (RZ)
        \item SQUARE-ROOT-NOT (SX)
        \item NOT (X)
    \end{itemize}
    \item Check the \textit{Enable Noise }checkbox if you would like it to be possible that noise occurs while your quantum program is being run.

    \item Click the \textit{Execute} button once you are done. All twelve LEDs of the HHB will light up to signal that your circuit is about to be run. If not all LEDs light up, stop your program and if any wires connecting the box component to your local device are loose.
    \item Observe each operation of your circuit being run on the HHB. The LED(s) representing the qubit(s) used in the operation being presented will light up. If the operation is invalid, the aforementioned LED(s) will blink until the user resubmits their code through the code editor or they turn off the device by either unplugging the hardware from their computer or close the code editor. An operation can be deemed invalid for the following reasons:
    \begin{itemize}
        \item A control qubit was used as a target qubit for the gate
        \item A target qubit was used as a control qubit for the gate
        \item An invalid gate was used (only applicable if transpilation is disabled). Figure~\ref{fig6} shows an example of a transpilation error being documented inside the code editor's console.
    \end{itemize}
    Operations can also be interrupted by noise, which will also cause the LEDs of the qubits involved in the operation to flash. Consult the console at the bottom of the code editor for information on why your program failed to execute. If no invalid operation or noise occurred during the run of the program, all LEDs will turn off to signal that the program has been run.
    \item Press the Execute button if you would like to run your program again
## Troubleshooting
If you are experiencing difficulties with your device please check that
- you are using the port of your local device that the box component is connected to
- no wires connecting the box component to your device are loose
- no LEDs are damaged

  If you are still experiencing issues, please consult IBM's [Qiskit documentation](https://www.ibm.com/quantum/qiskit)

