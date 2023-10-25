#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handle_root(){
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

void handle_wifi_form(){
  String page_code = "<div style=\"padding: 50px;\">";
  page_code+= "<form action=\"LED\" method=\"POST\">";
  page_code+= "<div style=\"text-align: right; width: 50%; \">Network name <input type=\"text\" name=\"network\"></div><br>";
  page_code+= "<div style=\"text-align: right; width: 50%;\">Password <input type=\"text\" name=\"pass\"></div><br>";
  page_code+= "<div style=\"text-align: right; width: 50%;\"><input type=\"submit\" value=\"Send\"></div></form>";
  server.send(200, "text/html", page_code);

}

void handle_wifi() {
  if (server.hasArg("network") && server.hasArg("pass")) {
    String user_ssid = server.arg("network");
    String user_password = server.arg("pass");

    WiFi.begin(user_ssid.c_str(), user_password.c_str());
    server.send(200, "text/html", "Wi-Fi setup complete");
    delay(1000);
    ESP.restart();
  } else {
    server.send(200, "text/html", "Failed");
  }
}

void handle_led(){
  digitalWrite(led_pin, !digitalRead(led_pin));
  server.sendHeader("Location", "/");
  server.send(303);
}

void handle_none(){
  server.send(404, "text/html", "404 Not Found");
}

void server_init(){
  server.on("/", HTTP_GET, handle_root);
  server.on("/LED", HTTP_POST, handle_led);
  server.on("/wifi_form", HTTP_GET, handle_wifi_form);
  server.on("/wifi", HTTP_POST, handle_wifi);
  server.onNotFound(handle_none);

  server.begin();
  Serial.println("HTTP Server is on. On port " + String(web_port));
}
