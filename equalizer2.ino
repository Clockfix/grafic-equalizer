#include <FastLED.h>

// Parametri LED matricai
#define LED_PIN  5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS 20

// Params for width and height
const uint8_t kMatrixWidth = 10;
const uint8_t kMatrixHeight = 14;
// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);
uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}
//----------------------------------
//Parametri MSGEQ7
int Reset=10;
int Strobe=9;
int InputL=0;
int InputR=1;
//---------------------------------
//parametri RoteryEncoder
const int encoderPinA = 2; 
const int encoderPinB = 4; 
const int switchPin = 3; 
//--------------------------------
int animation;  // animacijas rezims
int color;    // krasas rezims
bool mode;   // mode=0 - rezimu maina; mode=1 - krasas maina

void setup() {
  Serial.begin(9600);
  //---------
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
  //---------
  pinMode(Reset, OUTPUT);
  pinMode(Strobe, OUTPUT);
  pinMode(InputL, INPUT);
  pinMode(InputR, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoder, CHANGE); // interupt 0(pin2)
  pinMode(switchPin, INPUT); 
  attachInterrupt(digitalPinToInterrupt(switchPin), doSwitch, RISING); // interupt 1(pin3)
  //-------------
  pinMode(encoderPinA,INPUT);
  pinMode(encoderPinB,INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  // reset equalizer
  digitalWrite(Reset, HIGH);
  delayMicroseconds(100);
  digitalWrite(Reset, LOW);
  digitalWrite(Strobe, HIGH);
  delayMicroseconds(40);
}

void doEncoder()
{
  if(mode==0){
    if(digitalRead(encoderPinA) == digitalRead(encoderPinB)){
      animation++;
      if(animation>3){
        animation=3;
      }
      Serial.println(animation);
    }
    else{
      animation--;
      if(animation<0){
        animation=0;
      }
      Serial.println(animation);
  }
  } else {
    if(digitalRead(encoderPinA) == digitalRead(encoderPinB)){
      color++;
      if(color>7){
        color=7;
      }
      Serial.println(color);
    }
    else{
      color--;
      if(color<0){
        color=0;
      }
      Serial.println(color);
    }
  }
}

void doSwitch(){
  mode= !mode;
}

void getcolor(int Cred[], int Cgreen[], int Cblue[]){
  if (color==0){
    if(animation==0 || animation==1  ){
      for(int a=0;a<4;a++){
        Cred[a]=0;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
      for(int a=3;a<8;a++){
        Cred[a]=255;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
      for(int a=8;a<10;a++){
        Cred[a]=255;
        Cgreen[a]=0;
        Cblue[a]=0;
      }
    } else {//if(animation==2 || animation==3){
        Cred[9]=255;
        Cgreen[9]=0;
        Cblue[9]=0;
        Cred[4]=255;
        Cgreen[4]=0;
        Cblue[4]=0;
      for(int a=2;a<4;a++){
        Cred[a]=255;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
      for(int a=7;a<9;a++){
        Cred[a]=255;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
      for(int a=0;a<2;a++){
        Cred[a]=0;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
      for(int a=5;a<7;a++){
        Cred[a]=0;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
    }
    
  } else if(color==1) {
    for(int a=0;a<10;a++){
        Cred[a]=0;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
  } else if(color==2) {
    for(int a=0;a<10;a++){
        Cred[a]=0;
        Cgreen[a]=0;
        Cblue[a]=255;
      }
  } else if(color==3) {
    for(int a=0;a<10;a++){
        Cred[a]=255;
        Cgreen[a]=0;
        Cblue[a]=0;
      }
  } else if(color==4) {
    for(int a=0;a<10;a++){
        Cred[a]=255;
        Cgreen[a]=255;
        Cblue[a]=0;
      }
  } else if(color==5) {
    for(int a=0;a<10;a++){
        Cred[a]=0;
        Cgreen[a]=255;
        Cblue[a]=255;
      }
  } else if(color==6) {
    for(int a=0;a<10;a++){
        Cred[a]=255;
        Cgreen[a]=0;
        Cblue[a]=255;
      }
  } else if(color==7) {
    for(int a=0;a<10;a++){
        Cred[a]=255;
        Cgreen[a]=255;
        Cblue[a]=255;
      }
  }   
}

void getdata(int ValueL[], int ValueR[]) {
    for (int i=0;i<7;i++){
      digitalWrite(Strobe, LOW);
      delayMicroseconds(20);
      ValueL[i]=analogRead(InputL);
      ValueL[i]=map(ValueL[i], 0, 1023, 0, 11);
      ValueR[i]=analogRead(InputR);
      ValueR[i]=map(ValueR[i], 0, 1023, 0, 11);
      delayMicroseconds(20);
      digitalWrite(Strobe, HIGH);
      delayMicroseconds(40);
    }
}

void steriomode(){
  int dataL[7];
  int dataR[7];
  getdata(dataL, dataR);
  int red[10];
  int green[10];
  int blue[10];
  getcolor(red, green, blue)  ;
  for (int y=0;y<7;y++){

    for (int x=0;x<10;x++){
      if (dataL[y]-1>=x){
        leds[ XYsafe( x, y ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( x, y ) ]  = CRGB(0,0,0);
        }
    }    
  }
  for (int y=7;y<14;y++){
    for (int x=0;x<10;x++){
      if (dataR[y-7]-1>=x){
        leds[ XYsafe( x, y ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( x, y ) ]  = CRGB(0,0,0);
        }
    }  
  }
  FastLED.show();
}

void monomode(){
  int dataL[7];
  int dataR[7];
  getdata(dataL, dataR);
  int data[7];
  for (int a=0;a<7;a++){
    data[a]=(dataL[a]+dataR[a])/2;
  }
  int red[10];
  int green[10];
  int blue[10];
  getcolor(red, green, blue);
      
  for (int y=0;y<14;y=y+2){

    for (int x=0;x<10;x++){
      if (data[y/2]-1>=x){
        leds[ XYsafe( x, y ) ]  = CRGB(red[x],green[x],blue[x]);
        leds[ XYsafe( x, y+1 ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( x, y ) ]  = CRGB(0,0,0);
          leds[ XYsafe( x, y+1 ) ]  = CRGB(0,0,0);
        }
    }
       
  }
  FastLED.show();
}

void steriomode2(){
  int red[10];
  int green[10];
  int blue[10];
  getcolor(red, green, blue) ;
  int dataL[7];
  int dataR[7];
  getdata(dataL, dataR);
  for (int a=0;a<7;a++){
    dataL[a]=dataL[a]/2;
    dataR[a]=dataR[a]/2;
  }  
  for (int y=0;y<7;y++){

    for (int x=4;x<10;x++){
      if (dataL[y]-1>=x-5){
        leds[ XYsafe( x, y ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( x, y ) ]  = CRGB(0,0,0);
        }
    }
    for (int x=4;x>-1;x--){
      if (dataL[y]-1>=x){
        leds[ XYsafe( 4-x, y ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( 4-x, y ) ]  = CRGB(0,0,0);
        }
    }
    
  }
  for (int y=7;y<14;y++){

    for (int x=4;x<10;x++){
      if (dataR[y-7]-1>=x-5){
        leds[ XYsafe( x, y ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( x, y ) ]  = CRGB(0,0,0);
        }
    }
    for (int x=4;x>-1;x--){
      if (dataR[y]-1>=x){
        leds[ XYsafe( 4-x, y ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( 4-x, y ) ]  = CRGB(0,0,0);
        }
    }
    
  }
  FastLED.show();
}

void monomode2(){
  int red[10];
  int green[10];
  int blue[10];
  getcolor(red, green, blue);
  int dataL[7];
  int dataR[7];
  getdata(dataL, dataR);
  for (int a=0;a<7;a++){
    dataL[a]=dataL[a]/2;
    dataR[a]=dataR[a]/2;
  } 
  int data[7];
  for (int a=0;a<7;a++){
    data[a]=(dataL[a]+dataR[a])/2;
  } 
  for (int y=0;y<14;y=y+2){

    for (int x=4;x<10;x++){
      if (data[y/2]-1>=x-5){
        leds[ XYsafe( x, y ) ]  = CRGB(red[x],green[x],blue[x]);
        leds[ XYsafe( x, y+1 ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( x, y ) ]  = CRGB(0,0,0);
          leds[ XYsafe( x, y+1 ) ]  = CRGB(0,0,0);
        }
    }
    for (int x=4;x>-1;x--){
      if (data[y/2]-1>=x){
        leds[ XYsafe( 4-x, y ) ]  = CRGB(red[x],green[x],blue[x]);
        leds[ XYsafe( 4-x, y+1 ) ]  = CRGB(red[x],green[x],blue[x]);
      }else{
          leds[ XYsafe( 4-x, y ) ]  = CRGB(0,0,0);
          leds[ XYsafe( 4-x, y+1 ) ]  = CRGB(0,0,0);
        }
    }
    
  }
  FastLED.show();
}

void loop() {
  if(animation==0){
    monomode();
  } else if(animation==1) {
    steriomode();
  }else if(animation==2){
    steriomode2();
  }else {
    monomode2();
  }
}
