#include <M5Unified.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ref: https://sample.msr-r.net/m5stack-line-notify/

const char* ssid     = "XXXXXXXX"; // WiFi SSID
const char* password = "XXXXXXXX"; // WiFi Password

// deployed on GoogleAppsScript
const char* gas_api  = "https://script.google.com/a/macros/ifdl.jp/s/AKf...yqg/exec";

// JSON
// https://qiita.com/poruruba/items/4bf6a52520e431a8f4a5
// ArduinoJson to serialize to String
// https://github.com/bblanchon/ArduinoJson/issues/869

StaticJsonDocument<200> json_doc;

boolean GAS_post(String msg) {
  WiFiClientSecure client;
  client.setInsecure();
  if(!client.connect(gas_api, 443)) {
    printf("connect error!\n");
    return false;
  }

  String json_request;
  json_doc["message"] = msg;

  serializeJson(json_doc, json_request);
  printf("JSON string: %s\n", json_request.c_str());

  String request = String("")
              + "POST /api/notify HTTP/1.1\r\n"
              + "Host: " + gas_api + "\r\n"
              + "Content-Length: " + String(sizeof(json_request)) +  "\r\n"
              + "Content-Type: application/x-www-form-urlencoded\r\n\r\n"
              + String(json_request) + "\r\n";
  client.print(request);
  return true;
}

void setup() {
  M5.begin();
  M5.Display.setTextSize(2);
  M5.Display.clear();

  M5.Display.printf("WiFi connecting %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Display.print(".");
  }
  M5.Lcd.printf("ready\n");
}

void loop() {
  M5.update();
  if(M5.BtnA.wasPressed()) {
    String msg = "Button A Pressed";
    GAS_post(msg);
  }
}
