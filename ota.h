/*
OTA:
ESP-25F0BF	192.168.1.104	BC:DD:C2:25:F0:BF	

Проблема - не появлялся сетевой порт:
В последнем обновлении Windows 10 сбрасывается настройка "Mobile hot spot" (Мобильный хот спот).
включить "Разрешить использование моего интернет-соединения на других устройствах"
! Энергосбережение - отключается если включено
но пинг есть

для VSCode  в arduino.json поправить на IP: ,"port": "192.168.1.104" 
прошивается ок.

при этом
[Starting] Opening the serial port - 192.168.1.104
[Error]Error: Opening 192.168.1.104: File not found

*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#define sp Serial.print

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif
const char* ssid = STASSID;
const char* password = STAPSK;

//#define FPM_SLEEP_MAX_TIME 0xFFFFFFF
//void WiFiOn() {
//  wifi_fpm_do_wakeup();
//  wifi_fpm_close();
//  //Serial.println("Reconnecting");
//  wifi_set_opmode(STATION_MODE);
//  wifi_station_connect();
//  sp(" WiFi_ON ");
//}

char Conn()
{
  long rssi;// = WiFi.RSSI();  Serial.print(" RSSI:");  Serial.println(rssi);
  
    unsigned char c=0;
    while (WiFi.status() != WL_CONNECTED)
    {
//        LEDON
        sp(" "); sp((int)c); //        Serial.print(".");
        //delay(10);// LEDOFF
        if(++c > 30)
        {
          Serial.println("Failed: ");
//          ESP.deepSleep(60e6, WAKE_RF_DEFAULT); 
          return 0;
        }
        delay(1000);
    }
    sp(" connected "); Serial.print(WiFi.localIP());
    rssi = WiFi.RSSI();  Serial.print(" RSSI:");  Serial.println(rssi);
    return 1;
}

void otaSetup() {
    Serial.print("Connection...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
//    WiFiOn(); 
    if(!Conn())
    {
    //while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.print("Failed! Rebooting...");
      delay(2000);
      ESP.restart();
    }
    Serial.print("ok ");
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    //ArduinoOTA.setHostname("esp_uno");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_FS
        type = "filesystem";
      }
      // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
