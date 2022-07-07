#include <FirebaseESP32.h>
#include <FB_Const.h>
#include <Firebase.h>
#include <FB_Utils.h>
#include <FB_Net.h>
#include <FB_Error.h>

#include <FirebaseFS.h>

#include <WiFi.h>

#define WIFI_SSID     "SUES" // your wifi SSID
#define WIFI_PASSWORD "TripleUS2637" //your wifi PASSWORD

#define LedPin 4         // pin d4 as toggle pin

#define FIREBASE_HOST "esp32-wifi-monitor-default-rtdb.firebaseio.com" // change here
#define FIREBASE_AUTH "JEScAl2BJFIo5goTturXfL76y0eaCJiXBbx1wRgr"  // your private key
FirebaseData firebaseData;



#define RH(format, ...)  Serial.printf("RH:"format, ##__VA_ARGS__)

void setup ()
{
  pinMode(LedPin, OUTPUT);
  digitalWrite(LedPin, LOW);
  
  Serial.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ") ;
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);     

  vTaskDelay(1000);
}
void loop ()
{
  if(Firebase.getString(firebaseData, "/led1"))
  {
    String ledstatus = firebaseData.stringData();
    if(ledstatus.toInt() == 1){
      digitalWrite(LedPin, HIGH);
      Serial.println("on");
    }
    else {
      digitalWrite(LedPin, LOW);
      Serial.println("off");
      
    }
  }else{
    Serial.print("Error in getInt, ");
    Serial.println(firebaseData.errorReason());
  } 
}
