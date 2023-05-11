#include <OLED_I2C.h>
#include "pitches.h"
OLED myOLED(SDA, SCL, 8);

const int rocketLenght = 10;  // Rocket body Length
const int rocketHeight = 5;    // Rocket body Hight
const int maxX = 128;         // Maximum X-value
const int maxY = 64;          // Maximum Y-value
int y;                        // Rocket Y-coordinate
int x = 8;                    // Rocket X-coordinate
int pixX;                     // Pixel coordinate X
int pixY;                     // Pixel coordinate Y
int flameR = 8;               // Flame radius
int potValue;
int intensFire = 10;  // Fire pixel concentration
int astR = 4;         // Asteroid radius
int minastR = 2;
int maxastR = 10;
const int astPixConcent = 20;  // Pixels concentration inside the asteroid
int rOut = 5;
int astX = 128;
int astY;
float astSpd;   // Asteroid speed
int score = 100;  // Game score
int pixExpX;    //  Explosion coordinate X
int pixExpY;    //  Explsion coordinate Y
int explosionIntetsivity = 10;
int explosionR = 15;
int lifeCounter = 3;
int ammoCounter = 3;
int ammoPinStart = 5;
bool fire;
int gameSpeedCount = 0;
int numPinStart = 11;
int selected = 0;
int entered = -1;
int cursorY = 5;
int loopControl = 0;
int spaceControl = 0;
bool fireCount = false;
int gameSpeed = 1000;
int helpingCounter = 1;
int shield = 0;

int blockMatrix[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

unsigned long currentTime, previousTime = 0, currentBuzTime, previousBuzTime = 0, currentLifeTime, previousLifeTime = 0;
unsigned long currentLifeBarrierTime, previousLifeBarrierTime =0, speedTime, previousSpeedTime = 0;

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3
};
int noteDurations[] = {
  4, 8, 8, 4
};
void displayMenu() {  //To show menu
  int down = digitalRead(15);
  int up = digitalRead(16);
  int enter = digitalRead(17);

  if (up == LOW) {
    selected = selected - 1;
    if (cursorY >= 5 && cursorY <= 25) {
      cursorY -= 20;
    }

    delay(200);
  };
  if (down == LOW) {
    selected = selected + 1;
    if (cursorY >= 5 && cursorY <= 25) {
      cursorY += 20;
    }
    delay(200);
  };
  if (enter == LOW) {
    entered = selected;
  };

  myOLED.clrScr();
  myOLED.print(">", LEFT, cursorY);
  myOLED.print("DIFFICULT 1", CENTER, 5);
  myOLED.print("DIFFICULT 2", CENTER, 20);
  myOLED.print(cursorY, RIGHT, 50);

  if (entered == 0) {
    myOLED.clrScr();
    selected = 5;
    loopControl = 1;
  }
  if (entered == 1) {
    myOLED.clrScr();
    selected = 6;
    loopControl = 1;
  }
}
// Function for drawing rocket:
void drawRocket(int x, int y) {
  myOLED.drawRect(x, y, x + rocketLenght, y + rocketHeight);
  myOLED.drawLine(x + rocketLenght, y + rocketHeight, x + rocketLenght + 5, y + rocketHeight / 2);
  myOLED.drawLine(x + rocketLenght, y, x + rocketLenght + 5, y + rocketHeight / 2);
  myOLED.drawLine(x + 5, y + rocketHeight, x - 2, y + 8);  // Down
  myOLED.drawLine(x, y + rocketHeight, x - 2, y + 8);
  myOLED.drawLine(x + 5, y, x - 2, y - 4);  // Up
  myOLED.drawLine(x, y, x - 2, y - 4);
}

void drawFlame(int x, int y) {
  for (int i = 0; i <= intensFire; i++) {
    pixX = random(x - flameR, x);
    pixY = random(y - 2, y + flameR);
    myOLED.setPixel(pixX, pixY);
  }
}

