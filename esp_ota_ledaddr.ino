/*  Lenta WS2812B & OTA
    https://github.com/FastLED/FastLED/blob/master/examples/Blink/Blink.ino
*/
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h> //игнор:  #pragma message: No hardware SPI pins defined. All SPI access will default to bitbanged output
#include "ota.h"

/*
  ESP01 - 2 LEDs (На модуле ESP-01S установлен один светодиод - на выводе GPIO2)
    "port": "192.168.1.105",

  GPIO0, io
  GPIO1, TX LED-0:on
  GPIO2, io - WLED
  GPIO3 rx/io - ota to gnd
  почемуто не получалось получить io  //esp01 pin2 почемуто занят - возможно им моргает ArduinoOTA 
*/

#define LEDB_PIN 1 //esp01-1 buildin led blue
#define DATA_PIN 2
#define NET_PIN 3 //ota mode to gnd
#define LED_NUM 22
CRGB leds[LED_NUM];
bool net = false;

void setup() 
{  
  pinMode(NET_PIN,INPUT_PULLUP);  // pinMode(NET_PIN, INPUT);
  delay(11);
	if(digitalRead(NET_PIN) == 0) 
  {
    net = true;
    Serial.begin(115200); while (!Serial) {;} Serial.print("\r\nRun. ");
    otaSetup();
  }
  else
  {
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, LED_NUM);  // GRB ordering is typical
    FastLED.setBrightness(50);
    FastLED.clear();  
    FastLED.show(); 
  }
}

void WP(int pin, int v) //Write Pin
{
    pinMode(pin, OUTPUT); if(v) digitalWrite(pin, 1); else digitalWrite(pin, 0); 
}

int count=0, rr=10, gg=200, bb=130;

// void incl(){rr++; gg++; bb++; if(rr>222)rr=0;if(gg>222)gg=0;if(bb>222)bb=0;}
int rnp = 1;

void loop() { 
  if (net)
  {
    ArduinoOTA.handle();
    // if(digitalRead(NET_PIN) == 0) 
    // {
      WP(LEDB_PIN, 0); //on
      delay(55); //333
      WP(LEDB_PIN, 1); //off    
      delay(155);

    // {
    //   delay(10);
    //   if(digitalRead(NET_PIN) == 1) ESP.reset(); // restart();
    // }
    // }
  }
  else
  {
    if(++count > 255)count = 0;        // counter меняется от 0 до 255 (тип данных byte)

    // if(digitalRead(NET_PIN) == 0) //to gnd - go to ota
    // {
    //     FastLED.clear(); FastLED.show(); delay(555);
    //     ESP.restart();
    // }

  /*
  //raduga -------------------
  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(count + i * 255 / LED_NUM);
  }
  FastLED.show();  delay(10);        // скорость движения радуги
  */

  //случ -------------------
  int rn = rand() % (LED_NUM ); //без LED_NUM - 1
  // leds[rnp] = CRGB::Black;  
  leds[rn].setHue(rand() % (255)); //leds[rn].setHue(count); //leds[rn].setHue(222);
  rnp = rn;
  FastLED.show();  delay(100);        // скорость движения радуги


  /*//огонек
  FastLED.clear();
  leds[count] = CRGB::Green;
  if (++count >= LED_NUM) count = 0;
  FastLED.show();
  delay(100);*/
  

//   for(int n=0; n<NUM_LEDS; n++)
//  {
//     leds[n] = CRGB(rr,gg,bb); incl();
//    //leds[n] = CRGB(222,0,0); //incl();
//     FastLED.show();  delay(10);
//  }
  
//  leds[0] = CRGB::White; FastLED.show(); delay(30);
//  leds[0] = CRGB::Black; FastLED.show(); delay(30);
  
 


  // if(digitalRead(NET_PIN) == 1)WP(2,0);  else WP(2,1);  //net = true; else net = false;

  // WP(2,1); 
  // delay(55); //333
  // WP(2,0);
  // if(net)delay(55); else delay(555);

  }
}
