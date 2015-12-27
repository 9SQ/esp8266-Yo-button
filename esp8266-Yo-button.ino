#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define LED 5
#define SW 12

const char* ssid = "**********";
const char* password = "**********";

const char* api_token = "**********"; // Yo API key
const char* username = "**********"; // Destination username

volatile boolean send_flag = false;

WiFiClient client;

//*********************
//* LED Blink
//*********************
boolean led_flag = false;
int led_blink_count = 0;
int led_blink_count_end = 0;
os_timer_t blinkTimer;

void led_change_flag(void *pArg) {
  if (led_flag) {
    led_flag = false;
  }
  else {
    led_flag = true;
  }
  led_blink_count++;
  if (led_blink_count >= led_blink_count_end) {
    led_flag = false;
    os_timer_disarm(&blinkTimer);
  }
}

void ledBlink (int count, int ms) {
  led_blink_count = 0;
  led_blink_count_end = count * 2;
  os_timer_setfn(&blinkTimer, led_change_flag, NULL);
  os_timer_arm(&blinkTimer, ms, true);
}

//*********************
//* HTTP POST
//*********************
void httpPost(String host, String url, String data, char* result) {
  if (client.connect(host.c_str(), 80)) {
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: " + host);
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    client.setTimeout(200);
    delay(100);
    boolean body = false;
    int lfcount = 0;
    while (client.available()) {
      char c =  client.read();
      if (body) {
        *result = c;
        result++;
      }
      if (c == '\r' || c == '\n') {
        lfcount++;
      }
      else {
        lfcount = 0;
      }
      if (lfcount > 3) {
        body = true;
      }
      *result = '\0';
    }
  }
}

//*********************
//* Wi-Fi Connection
//*********************
boolean checkConnection(int howmany) {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while ( count < howmany ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

//*********************
//* setup
//*********************
void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);
  pinMode(LED, OUTPUT);
  if (checkConnection(30)) {
    digitalWrite(LED, LOW);
    pinMode(SW, INPUT_PULLUP);
  }
  else {
    Serial.println("Wi-Fi Access Point not found.");
  }
}

//*********************
//* loop
//*********************
void loop() {
  if (!digitalRead(SW)) {
    send_flag = true;
    Serial.println("PUSH!");
  }

  if (send_flag) {
    String param = "api_token=";
    param += api_token;
    param += "&username=";
    param += username;

    char json[700];
    httpPost("api.justyo.co", "/yo/", param, json);

    StaticJsonBuffer<700> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);

    if (!root.success()) {
      ledBlink(2, 300);
      Serial.println("ERROR!");
    }
    else {
      String r = root["success"];
      if (r.equals("true")) {
        ledBlink(3, 150);
        Serial.println("SUCCESS!");
      }
      else {
        ledBlink(2, 300);
        Serial.println("ERROR!");
      }
    }
    send_flag = false;
  }

  // LED Blink
  if (led_flag) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  yield();
}