void Asteroid(int x, int y) {
  myOLED.drawCircle(x + astR, y + astR, astR);
}
void Shield(int centerX, int centerY, int leftX, int leftY, int rightX, int rightY){
  myOLED.drawCircle(centerX + astR, centerY + astR, astR);
  myOLED.drawLine(leftX, leftY, rightX, rightY);
}
void Junk(int leftX, int leftY, int rightX, int rightY) {
  myOLED.drawRect(leftX, leftY, rightX, rightY);
}
void Laser(int leftX, int leftY, int rightX, int rightY) {
  myOLED.drawLine(leftX, leftY, rightX, rightY);
}
void Life(int leftX, int leftY, int rightX, int rightY, int midTopX, int midTopY, int midBottomX, int midBottomY) {
  myOLED.drawLine(leftX, leftY, rightX, rightY);
  myOLED.drawLine(midTopX, midTopY, midBottomX, midBottomY);
}
void Ammo(int leftX, int leftY, int rightX, int rightY, int bottomX, int bottomY) {
  myOLED.drawLine(leftX, leftY, rightX, rightY);
  myOLED.drawLine(leftX, leftY, bottomX, bottomY);
}


void Explosion(int x, int y) {
  for (int i = 0; i < explosionIntetsivity; i++) {
    pixExpX = random(x, x + explosionR);
    pixExpY = random(y - explosionR, y + explosionR);
    myOLED.setPixel(pixExpX, pixExpY);
    myOLED.setPixel(pixExpX + 1, pixExpY);
    myOLED.setPixel(pixExpX, pixExpY + 1);
    myOLED.update();
  }
}
extern uint8_t SmallFont[];

