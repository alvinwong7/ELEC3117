// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define GENDER 0
#define AGE 1
#define RAINY 2
#define HOT 3
#define SMOKE 4
#define STOP 5
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

//defines pins
int temperaturePin = 4; //ANALOGUE A4
int nRainIn = 5; //ANALOGUE A5
//int smokePin = 6; // ANALOGUE A6
int nRainDigitalIn = 2; //DIG 2
int nRainVal;
boolean bIsRaining = false;
int rain;
int ledPin = 13;                // choose the pin for the LED
int PIRPin = 3;               // choose the input pin (for PIR sensor)
int cond = GENDER;
int prevCond = STOP;
char data;
int a = 0;

// Gender and age variables
int male = 0;
int female = 0;
int totalAge = 0;
int firstNum = 0;
int secondNum = 0;
int tmp = 0;
int avgAge = 0;


void setup() {
  //Matrix Setup
  matrix.begin();  
  Serial.begin(9600);
  pinMode(2,INPUT); // Declares Rain as input
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(PIRPin, INPUT);     // declare PIR as input
  //pinMode(smokePin, INPUT);
  
}

void loop() {
  //FOR TEMP DETECTION 

    
  
    if (weather() == 1 && (prevCond != cond || prevCond == GENDER)){
      cond = RAINY;
    } else if (temperature() > 36 && (prevCond != cond || prevCond == GENDER)){
      cond = HOT;
    //} else if (analogRead(smokePin) > 100 && (prevCond != cond || prevCond == GENDER)) {
    //  cond = SMOKE;
    } else if (digitalRead(PIRPin) == LOW) {
      cond = STOP;
    } else {
      cond = GENDER;
    }
    prevCond = cond;

    // Gender and age
    if (Serial.available()) {
      male = 0;
      female = 0;
      totalAge = 0;
      firstNum = 0;
      secondNum = 0;
      tmp = 0;
      avgAge = 0;
    }
    while(Serial.available()) {
      data = Serial.read();
      delay(1);
      if (data == 77 && tmp == 0) {
        male += 1;
        tmp = 1;
      } else if (data == 70 && tmp == 0) {
        female += 1;
        tmp = 1;
      } else if (tmp == 1) {
        tmp = 2;
        firstNum = data - 48;
      } else if (tmp == 2) {
        tmp = 0;
        secondNum = data - 48;
        totalAge = totalAge + (firstNum*10 + secondNum);
      }
    }
    avgAge = totalAge/(male+female);
    

    if (cond == GENDER){
      gender(male,female);
      delay(2500);
      if (avgAge > 0) {
        age(avgAge);
      } else {
        age(0);
      }
      delay(2500);
    } else if (cond == RAINY){
      rainy();
      delay(5000);
    } else if (cond == HOT){
      hot();
      delay(5000);
    /*} else if (cond == SMOKE){
      smoking();
      delay(5000);*/
    } else if (cond == STOP) {
      matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 0));
      delay(1);
    }
}

void smoking(){
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 0));
  matrix.fillRect(8, 16, 8, 2, matrix.Color333(7, 5, 0)); 
  matrix.fillRect(15, 16, 10, 2, matrix.Color333(7, 5, 5)); 
  matrix.setCursor(0, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);
  matrix.print('S'); 
  matrix.print('A');
  matrix.print('Y');
  matrix.setCursor(20, 0);  
  matrix.print('N'); 
  matrix.print('O');
  matrix.drawPixel(25, 21, matrix.Color333(7, 4, 4));  
  matrix.drawPixel(25, 19, matrix.Color333(7, 4, 4)); 
  matrix.drawPixel(25, 24, matrix.Color333(7, 4, 4));

  //matrix.drawLine(13, 20, 17, 25, matrix.Color333(7, 7, 7));
  matrix.drawLine(13, 14, 17, 19, matrix.Color333(7, 0, 0));
  matrix.drawLine(17, 14, 13, 19, matrix.Color333(7, 0, 0));
  //matrix.drawLine(31, 0, 0, 31, matrix.Color333(7, 0, 0));
}

