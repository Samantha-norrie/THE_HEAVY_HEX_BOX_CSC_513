int index = 0;
const String T = "T";
const String t = "t";
const String C = "C";
const int thirteen = 13;
// C Pins

const int twelve = 12;
const int eleven = 11;
const int ten = 10;
const int nine = 9;
const int eight = 8; 

// T Pins
const int two = 2;

// t Pins
const int seven = 7;
const int six = 6;
const int five = 5;

const int four = 4;
const int three = 3;


const int CONTROL_QUBITS[6] = {1, 3, 5, 7, 9, 11};
const int TARGET_QUBITS[6] = {0, 2, 4, 6, 8, 10};

/*
 * 0 = not receiving input
 * 1 = receiving controls
 * 2 = receiving targets
 */
int inputPhase = 0;

int tempControlIndex = 0;
int tempTargetIndex = 0;
int tempControl[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int tempTarget[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

// store pins of violating qubits
int violationsIndex = 0;
int violations[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
bool violationOnOff = false;
bool flagAllQubits = false;

/*
TODO: Fix. No rhyme or reason for these configurations, 
just the consequence of gluing the hardware together before writing the code!
*/

void turnOffAllLEDs() {
  for (int i = 0; i < 12; i++) {
    digitalWrite(i, LOW);
  }
}
int convertQubitToPin(int qubitIndex) {
  switch (qubitIndex) {
    case 0:
      return 2; //TODO
    case 1:
      return 10;
    case 2:
      return 6;
    case 3:
      return 8;
    case 4:
      return 12;
    case 5:
      return 3;
    case 6:
      return 7;
    case 7:
      return 13;
    case 8:
      return 4;
    case 9:
      return 9;
    case 10:
      return 5;
    case 11:
      return 11;
  }
  return -1;
}

// Turn all LEDs on and off
void startSequence() {
  for (int i = 2; i <= 13; i++) {
    digitalWrite(i, HIGH);
  }
  delay(5000);
  for (int i = 2; i <= 13; i++) {
    digitalWrite(i, LOW);
  }
}

// Check if qubit at index is a control qubit
bool isControlQubit(int qubitIndex) {
  for (int i = 0; i < 6; i++) {
    if (CONTROL_QUBITS[i] == qubitIndex) {
      return true;
    }
  }
  return false;
}

void handleViolations(bool on) {
  for (int i = 0; i < violationsIndex; i++) {
    digitalWrite(violations[i], on? HIGH: LOW);
  }
  // delay(1000);
  // for (int i = 0; i < violationsIndex; i++) {
  //   digitalWrite(violations[i], LOW);
  // }

}

void clearArray(int arr[12]) {
  for (int i = 0; i < 12; i++) {
    arr[i] = -1;
  }
}

// Illuminate LEDs for properly placed qubits, note misplaced qubits
void handleOperation() {

  // for target qubits
  for (int i = 0; i <= tempTargetIndex; i++) {
    int currQubit = tempTarget[i];
    int convertedPin = convertQubitToPin(tempTarget[i]);
    if (flagAllQubits || isControlQubit(currQubit)) {
      violations[violationsIndex] = convertedPin;
      violationsIndex++;
    } else {
      digitalWrite(convertedPin, HIGH);
    }
  }

  // For control qubits
  if (tempTarget[0] != -1) {
    for (int i = 0; i <= tempControlIndex; i++) {
      int currQubit = tempControl[i];
      int convertedPin = convertQubitToPin(tempControl[i]);
      if (flagAllQubits || !isControlQubit(currQubit)) {
        violations[violationsIndex] = convertedPin;
        violationsIndex++;
      } else {
        digitalWrite(convertedPin, HIGH);
      }
    }
  // for single-qubit gate
  } else {
    digitalWrite(convertQubitToPin(tempControl[0]), HIGH);
  }

  clearArray(tempControl);
  tempControlIndex = 0;
  clearArray(tempTarget);
  tempTargetIndex = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(twelve, OUTPUT);
  pinMode(eleven, OUTPUT);
  pinMode(ten, OUTPUT);
  pinMode(nine, OUTPUT);
  pinMode(eight, OUTPUT);
  pinMode(seven, OUTPUT);
  pinMode(six, OUTPUT);
  pinMode(five, OUTPUT);
  pinMode(four, OUTPUT);
  pinMode(three, OUTPUT);
  pinMode(two, OUTPUT);
  pinMode(thirteen, OUTPUT);
}

void loop() {

  if (violationsIndex != 0) {
    violationOnOff = !violationOnOff;
    handleViolations(violationOnOff);
  } else if (Serial.available() > 0) {

    // indicate that input is being received
    if (inputPhase == 0) {
      startSequence();
      inputPhase = 1;
    }
    char incomingChar = Serial.read();
    // Done receiving
    if (incomingChar == 'D') {
      inputPhase = 0;
      if (tempControl[0] != -1) {
        turnOffAllLEDs();
        handleOperation();
      }
    } else if (incomingChar == '\n') {
      turnOffAllLEDs();
      handleOperation();
      // violationsIndex = 0;
      // clearArray(violations);
    // Control qubits incoming
    } else if (incomingChar == 'C') {
      inputPhase = 1;
      // if (tempControl[0] != -1) {
      //   turnOffAllLEDs();
      //   handleOperation();
      // }
    // Target qubits incoming
    } else if (incomingChar == 'V') {
      flagAllQubits = true;
    } else if (incomingChar == 'T') {
      inputPhase = 2;
    // TODO remove
    } else if (incomingChar == ' ') {
      int i = 0;
    } else {
      int qubit = incomingChar - '0';
      // if (flagAllQubits) {
      //   // violations[violationsIndex] = convertQubitToPin(qubit);
      //   // violationsIndex++;
      // } else 
      if (inputPhase == 1) {
        tempControl[tempControlIndex] = qubit;
        tempControlIndex++;
      } else {
        tempTarget[tempTargetIndex] = qubit;
        tempTargetIndex++;
      }
    }
  }
  delay(1000);                

}

