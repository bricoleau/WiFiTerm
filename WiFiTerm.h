#ifndef WiFiTerm_h
#define WiFiTerm_h

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "WiFiTermBuffer.h"

#if defined(ESP32)
  #include <WebServer.h>
#elif defined(ESP8266)
  #include <ESP8266WebServer.h>
  #define WebServer ESP8266WebServer
#endif

#define WIFITERM_TX_BUF_SIZE 300
#define WIFITERM_RX_BUF_SIZE 100
#define WIFITERM_WEBSOCKET_PORT 81

class WiFiTerm : public Stream //with embedded Print class
{
  private :
    WiFiTermBuffer txBuf, rxBuf;
    WebSocketsServer webSocket;

  public :
    WiFiTerm();

    void begin(WebServer &server);
    void handleClient();

    //web options
    void setAsDefaultWhenUrlNotFound();
    void activateArduinoFavicon();
    inline int connectedClients() {return webSocket.connectedClients();}

    //required for Print class
    size_t write(uint8_t character);

    //required for Stream class
    inline int read() {return rxBuf.read();}
    inline int peek() {return rxBuf.peek();}
    inline int available() {return rxBuf.available();}
    inline void flush()    {rxBuf.flush();}

    inline void resetTx() {txBuf.reset();}
  private :
    void send();
    void sendPrevious(uint8_t num);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

  //Add-on : link to another Print Class 
  private :
    Print *linked;
  public :
    inline void link(Print& p) {linked = &p;}
    inline void unlink() {linked = NULL;}
};

extern WiFiTerm term;

#endif
