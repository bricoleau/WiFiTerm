//Gateway Serial <-> WiFiTerm
//with embedded baudrate selector

#include <ESP8266WiFi.h>
const char* ssid = "";
const char* password = "";

#include "WiFiTerm.h"

ESP8266WebServer server(80);

const uint32_t baud_rates[] = {
  300,
  1200,
  2400,
  4800,
  9600,
  19200,
  38400,
  57600,
  74880,
  115200,
  230400
};
const uint8_t nb_baud_rates = sizeof(baud_rates) / sizeof(uint32_t);

uint8_t  baud_rate_selection = 0;
uint32_t baud_rate = 0;
uint8_t timeout_ms = 1;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
  }

  server.begin();
  term.begin(server);

  while (baud_rate_selection == 0)
  {
    term.println("Please select Serial baud rate :");
    for (size_t i=0; i<nb_baud_rates; i++)
    {
      term.print(i+1);
      term.print(". ");
      term.println(baud_rates[i]);
    }
    term.print('>');

    while (!term.available())
    {
      server.handleClient();
      term.handleClient();
      delay(100);
    } 

    int i = term.parseInt();
    term.flush();
    term.println(i);
    if (1 <= i && i <= nb_baud_rates)
    {
      baud_rate_selection = i;
      baud_rate = baud_rates[baud_rate_selection - 1];
      timeout_ms = 1 + 10000 / baud_rate;
    }
    else
    {
      term.println("Invalid selection\n");
    }
  }

  Serial.begin(baud_rate);
  term.print("Gateway started at ");
  term.print(baud_rate);
  term.println(" bps");
  term.println();
}

void loop()
{
  server.handleClient();
  term.handleClient();

  //term to Serial
  if (term.available())
  {
    Serial.println(term.readString());
  }

  //Serial to term
  size_t qty = WIFITERM_TX_BUF_SIZE;
  while (Serial.available() && qty--)
  {
    char c = Serial.read();
    term.print(c);
    if (!Serial.available())
    {
      delay(timeout_ms); //wait for next char
    }
  }
}
