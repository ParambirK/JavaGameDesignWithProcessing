#include "HUSKYLENS.h"
int oldx;
int newx; 
int oldy;
int newy;
//Button Mapping
  int x = 11;
  int cir = 2;
  int sq = 3;
  int tri = 4;
//D-pad Mapping
  int up = 5;
  int down = 6;
  int right = 7;
  int left = 8;

HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
void printResult(HUSKYLENSResult result);


void setup() {
    Serial.begin(9600);
    Wire.begin();
    pinMode(x, INPUT);
    pinMode(cir, INPUT);
    pinMode(sq, INPUT);
    pinMode(tri, INPUT);
    pinMode(up, INPUT);
    pinMode(down, INPUT);
    pinMode(right, INPUT);
    pinMode(left, INPUT);

    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {

    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);

            int xdiff = xdifference(result); 
            Serial.println(xdiff);

            if(xdiff > 19){
              Serial.println("BIG DIFF!");
              stageFatality();
            }
            
        }
        
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}

//TRIGGER A COMBO? FUNCTIONS
int xdifference (HUSKYLENSResult result){
  if(result.command == COMMAND_RETURN_BLOCK){
    oldx=result.xCenter;
    delay(5);
    newx=result.xCenter;
    return newx-oldx;
  }
}

int ydifference (HUSKYLENSResult result){
  if(result.command == COMMAND_RETURN_BLOCK){
  oldy=result.xCenter;
  delay(5);
  newy=result.xCenter;
  return newy-oldy;
  }
}


//ADD JOSHUA'S COMBO BUTTONS FUNCTIONS HERE
void stageFatality(){
  pushButton(left, 115);
  pushButton(down, 115);
  pushButton(down, 115);
  pushButton(sq, 115); 
}


void pushButton(int pin, int t){
  digitalWrite(pin, HIGH); 
  delay(t);
  digitalWrite(pin, LOW);
}





