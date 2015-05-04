#include <RGBConverter.h>

#include <Audio.h>
#include <Wire.h>
#include <SD.h>

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_S6D02A1.h> // Hardware-specific library
#include <Adafruit_ILI9341.h>
#include <SPI.h>


#define TFT_DC  20
#define TFT_CS  21
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, 8);


// use HW SPI interface for TFT
//Adafruit_S6D02A1 tft = Adafruit_S6D02A1(cs, dc, rst);
// Option 1: use any pins but a little slower
//Adafruit_S6D02A1 tft = Adafruit_S6D02A1(cs, dc, mosi, sclk, rst);


const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S       audioInput;         // audio shield: mic or line-in
AudioMixer4         summer;
AudioAnalyzeFFT1024  myFFT;
AudioOutputI2S      audioOutput;        // audio shield: headphones & line-out

// Create Audio connections between the components
//
//AudioConnection c1(audioInput, 0, audioOutput, 0);
//AudioConnection c2(audioInput, 1, audioOutput, 1);
AudioConnection c3(audioInput, 0, summer, 0);  // sum the shifted filter outputs which will supress the image
AudioConnection c4(audioInput, 1, summer, 1);
AudioConnection c5(summer, 0, myFFT, 0);


// Create an object to control the audio shield.
// 
AudioControlSGTL5000 audioShield;



void setup() {
  
  SPI.setMOSI(7);
  SPI.setSCK(14);
  tft.begin();
  //Serial1.begin(115200);
  Serial1.begin(921600);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(60);
//  AudioControlSGTL5000 codec;
//  codec.unmuteLineout();
 
  // If your TFT's plastic wrap has a Black Tab, use the following:
 // tft.initR(INITR_BLACKTAB);   // initialize a S6D02A1S chip, black tab
  // If your TFT's plastic wrap has a Red Tab, use the following:
  //tft.initR(INITR_REDTAB);   // initialize a S6D02A1R chip, red tab
  // If your TFT's plastic wrap has a Green Tab, use the following:
  //tft.initR(INITR_GREENTAB); // initialize a S6D02A1R chip, green tab
  tft.setRotation(1);
  Serial1.println("init");
//  SPI.setClockDivider(SPI_CLOCK_DIV2); // crank up the spi
  uint16_t time = millis();
  tft.fillScreen(ILI9341_BLACK);
  time = millis() - time;

  Serial1.println(time, DEC);
//  delay(500);

  // large block of text
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 230);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextWrap(true);
  tft.print("Teensy Spectrum Analyser 2");

  delay(1000);


  // pin 21 will select rapid vs animated display
  //pinMode(21, INPUT_PULLUP);
}

int count=0;
const int nsum[16] = {1, 1, 2, 2, 3, 4, 5, 6, 6, 8, 12, 14, 16, 20, 28, 24};
int peak[512];
int maximum[16];
unsigned long last_time = millis();

void loop() {
  if (myFFT.available()) {
    
/*    
    // convert the 128 FFT frequency bins
    // to only 16 sums, for a 16 character LCD
    int sum[32];
    int i;
    for (i=0; i<16; i++) {
      sum[i] = 0;
    }
    int n=0;
    int count=0;
    for (i=0; i<128; i++) {
      sum[n] = sum[n] + myFFT.output[i];
      count = count + 1;
      if (count >= nsum[n]) {
        Serial.print(count);
        Serial.print(" ");
        n = n + 1;
        if (n >= 326) break;
        count = 0;
      }
    }
*/
    // The range is set by the audio shield's
    // knob, which connects to analog pin A1.
    int scale;
    //scale = 2 + (1023 - analogRead(A1)) / 7;
    scale = 30;
/*    Serial.print(" - ");
    Serial.print(scale);
    Serial.print("  ");
*/

  for (int16_t x=0; x < 260; x+=2) {
//    tft.drawFastVLine(x, 0, myFFT.output[x]/scale, ILI9341_GREEN);
     int bar=abs(myFFT.output[x])/scale;
     ffttorgb(bar);
     if (bar >225) bar=225;
     if (bar > peak[x]) peak[x]=bar;
     tft.drawFastVLine(x, 225-bar,bar, ILI9341_MAGENTA);
//     tft.drawFastVLine(x+1, 20, abs(myFFT.output[x]/scale), ILI9341_GREEN);
     tft.drawFastVLine(x, 0, 225-bar, ILI9341_BLACK);    
//     tft.drawFastVLine(x+1, 20+abs(myFFT.output[x]/scale), 80, ILI9341_BLACK); 
     tft.drawPixel(x,225-peak[x], ILI9341_YELLOW);
//     tft.drawFastVLine(x, 225-peak[x],bar, ILI9341_BLUE);    
     if(peak[x]>0) peak[x]-=5;
  }
  
/*
    for (int i=0; i<16; i++) {
      // Reduce the range to 0-8
      int val = sum[i] / scale;
      if (val > 8) val = 8;

      // Compute an animated maximum, where increases
      // show instantly, but if the number is less that
      // the last displayed value, decrease it by 1 for
      // a slow decay (looks pretty)
      if (val >= maximum[i]) {
        maximum[i] = val;
      } else {
        if (maximum[i] > 0) maximum[i] = maximum[i] - 1;
      }

      // a switch on pin 22 select whether we show the
      // slower animation or the direct/fast data
      if (digitalRead(21) == HIGH) {
        val = maximum[i];
      }

      // print each custom digit
      if (val == 0) {
 //       lcd.write(' ');
      } else {
 //       lcd.write(val - 1);
      }
      
      Serial.print(sum[i]);
      Serial.print("=");
      Serial.print(val);
      Serial.print(",");
    }
 */
// Change this to if(1) for measurement output
if(0) {
/*
  For PlaySynthMusic this produces:
  Proc = 20 (21),  Mem = 2 (8)
*/
  if(millis() - last_time >= 5000) {
    Serial.print("Proc = ");
    Serial.print(AudioProcessorUsage());
    Serial.print(" (");    
    Serial.print(AudioProcessorUsageMax());
    Serial.print("),  Mem = ");
    Serial.print(AudioMemoryUsage());
    Serial.print(" (");    
    Serial.print(AudioMemoryUsageMax());
    Serial.println(")");
    last_time = millis();
  }
} 
 //   Serial.println();
    count = 0;
  }
}


void ffttorgb(int value){
 static long lastPrintTime = millis();
 int rgb[] = {0,0,0};

 if(millis() - lastPrintTime >= 0) {
   
   //Serial.println(value);
   Serial1.print('.', BYTE);
   Serial1.print(abs(myFFT.output[0])/30, BYTE);
   //Serial1.write(abs(myFFT.output[0])/30);
   //Serial1.print(",");
   Serial1.print(abs(myFFT.output[2])/30, BYTE);
   //Serial1.write(abs(myFFT.output[2])/30);
   //Serial1.print(",");
   Serial1.println(abs(myFFT.output[1])/30, BYTE);
   //Serial1.write(abs(myFFT.output[1])/30);
   //Serial1.println(myFFT.read(5)+myFFT.read(6)+myFFT.read(7)+myFFT.read(8));
  

   //hsvToRgb(double h, double s, double v, byte rgb[]);
   
  lastPrintTime = millis(); 
 }
  
  
}


