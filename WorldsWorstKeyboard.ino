// This is the program for Worlds Worst Keyboard
// Adapting from my media controller program, instead of the keys giving media signals and the pot acting as a volume knob,
// the pot now acts as a layer controller, and switches between layers so that we can use the keys, 4 at a time.
// Please enjoy it.

#include <Keyboard.h>

int switch1Pin = 3;
int switch2Pin = 4;
int switch3Pin = 2;
int switch4Pin = 1; // variables for which switch corresponds to which pin
bool switch1State = false;
bool switch2State = false;
bool switch3State = false;
bool switch4State = false;

unsigned long currentTime = 0; // used to create a polling rate without using delay()
int releaseDebounce = 12;
int switch1Count = 0;
int switch2Count = 0;
int switch3Count = 0;
int switch4Count = 0;
bool switch1Pressed = false;
bool switch2Pressed = false;
bool switch3Pressed = false;
bool switch4Pressed = false; // all above used for debouncing

long potValue = 0;
long prevPotValue = 0; // both set to zero to initialise so that they start even

//const layerMatrix = math.Matrix([[KEY_SPACE, KEY_CAPS_LOCK, KEY_TAB, KEY_PERIOD], 
//[KEY_A, KEY_B, KEY_C, KEY_D],
//[KEY_E, KEY_F, KEY_G, KEY_H],
//[KEY_I, KEY_J, KEY_K, KEY_L],
//[KEY_M, KEY_N, KEY_O, KEY_P],
//[KEY_Q, KEY_R, KEY_S, KEY_T],
//[KEY_U, KEY_V, KEY_W, KEY_X],
//[KEY_Y, KEY_Z, KEY_ENTER, KEY_BACKSPACE]]);

int currentLayer = 0; // value used to search the matrix later
int layerMatrix[][4] = {
  {KEY_SPACE, KEY_CAPS_LOCK, KEY_TAB, KEY_PERIOD},
  {KEY_A, KEY_B, KEY_C, KEY_D},
  {KEY_E, KEY_F, KEY_G, KEY_H},
  {KEY_I, KEY_J, KEY_K, KEY_L},
  {KEY_M, KEY_N, KEY_O, KEY_P},
  {KEY_Q, KEY_R, KEY_S, KEY_T},
  {KEY_U, KEY_V, KEY_W, KEY_X},
  {KEY_Y, KEY_Z, KEY_ENTER, KEY_BACKSPACE}
}; // matrix so that we can easily access the desired keycode for the layer defined by the value of the pot
// note that keycodes correspond to integer values internally, which is why we use int as the type for the array here

void setup() {
  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);
  pinMode(switch3Pin, INPUT_PULLUP);
  pinMode(switch4Pin, INPUT_PULLUP);

  Serial.begin(9600);
  Keyboard.begin();
  potValue = analogRead(A6);
  prevPotValue = analogRead(A6);
}

void loop() {
  while(true) {
    currentTime = micros();
    if(currentTime % 500 < 50) { // in other words, polling rate of 10000hz, with leeway to make sure the function is triggered every hz.
      checkPins();
    }
    // if(currentTime % 200000 < 50) {
    //   Serial.println(potValue);
    //   Serial.println(currentLayer);
    // }
    // Uncomment the above if you want to be able to see what layer you are currently on via serial monitor
  }
}

void checkPins() {
  switch1State = digitalRead(switch1Pin);
  switch2State = digitalRead(switch2Pin);
  switch3State = digitalRead(switch3Pin);
  switch4State = digitalRead(switch4Pin); // get the current status of each pin, HIGH (1) when depressed and LOW (0) when pressed
  potValue = analogRead(A6);
  if(potValue < 121) {
    currentLayer = 7;
  }
  else if(potValue < 250) {
    currentLayer = 6;
  }
  else if(potValue < 379) {
    currentLayer = 5;
  }
  else if(potValue < 508) {
    currentLayer = 4;
  }
  else if(potValue < 637) {
    currentLayer = 3;
  }
  else if(potValue < 766) {
    currentLayer = 2;
  }
  else if(potValue < 895) {
    currentLayer = 1;
  }
  else {
    currentLayer = 0;
  } // sets the value for the current layer depending on which range the potentiometer's value is in.
  

  if(!switch1State) { // when the switch is pressed down
    pressSwitch1();
    switch1Count = 0; // reset the counter
  } else {
    switch1Count++;
    if(switch1Count >= releaseDebounce) { // if the switch has been unheld for 12 polling cycles, release
      releaseSwitch1();
    }
  }

  if(!switch2State) {
    pressSwitch2();
    switch2Count = 0;
  } else {
    switch2Count++;
    if(switch2Count >= releaseDebounce) {
      releaseSwitch2();
    }
  }

  if(!switch3State) {
    pressSwitch3();
    switch3Count = 0;
  } else {
    switch3Count++;
    if(switch3Count >= releaseDebounce) {
      releaseSwitch3();
    }
  }

  if(!switch4State) {
    pressSwitch4();
    switch4Count = 0;
  } else {
    switch4Count++;
    if(switch4Count >= releaseDebounce) {
      releaseSwitch4();
    }
  }
}

void pressSwitch1() {
  if(!switch1Pressed) { // executes when the switch isn't currently pressed to prevent sending signals every polling cycle the key is held
    Keyboard.press(layerMatrix[currentLayer][0]); // access the keycode by layer matrix at the row of the current layer and column of the respective switch
    switch1Pressed = true;
  }
}

void releaseSwitch1() {
  if(switch1Pressed) { // prevents held release signals
    Keyboard.release(layerMatrix[currentLayer][0]);
    switch1Pressed = false;
  }
}

void pressSwitch2() {
  if(!switch2Pressed) {
    Keyboard.press(layerMatrix[currentLayer][1]);
    switch2Pressed = true;
  }
}

void releaseSwitch2() {
  if(switch2Pressed) {
    Keyboard.release(layerMatrix[currentLayer][1]);
    switch2Pressed = false;
  }
}

void pressSwitch3() {
  if(!switch3Pressed) {
    Keyboard.press(layerMatrix[currentLayer][2]);
    switch3Pressed = true;
  }
}

void releaseSwitch3() {
  if(switch3Pressed) {
    Keyboard.release(layerMatrix[currentLayer][2]);
    switch3Pressed = false;
  }
}

void pressSwitch4() {
  if(!switch4Pressed) {
    Keyboard.press(layerMatrix[currentLayer][3]);
    switch4Pressed = true;
  }
}

void releaseSwitch4() {
  if(switch4Pressed) {
    Keyboard.release(layerMatrix[currentLayer][3]);
    switch4Pressed = false;
  }
}