void setup() {
  //For Leds
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  //For Buzzer
  pinMode(8, OUTPUT);
  //For 7-Segment
  pinMode(13, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);
  //For LDR
  pinMode(A3, INPUT);

  myOLED.begin();
  Serial.begin(9600);
  myOLED.setFont(SmallFont);
  analogWrite(A1, 1023);
  analogWrite(A2, 0);
  potValue = analogRead(A8);
  randomSeed(analogRead(A0));
  noTone(8);
}
int ledCount = 0;  //To light up LED's one time
void loop() {
  if (loopControl == 0) {
    if (selected == 0 || selected == 1) {
      displayMenu();
      myOLED.update();
      int ldr = analogRead(A3);
      //Serial.println(ldr);
      //delay(50);
      if (ldr > 200) {
        myOLED.invert(true);
      }
      if (ldr < 200) {
        myOLED.invert(false);
      }
    }
  }
  if (selected == 5 && loopControl == 1) {  //Assign random value
    difficultLoopOne();
  }
  if (selected == 6 && loopControl == 1) {
    difficultLoopTwo();
  }
}
void difficultLoopOne() {
  currentTime = millis();
  myOLED.clrScr();
  if (currentTime - previousTime >= 400 && lifeCounter != 0) {
    if (lifeCounter != 0) {
      int butFire;
      butFire = digitalRead(18);
      potValue = analogRead(A8);
      y = map(potValue, 0, 1023, 0, 7);
      drawRocket(x, ((y + 1) * 8) - 6);
      drawFlame(x, ((y + 1) * 8) - 6);
      //Serial.println(y);
      if (ledCount == 0) {  //Led setup just once
        int gunStartPin = 5;
        for (int i = numPinStart; i < numPinStart + lifeCounter; i++) {
          digitalWrite(i, HIGH);
        }
        for (int i = gunStartPin; i < gunStartPin + ammoCounter; i++) {
          digitalWrite(i, HIGH);
        }
        ledCount++;
      }
      //Fire mechanic
      if (butFire == LOW && ammoCounter > 0) {//3 2 1 0
        blockMatrix[y][2] = 6;
        fireCount = true;
        ammoCounter--;
        Serial.println(ammoCounter);
        delay(50);
      }
      if (ammoCounter == 2)
        digitalWrite(7, LOW);
      if (ammoCounter == 1)
        digitalWrite(6, LOW);
      if (ammoCounter == 0)
        digitalWrite(5, LOW);

      //Assign random blocks
      int randomBlockCount = 1;
      if(helpingCounter % 7 == 0){
        randomBlockCount = 2;
      }
      //For escape the must one space
      int k = 0;
      if(spaceControl % 2 == 0){
        while (k != randomBlockCount) {  //Setting blocks
        int blockI = random(0, 9);
        int blockJ = 15;
        if (blockMatrix[blockI][blockJ] == 0) {  //if it is empty then modify
          if(helpingCounter % 7 == 0){
            int blockType = random(3, 6);
            helpingCounter++;
            blockMatrix[blockI][15] = blockType;          //1-asteroid,2-junk,3-lİfe,4-ammo,5-barrier
          }
          else{
            int blockType = random(1, 3);
            helpingCounter++;
            blockMatrix[blockI][15] = blockType;
          }
        } else {
          while (blockMatrix[blockI][blockJ] != 0) {
            blockI = random(0, 9);
          }
          int blockType = random(1, 6);
          blockMatrix[blockI][15] = blockType;
        }  //placement is okey
        k++;
      }
      }
      else{
        for(int i = 0; i < 8; i++){
          blockMatrix[i][15] = 0;
        }
      }
      spaceControl++;
      for (int i = 0; i < 8; i++) {  //Collision Detect
        for (int j = 0; j < 3; j++) {
          if (blockMatrix[i][j] == 1 || blockMatrix[i][j] == 2) {
              if (i == y) {
              currentLifeTime = millis();
              if (currentLifeTime - previousLifeTime >= 3000) {
                lifeCounter--;
                previousLifeTime = currentLifeTime;
              }
              digitalWrite(numPinStart + lifeCounter, LOW);
              blockMatrix[i][j] = 0;
              for (int thisNote = 0; thisNote < 4; thisNote++) {
                currentBuzTime = millis();
                int noteDuration = 500 / noteDurations[thisNote];
                tone(8, melody[thisNote], noteDuration);
                int pauseBetweenNotes = noteDuration * 1.30;
                if (currentBuzTime - previousBuzTime >= pauseBetweenNotes) {
                  previousBuzTime = currentBuzTime;
                  noTone(8);
                }
              }
            }
          } else if (blockMatrix[i][j] == 3) {//Life collect
            if (i == y) {
              blockMatrix[i][j] = 0;
              if (lifeCounter < 3) {
                lifeCounter++;
                digitalWrite(numPinStart + lifeCounter - 1, HIGH);
              }
            }
          } else if (blockMatrix[i][j] == 4) {//Ammo collect
            if (i == y) {
              blockMatrix[i][j] = 0;
              if (ammoCounter < 3) {
                ammoCounter++;
                digitalWrite(ammoPinStart + ammoCounter - 1, HIGH);
              }
            }
          }
          else if(blockMatrix[i][j] == 5){
            if (i == y) {
              blockMatrix[i][j] = 0;
            }
          }
        }
      }
      for (int i = 0; i < 8; i++) {  //Slide matrix
        for (int j = 1; j < 16; j++) {
          int temp = blockMatrix[i][j];
          blockMatrix[i][j - 1] = temp;
          if (j == 15) {
            blockMatrix[i][j] = 0;
          }
        }
      }
      for (int i = 0; i < 8; i++) {  //Slide Lazer
        for (int j = 0; j < 16; j++) {
          if(blockMatrix[i][j] == 6 && fireCount){
            if(blockMatrix[i][j+1] == 1 || blockMatrix[i][j+1] == 2){
              blockMatrix[i][j+1] = 0;
            }
            blockMatrix[i][j] = 0;
            blockMatrix[i][j+2] = 6;
            fireCount = false;
          }
        }
      }
      fireCount = true;
      for (int i = 0; i < 8; i++) {  //Display in screen
        for (int j = 0; j < 16; j++) {
          if (blockMatrix[i][j] == 1) {
            Asteroid(j * 8, i * 8);
          }
          if (blockMatrix[i][j] == 2) {
            Junk(j * 8, i * 8, (j + 1) * 8, (i + 1) * 8);
          }
          if (blockMatrix[i][j] == 3) {
            Life(j * 8, (i * 8) + 4, (j + 1) * 8, (i * 8) + 4, (j * 8) + 4, i * 8, (j * 8) + 4, (i + 1) * 8);
          }
          if (blockMatrix[i][j] == 4) {
            Ammo(j * 8, i * 8, (j + 1) * 8, i * 8, j * 8, (i + 1) * 8);
          }
          if(blockMatrix[i][j] == 5){
            Shield(j * 8, i * 8, j * 8, (i * 8) + 4, (j + 1) * 8, (i * 8) + 4);
          }
          if(blockMatrix[i][j] == 6){
            Laser(j * 8, (i * 8) + 4,(j + 1) * 8, (i * 8) + 4);
          }
        }
      }
      //  myOLED.update();
      Serial.println();
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
          Serial.print(blockMatrix[i][j]);
          Serial.print(" ");
        }
        Serial.println();
      }
     // delay(2000);

      score++;
      sevenSegment(score);
      myOLED.update();
      previousTime = currentTime;
    }
    if (lifeCounter == 0) {
      myOLED.clrScr();
      Explosion(x, y);
      myOLED.print("GAME OVER", 30, 20);
      myOLED.update();
      delay(1000);
      loopControl = 0;
      selected = 0;
      entered = -1;
      lifeCounter = 3;
      ammoCounter = 3;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
          blockMatrix[i][j] = 0;
        }
      }
      myOLED.clrScr();
    }
  }
}
int assign = 0;
void difficultLoopTwo() {
  currentTime = millis();
  speedTime = millis();
  myOLED.clrScr();
  if (currentTime - previousTime >= gameSpeed && lifeCounter != 0) {
    if(speedTime - previousSpeedTime >= 10000){
      gameSpeed = (gameSpeed * 8) / 10;
      previousSpeedTime = speedTime;
      Serial.println(gameSpeed);
    }
    if (lifeCounter != 0) {
      int butFire;
      butFire = digitalRead(18);
      potValue = analogRead(A8);
      y = map(potValue, 0, 1023, 0, 7);
      drawRocket(x, ((y + 1) * 8) - 6);
      drawFlame(x, ((y + 1) * 8) - 6);
      //Serial.println(y);
      if (assign == 0) {  //Led setup just once
        int gunStartPin = 5;
        for (int i = numPinStart; i < numPinStart + lifeCounter; i++) {
          digitalWrite(i, HIGH);
        }
        for (int i = gunStartPin; i < gunStartPin + ammoCounter; i++) {
          digitalWrite(i, HIGH);
        }
        assign++;
      }
      //Fire mechanic
      if (butFire == LOW && ammoCounter > 0) {//3 2 1 0
        blockMatrix[y][2] = 6;
        fireCount = true;
        ammoCounter--;
        Serial.println(ammoCounter);
        delay(50);
      }
      if (ammoCounter == 2)
        digitalWrite(7, LOW);
      if (ammoCounter == 1)
        digitalWrite(6, LOW);
      if (ammoCounter == 0)
        digitalWrite(5, LOW);

      //Assign random blocks
      int randomBlockCount = 1;  //For escape the must one space
      int k = 0;
      if(spaceControl % 2 == 0){
        while (k != randomBlockCount) {  //Setting blocks
        int blockI = random(0, 9);
        int blockJ = 15;
        if (blockMatrix[blockI][blockJ] == 0) {  //if it is empty then modify
          int blockType = random(1, 5);          //1-asteroid,2-junk,3-lİfe,4-ammo
          blockMatrix[blockI][15] = blockType;
        } else {
          while (blockMatrix[blockI][blockJ] != 0) {
            blockI = random(0, 9);
          }
          int blockType = random(1, 5);
          blockMatrix[blockI][15] = blockType;
        }  //placement is okey
        k++;
      }
      }
      else{
        for(int i = 0; i < 8; i++){
          blockMatrix[i][15] = 0;
        }
      }
      spaceControl++;
      for (int i = 0; i < 8; i++) {  //Collision Detect
        for (int j = 0; j < 3; j++) {
          if (blockMatrix[i][j] == 1 || blockMatrix[i][j] == 2) {
            if (i == y) {
              currentLifeTime = millis();
              if (currentLifeTime - previousLifeTime >= 3000) {
                lifeCounter--;
                previousLifeTime = currentLifeTime;
              }
              digitalWrite(numPinStart + lifeCounter, LOW);
              blockMatrix[i][j] = 0;
              for (int thisNote = 0; thisNote < 4; thisNote++) {
                currentBuzTime = millis();
                int noteDuration = 500 / noteDurations[thisNote];
                tone(8, melody[thisNote], noteDuration);
                int pauseBetweenNotes = noteDuration * 1.30;
                if (currentBuzTime - previousBuzTime >= pauseBetweenNotes) {
                  previousBuzTime = currentBuzTime;
                  noTone(8);
                }
              }
            }
          } else if (blockMatrix[i][j] == 3) {
            if (i == y) {
              blockMatrix[i][j] = 0;
              if (lifeCounter < 3) {
                lifeCounter++;
                digitalWrite(numPinStart + lifeCounter - 1, HIGH);
              }
            }
          } else if (blockMatrix[i][j] == 4) {
            if (i == y) {
              blockMatrix[i][j] = 0;
              if (ammoCounter < 3) {
                ammoCounter++;
                digitalWrite(ammoPinStart + ammoCounter - 1, HIGH);
              }
            }
          }
        }
      }
      for (int i = 0; i < 8; i++) {  //Slide matrix
        for (int j = 1; j < 16; j++) {
          int temp = blockMatrix[i][j];
          blockMatrix[i][j - 1] = temp;
          if (j == 15) {
            blockMatrix[i][j] = 0;
          }
        }
      }
      for (int i = 0; i < 8; i++) {  //Slide Lazer
        for (int j = 0; j < 16; j++) {
          if(blockMatrix[i][j] == 6 && fireCount){
            if(blockMatrix[i][j+1] == 1 || blockMatrix[i][j+1] == 2){
              blockMatrix[i][j+1] = 0;
            }
            blockMatrix[i][j] = 0;
            blockMatrix[i][j+2] = 6;
            fireCount = false;
          }
        }
      }
      fireCount = true;
      for (int i = 0; i < 8; i++) {  //Display in screen
        for (int j = 0; j < 16; j++) {
          if (blockMatrix[i][j] == 1) {
            Asteroid(j * 8, i * 8);
          }
          if (blockMatrix[i][j] == 2) {
            Junk(j * 8, i * 8, (j + 1) * 8, (i + 1) * 8);
          }
          if (blockMatrix[i][j] == 3) {
            Life(j * 8, (i * 8) + 4, (j + 1) * 8, (i * 8) + 4, (j * 8) + 4, i * 8, (j * 8) + 4, (i + 1) * 8);
          }
          if (blockMatrix[i][j] == 4) {
            Ammo(j * 8, i * 8, (j + 1) * 8, i * 8, j * 8, (i + 1) * 8);
          }
          if(blockMatrix[i][j] == 6){
            Laser(j * 8, (i * 8) + 4,(j + 1) * 8, (i * 8) + 4);
          }
        }
      }
      //  myOLED.update();
      Serial.println();
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
          Serial.print(blockMatrix[i][j]);
          Serial.print(" ");
        }
        Serial.println();
      }
     // delay(2000);

      score++;
      sevenSegment(score);
      myOLED.update();
      previousTime = currentTime;
    }
    if (lifeCounter == 0) {
      myOLED.clrScr();
      Explosion(x, y);
      myOLED.print("GAME OVER", 30, 20);
      myOLED.update();
      delay(1000);
      loopControl = 0;
      selected = 0;
      entered = -1;
      lifeCounter = 3;
      ammoCounter = 3;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
          blockMatrix[i][j] = 0;
        }
      }
      myOLED.clrScr();
    }
  }
}
//7-Segment
void zero() {
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(28, LOW);
}
void one() {
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  digitalWrite(28, LOW);
}
void two() {
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, LOW);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, LOW);
  digitalWrite(28, HIGH);
}
void three() {
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  digitalWrite(28, HIGH);
}
void four() {
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
}
void five() {
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
}
void six() {
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
}
void seven() {
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  digitalWrite(28, LOW);
}
void eight() {
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
}
void nine() {
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, LOW);
  digitalWrite(27, HIGH);
  digitalWrite(28, HIGH);
}
void zero2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(35, LOW);
}
void one2() {
  digitalWrite(29, LOW);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
}
void two2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, LOW);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(34, LOW);
  digitalWrite(35, HIGH);
}
void three2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, HIGH);
}
void four2() {
  digitalWrite(29, LOW);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
}
void five2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, LOW);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
}
void six2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, LOW);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
}
void seven2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
}
void eight2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
}
void nine2() {
  digitalWrite(29, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(33, LOW);
  digitalWrite(34, HIGH);
  digitalWrite(35, HIGH);
}
void zero3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(42, HIGH);
  digitalWrite(43, LOW);
}
void one3() {
  digitalWrite(37, LOW);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, LOW);
  digitalWrite(41, LOW);
  digitalWrite(42, LOW);
  digitalWrite(43, LOW);
}
void two3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, LOW);
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(42, LOW);
  digitalWrite(43, HIGH);
}
void three3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, LOW);
  digitalWrite(42, LOW);
  digitalWrite(43, HIGH);
}
void four3() {
  digitalWrite(37, LOW);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, LOW);
  digitalWrite(41, LOW);
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
}
void five3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, LOW);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, LOW);
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
}
void six3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, LOW);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
}
void seven3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, LOW);
  digitalWrite(41, LOW);
  digitalWrite(42, LOW);
  digitalWrite(43, LOW);
}
void eight3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, HIGH);
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
}
void nine3() {
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(39, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(41, LOW);
  digitalWrite(42, HIGH);
  digitalWrite(43, HIGH);
}
void sevenSegment(int score) {
  if (score >= 0 && score < 10) {
    switch (score) {
      case 0:
        zero();
        break;
      case 1:
        one();
        break;
      case 2:
        two();
        break;
      case 3:
        three();
        break;
      case 4:
        four();
        break;
      case 5:
        five();
        break;
      case 6:
        six();
        break;
      case 7:
        seven();
        break;
      case 8:
        eight();
        break;
      case 9:
        nine();
        break;
    }
  }
  if (score >= 10 && score < 100) {
    switch (score % 10) {  //Ones digit
      case 0:
        zero2();
        break;
      case 1:
        one2();
        break;
      case 2:
        two2();
        break;
      case 3:
        three2();
        break;
      case 4:
        four2();
        break;
      case 5:
        five2();
        break;
      case 6:
        six2();
        break;
      case 7:
        seven2();
        break;
      case 8:
        eight2();
        break;
      case 9:
        nine2();
        break;
    }
    switch (score / 10) {  //Tens digit
      case 0:
        zero();
        break;
      case 1:
        one();
        break;
      case 2:
        two();
        break;
      case 3:
        three();
        break;
      case 4:
        four();
        break;
      case 5:
        five();
        break;
      case 6:
        six();
        break;
      case 7:
        seven();
        break;
      case 8:
        eight();
        break;
      case 9:
        nine();
        break;
    }
  }
  if (score >= 100) {
    int temp = score;
    switch (score / 100) {  //Hundreds digit
      case 0:
        zero();
        break;
      case 1:
        one();
        break;
      case 2:
        two();
        break;
      case 3:
        three();
        break;
      case 4:
        four();
        break;
      case 5:
        five();
        break;
      case 6:
        six();
        break;
      case 7:
        seven();
        break;
      case 8:
        eight();
        break;
      case 9:
        nine();
        break;
    }
    score = temp;
    switch ((score / 10) % 10) {  //Tens digit
      case 0:
        zero2();
        break;
      case 1:
        one2();
        break;
      case 2:
        two2();
        break;
      case 3:
        three2();
        break;
      case 4:
        four2();
        break;
      case 5:
        five2();
        break;
      case 6:
        six2();
        break;
      case 7:
        seven2();
        break;
      case 8:
        eight2();
        break;
      case 9:
        nine2();
        break;
    }
    score = temp;
    switch (score % 10) {
      case 0:
        zero3();
        break;
      case 1:
        one3();
        break;
      case 2:
        two3();
        break;
      case 3:
        three3();
        break;
      case 4:
        four3();
        break;
      case 5:
        five3();
        break;
      case 6:
        six3();
        break;
      case 7:
        seven3();
        break;
      case 8:
        eight3();
        break;
      case 9:
        nine3();
        break;
    }
  }
}