void rainy(){

  int col = 0;
  int row;
  //background
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 7));
  //vertical
  for (row = 13; row < 25; row++){
    matrix.drawPixel(15, row, matrix.Color333(7, 7, 7));  
  }
  //does hook
  col = 15;
  for (row = 24; row>22; row--){
    col--;
    matrix.drawPixel(col, row, matrix.Color333(7, 7, 7));
  }
  //top cover
  for (col = 10; col < 21; col++){
    matrix.drawPixel(col, 9, matrix.Color333(7, 7, 7));    
  }
    matrix.drawPixel(9, 10, matrix.Color333(7, 7, 7));    
    matrix.drawPixel(21, 10, matrix.Color333(7, 7, 7)); 
    matrix.drawPixel(8, 11, matrix.Color333(7, 7, 7));
    matrix.drawPixel(22, 11, matrix.Color333(7, 7, 7)); 
    matrix.drawPixel(7, 12, matrix.Color333(7, 7, 7));
    matrix.drawPixel(23, 12, matrix.Color333(7, 7, 7)); 
  //bottom horizontal
  for (col = 8; col < 23; col++){
     matrix.drawPixel(col, 12, matrix.Color333(7, 7, 7));  
  }
    
  //Rain drops
  int count=0;
  while(count < 100){
    count ++;
    for (row = 2; row < 8; row= row+2){
      for (col = 8; col < 23; col= col + 2){
        matrix.drawPixel(col, row, matrix.Color333(7, 7, 7)); 
      }    
    }
  }
}

void gender(int boys, int girls){
  matrix.fillRect(16, 0, 16, 32, matrix.Color333(0, 0, 7));
  matrix.fillRect(0, 0, 16, 32, matrix.Color333(7, 0, 6));
  //FEMALE
  matrix.setCursor(3, 8);    // start at top left, with one pixel of spacing
  matrix.setTextSize(2);     // size 1 == 8 pixels high
  matrix.print(girls);

  //MALE
  matrix.setCursor(19, 8);    // start at top left, with one pixel of spacing
  matrix.setTextSize(2);     // size 1 == 8 pixels high
  matrix.print(boys);
}

void age(int age){
  //DIGIT CALC
  int dig1, dig2;
  dig1 = age / 10;
  dig2 = age - dig1*10;
  
  //background
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 0)); 
 //AGE
  matrix.setCursor(0, 0);
  matrix.setTextSize(0.5);
  matrix.print('A');  
  matrix.print('G');
  matrix.print('E');  
  matrix.setCursor(5, 8);    // start at top left, with one pixel of spacing
  matrix.setTextColor(matrix.Color333(7,7,7)); 
  matrix.setTextSize(2.8);     // size 1 == 8 pixels high
  matrix.print(dig1);   
  matrix.print(dig2);
}

void hot(){
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 0));
  matrix.setTextSize(0.5);
  matrix.setCursor(5, 0);
  matrix.print('s');  
  matrix.print('o');
  matrix.print('f');  
  matrix.print('t');  
  matrix.setCursor(2, 24);
  matrix.print('s');  
  matrix.print('e');
  matrix.print('r');  
  matrix.print('v');  
  matrix.print('e');   
  matrix.fillCircle(15, 14, 4, matrix.Color333(1,0,6)); 
  matrix.fillTriangle(11,15,19,15, 15, 23,matrix.Color333(7, 7, 0));
}

int temperature(){
    int row;
    int cond = GENDER;
    int reading;
    float voltage, temp;
    reading = analogRead(temperaturePin); 
    voltage = reading * 5.0;
    voltage /= 1024.0; 
    temp = (voltage - 0.5) * 100 ; 
    return temp;
}

int weather() {
  //-RAIN SENSOR -------------------------------//
  
  nRainVal = analogRead(nRainIn);
  bIsRaining = !(digitalRead(nRainDigitalIn));
  
  if(bIsRaining){
    rain = 1;
  }
  else{
    rain = 0;
  }

  delay(200);
  return (rain);
  //--------------------------------------------//
}
