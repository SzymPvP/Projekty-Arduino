#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature DS18B20(&oneWire);
char temperatureCString[6];

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  DS18B20.begin();
  
  Serial.println();
  Serial.print("Laczenie z WiFi");
  
  WiFi.begin("rdi_2120", "gcHFk9iA");
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Polaczono z WiFi");
  
  server.begin();
  Serial.println("Serwer stoi, czekamy na IP...");
  delay(10000);
  
  Serial.println(WiFi.localIP());
}

void getTemperature() {
  float tempC;
  do {
    DS18B20.requestTemperatures(); 
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 2, 2, temperatureCString);
    delay(100);
  } while(tempC == 85.0);
}

void loop() {
  WiFiClient client = server.available();
  
  if(client) {
    Serial.println("Polaczono z klientem!");
    boolean blank_line = true;
    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        if(c == '\n' && blank_line) {
            getTemperature();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head>");
            client.println("    <title>ESP8266 - Inteligentny dom</title>");
            client.println("    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">");
            client.println("</head>");
            client.println("<body>");
            client.println("    <center>");
            client.println("        <h1>ESP8266 - Temperatura w domu</h1>");
            client.print("        <h3>Temperatura: ");
            client.print(temperatureCString);
            client.println("&#176;C</h3>");
            client.println("    </center>");
            client.println("</body>");
            client.println("</html>");
            break;
        }
        if(c == '\n') {
          blank_line = true;
        }
        else if(c != '\r') {
          blank_line = false;
        }
      }
    }
    delay(5);
    client.stop();
    Serial.println("Rozlaczono z klientem.");
  }
}
