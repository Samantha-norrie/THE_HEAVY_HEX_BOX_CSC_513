#include "Qubit.h"
#include "Gate.h"
const int MAX_SIZE = 10;
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

int gateIndex = 0;
Gate* gates[20] = {nullptr};
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

void startSequence() {
  for (int i = 2; i <= 13; i++) {
    digitalWrite(i, HIGH);
  }
  delay(5000);
  for (int i = 2; i <= 13; i++) {
    digitalWrite(i, LOW);
  }
}
void handleCurrentGate(Gate curr) {
  for(int i = 0; i < 12; i++) {
    if (curr.getControlQubits()[i] != -1) {
      digitalWrite(curr.getControlQubits()[i], HIGH);
    }
    if (curr.getTargetQubits()[i] != -1) {
      digitalWrite(curr.getTargetQubits()[i], HIGH);
    }
  }
}

void handleSequence() {
  startSequence();
  for (int i = 0; i < 20; i++) {
    if (gates[i] != nullptr) {
      handleCurrentGate(*gates[i]);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Qubit qubitOne(1, 10, C);
  Qubit qubitTwo(2, 6, t);
  Qubit qubitThree(3, 8, C);
  Qubit qubitFive(5, 12, T);
  Qubit qubitSix(6, 7, t);
  Qubit qubitSeven(7, 13, C);
  Qubit qubitNine(9, 9, C);
  Qubit qubitTen(10, 5, t);
  Qubit qubitEleven(11, 11, C);

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

  if (Serial.available() > 0) {

    // indicate that input is being received
    if (inputPhase == 0) {
      inputPhase = 1;
    }
    char incomingChar = Serial.read();
    if (incomingChar == 'D') {
      inputPhase = 0;
      if (tempControl[0] != -1) {
        gates[gateIndex] = new Gate(tempControl, tempTarget);
      }
      handleSequence();
    } else if (incomingChar == 'C') {
      inputPhase = 1;
      if (tempControl[0] != -1) {
        gates[gateIndex] = new Gate(tempControl, tempTarget);
        gateIndex++;
      }
    } else if (incomingChar == 'T') {
      inputPhase = 2;
    } else if (incomingChar == ' ') {
      int i = 0;
    } else {
      if (inputPhase == 1) {
        tempControl[tempControlIndex] = incomingChar - '0';
        tempControlIndex++;
      } else {
        tempTarget[tempTargetIndex] = incomingChar - '0';
        tempTargetIndex++;
      }
    }
    int newPin = incomingChar - '0';
    digitalWrite(newPin, HIGH);
    Serial.print("Received from Python: ");
    delay(1000);
  }                

}

