


#include <Wire.h>
#include "SparkFunHTU21D.h"

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//Create an instance of the object
HTU21D myHumidity;

void show_yes_no(const char *prefix, int val)
{
  Serial.print(prefix);
  if (val)
    Serial.println("yes");
  else
    Serial.println("no");
}

void dump_user_register()
{
  byte reg = myHumidity.readUserRegister();

  Serial.print("Resolution (Humidity, Temperature): ");
  switch (reg & USER_REGISTER_RESOLUTION_MASK) {
  case USER_REGISTER_RESOLUTION_RH12_TEMP14: Serial.print(12); Serial.print(", "); Serial.println(14); break;
  case USER_REGISTER_RESOLUTION_RH8_TEMP12: Serial.print(8); Serial.print(", "); Serial.println(12); break;
  case USER_REGISTER_RESOLUTION_RH10_TEMP13: Serial.print(10); Serial.print(", "); Serial.println(13); break;
  case USER_REGISTER_RESOLUTION_RH11_TEMP11: Serial.print(11); Serial.print(", "); Serial.println(11); break;
  }

  //show_yes_no("End of battery: ", reg & USER_REGISTER_END_OF_BATTERY);
  //show_yes_no("Heater enabled: ", reg & USER_REGISTER_HEATER_ENABLED);
  //show_yes_no("Disable OTP reload: ", reg & USER_REGISTER_DISABLE_OTP_RELOAD);
}

void setup()
{
  
  Serial.begin(9600);
  Serial.println("HTU21D Example!");

  WiFi.begin("YOURWIFISSID", "YOURWIFIPASSWORD");   //WiFi connection 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
 
  }


    Serial.println("Wifi is now trying Hotspot connection");
 
  }

  myHumidity.begin();
  
  dump_user_register();
}

void loop()
{
  float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)


  Serial.print("Time:");
  Serial.print(millis());
  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("%");

  Serial.println();

//wifi http post start here
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 
   HTTPClient http;    //Declare object of class HTTPClient
 
  String thingspeak ="http://api.thingspeak.com/update.json?api_key=WriteAPIKey&field1=";
  thingspeak +=  millis();
  thingspeak += "&field2=";
  thingspeak += temp;
  thingspeak += "&field3=";
  thingspeak += humd;
  thingspeak += "&field4=42";
  thingspeak += "&field5=";
  thingspeak += WiFi.localIP().toString();

  http.begin(thingspeak);      //Specify request destination
 Serial.println(thingspeak);   //Print HTTP return code
  
  
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
   int httpCode = http.POST(thingspeak);   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
 
    Serial.println("Error in WiFi connection");   
 //wifi http post finish here 
 
 }

    digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW

  delay(30000);

}
