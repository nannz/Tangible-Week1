#include "Keyboard.h"
#include "Mouse.h"

//initialize two pins for s and w
const int buttonUp = 4; //up button pin w
const int buttonDown = 2; //down button pin s
int previousBtnStateUp = HIGH;
int previousBtnStateDown = HIGH;

//initialize for a and d - rotation
int potPin = A0; //Analog pin A0
int currentVal = 0; //initialize the value from the potenmeter
int previousVal = 0;

//initialize analog pin for xAxis and yAxia
int xAxis = A2;
int yAxis = A1;
//initialize range, delay amount, threshold, centre for better controling the mouse
int range = 12;// output range of X or Y movement to control the speed?
int responseDelay = 5;
int threshold = range / 4;// if the change is smaller than 3, than it's not move
int center = range / 2; // resting position value
//SDSDSDSD
//initialize digitalPin for mouseClick
int mouseButton = 12;

void setup() {
  //make the buttons pin input for s and w
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  //initiate mouseButton
  pinMode(mouseButton, INPUT);
  digitalWrite(mouseButton, HIGH);

  //initialize control over the keyboard:
  Keyboard.begin();
  //initialize control over the mouse:
  Mouse.begin();
}

void loop() {
  //detect up button and down button, s and w
  previousBtnStateUp = upDownButton(buttonUp, previousBtnStateUp);
  previousBtnStateDown = upDownButton(buttonDown, previousBtnStateDown);

  //read analog value and control a and d for rotate left or right
  previousVal = adButton(potPin, previousVal);

  //mouseClick
  mouseClick(mouseButton);

  //move the mouse
  moveMouse(xAxis, yAxis);

  delay(responseDelay);
}

//function for controlling s and w
int upDownButton(int thisButton, int previousState) {
  int thisState = digitalRead(thisButton);

  if (thisState != previousState) {
    //and it is currently pressed
    if (thisState == HIGH) {
      //output
      if (thisButton == buttonUp) {
        //press 'w'
        Keyboard.press('W');
        // delay(responseDelay);
        //Keyboard.releaseAll();
      }
      if (thisButton == buttonDown) {
        //press 's'
        Keyboard.press('S');
        //delay(responseDelay);
        //Keyboard.releaseAll();
      }
    } else if (thisState == LOW) {
      Keyboard.releaseAll();
    }
    previousState = thisState;
    return previousState;
  }
}

int adButton(int thisPotPin, int previousVal ) {
  int currentVal = analogRead(thisPotPin);

  if (currentVal - previousVal > 1) { //rotate right
    //Serial.println("pressed, D");
    Keyboard.release('A');
    Keyboard.press('D');
  } else if (currentVal - previousVal < -1) { //rotate left
    //Serial.println("pressed, A");
    Keyboard.release('D');
    Keyboard.press('A');
  } else {
    if (currentVal == 0) { //press a
      Keyboard.release('D');
      Keyboard.press('A');
    } else if (currentVal == 1023) { //press d
      Keyboard.release('A');
      Keyboard.press('D');
    } else {
      //Serial.println("no pressed");
      Keyboard.release('A');
      Keyboard.release('D');
    }
  }
  //reset
  previousVal = currentVal;
  return previousVal;
}



void moveMouse(int xAxis, int yAxis) {
  //read the two axes
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);

  Mouse.move(xReading, yReading, 0);

}

//read the corresponding value of the x and y movements according to xAsix and yAxis read from the joystick
int readAxis(int thisAxis) {
  int thisReading = analogRead(thisAxis);
  //map the reading to the defined range
  thisReading = map(thisReading, 0, 1023, 0, range);
  //the distance between the centre and the new position
  int distance = thisReading - range / 2;
  //if the distance is too small, consider it as not moved
  if (abs(distance) < threshold) {
    distance = 0;
  }
  return distance;
}


void mouseClick(int mousePin) {
  int mouseState = digitalRead(mousePin);
  //press the joystick
  if (mouseState == LOW) {
    if (Mouse.isPressed(MOUSE_LEFT) == false) {
      Mouse.press(MOUSE_LEFT);
      Serial.println("pressed");
    }
  } else { //not press the joystick
    //if this time, the mouse is pressed, release it
    if (Mouse.isPressed(MOUSE_LEFT) == true) {
      Mouse.release(MOUSE_LEFT);
      Serial.println("released");
    }
  }
}

