#include "rdm6300.h"
#define RDM6300_RX_PIN 14
#define READ_LED_PIN 13

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "TechPublic"
#define WIFI_PASSWORD "WIFI_PASSWORD"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDyvT6mpL1iU4T8wIOz1T-UeDVDznw4RSo"

/* 3. Define the RTDB URL */
#define DATABASE_URL "dog-id-7b085-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "rdcrproject@gmail.com"
#define USER_PASSWORD "123dog123dog"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


Rdm6300 rdm6300;
char txString[16] = {0};
uint32_t currentId = 0 ;


void setup() {
  Serial.begin(115200);

  pinMode(READ_LED_PIN, OUTPUT);
  digitalWrite(READ_LED_PIN, LOW);

  rdm6300.begin(RDM6300_RX_PIN);

  Serial.println("\nPlace RFID tag near the rdm6300...");
    Serial.begin(115200);

  WiFi.begin(WIFI_SSID);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  //Or use legacy authenticate method
  //config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = "LlgPF7OoS9hPiLt1TdEwyf5HAWymJafZYg80Fdrg";

  //To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino


  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

void loop() {
  if (Firebase.ready() /*&& (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)*/&&rdm6300.update()){
       currentId = rdm6300.get_tag_id();
       Serial.println(rdm6300.get_tag_id(), HEX);
       dtostrf(currentId,6,0,txString);
       //sendDataPrevMillis = millis();
       Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/scanner1"),txString ) ? "ok" : fbdo.errorReason().c_str());
       
    }


  delay(10);

}
