#include <M5Unified.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// ref: https://sample.msr-r.net/m5stack-line-notify/
//fja;lfjasdgjeoaasdkjfasdjsdfj;//

const char* ssid     = "ifdl"; // WiFi SSID
const char* password = "hogeupip5"; // WiFi Password

// deployed on GoogleAppsScript
const char* gas_api  = "https://script.google.com/macros/s/AKfycbyXkDVUDI2yw_BcLHasZrMRZl_6FCq5LhpbIS-VqetE7o2YOaa0HWW1hkoeIW7TvOJ5ew/exec";

// JSON
// https://qiita.com/poruruba/items/4bf6a52520e431a8f4a5
// ArduinoJson to serialize to String
// https://github.com/bblanchon/ArduinoJson/issues/869

StaticJsonDocument<200> json_doc;

boolean GAS_post(String msg) {
  WiFiClientSecure client;
  client.setInsecure();
  if(!client.connect("script.google.com", 443)) {
    M5.Display.printf("connect error!\n");
    return false;
  }
  String json_request;
  json_doc["message"] = msg;

  serializeJson(json_doc, json_request);
  // M5.Display.printf("JSON string: %s\n", json_request.c_str()); // for debug

  String request = String("")
              + "POST " + String(gas_api) + " HTTP/1.1\r\n"
              + "Host: script.google.com\r\n"
              + "Content-type: application/json\r\n"
              + "Content-Length: " + String(json_request.length()) + "\r\n"
              + "Connection: close\r\n\r\n"
              + String(json_request) + "\r\n";
  printf("request: %s\n", request.c_str());
  client.print(request);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    printf("%s\n", line.c_str());
  }
  client.stop();

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
    M5.Display.printf("Button A Pressed\n");
    printf("Button A Pressed\n");
    String msg = "Button A Pressed";
    GAS_post(msg);
  }
}
