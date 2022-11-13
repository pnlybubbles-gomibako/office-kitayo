#include <M5Atom.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "constant.h"

const int32_t http_timeout = 10 * 1000;

const int32_t red = 0x400000;
const int32_t green = 0x004000;
const int32_t blue = 0x000040;
const int32_t white = 0x202020;

void wifiSetup() {
  WiFi.disconnect();
  Serial.printf("SSID: %s\n", WIFI_SSID);
  Serial.print("WiFi connecting");
  M5.dis.drawpix(0, red);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (i % 10 == 0) {
      WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    i++;
    Serial.print(".");
    delay(500);
  }

  Serial.println("OK");
  M5.dis.drawpix(0, white);
}

void send(String url) {
  if (WiFi.status() != WL_CONNECTED) {
    wifiSetup();
  }

  Serial.print("http requesting...");

  HTTPClient http;
  http.begin(url);
  http.setReuse(false);
  http.setConnectTimeout(http_timeout);
  http.setTimeout(http_timeout);
  int status_code = http.POST("");
  Serial.printf("status_code=%d\r\n", status_code);
  Serial.println(HTTPClient::errorToString(status_code));
  if (status_code != 200) {
    Serial.println("failed");
    M5.dis.drawpix(0, red);
  } else {
    Serial.println("success");
    M5.dis.drawpix(0, green);
  }
  http.end();

  delay(1000);
}

void setup() {
  M5.begin(true, false, true);
  delay(10);
  M5.dis.setBrightness(10);
  Serial.begin(115200);

  wifiSetup();
}

void loop() {
  if (M5.Btn.wasPressed()) {
    M5.dis.drawpix(0, blue);

    while (M5.Btn.isPressed()) {
      delay(100);
      M5.Btn.read();
    }

    send(REQUEST_URI);

    M5.dis.drawpix(0, white);
  }

  delay(100);
  M5.update();
}
