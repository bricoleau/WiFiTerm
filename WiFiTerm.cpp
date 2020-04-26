#include "WiFiTerm.h"
#include "WiFiTerm_webfiles.h"

WiFiTerm term;

WiFiTerm::WiFiTerm() :
  txBuf(WIFITERM_TX_BUF_SIZE),
  rxBuf(WIFITERM_RX_BUF_SIZE),
  webSocket(WIFITERM_WEBSOCKET_PORT)
{
  setTimeout(0);
  unlink();
}

void WiFiTerm::begin(WebServer &server)
{
  WiFiTerm_webfiles.begin(server);
  webSocket.begin();
  webSocket.onEvent(WiFiTerm::webSocketEvent);
}

void WiFiTerm::handleClient()
{
  send();
  webSocket.loop();
}

void WiFiTerm::setAsDefaultWhenUrlNotFound()
{
  if (WiFiTerm_webfiles.server != NULL)
  {
    WiFiTerm_webfiles.server->onNotFound([]()
    {
      static const char location_P[] PROGMEM = "Location";
      static const char text_plane_P[] PROGMEM = "text/plane";
      char arg1[20];
      char arg2[20];

      strcpy_P(arg1, location_P);
      strcpy_P(arg2, webfiles_term_html_path_P);
      WiFiTerm_webfiles.server->sendHeader((const char*)arg1, (const char*)arg2, true);

      strcpy_P(arg1, text_plane_P);
      arg2[0] = '\0';
      WiFiTerm_webfiles.server->send(302, (const char*)arg1, (const char*)arg2); 
    });
  }
}

void WiFiTerm::activateArduinoFavicon()
{
  WiFiTerm_webfiles.activateFavicon();
}

size_t WiFiTerm::write(uint8_t character)
{
  if (txBuf.isFull()) send();
  txBuf.write(character);
  if (linked != NULL) linked->write(character);
  return 1;
}

void WiFiTerm::send()
{
  if (txBuf.isNotEmpty())
  {
    if (webSocket.connectedClients())
    {
      size_t nb = txBuf.available();
      char content[nb];
      size_t i = 0;
      while (txBuf.available()) content[i++] = txBuf.read(); 
      webSocket.broadcastTXT(content, nb);
    }
    else
    {
      txBuf.flush();
    }
  }
}

void WiFiTerm::sendPrevious(uint8_t num)
{
  size_t nb = txBuf.previousAvailable();
  if (nb)
  {
    char content[nb];
    txBuf.peekPrevious(content);
    webSocket.sendTXT(num, content, nb);
  }
}

void WiFiTerm::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch(type)
  {
    case WStype_CONNECTED:
      term.sendPrevious(num);
      break;
    case WStype_TEXT:
      if (length == 0) //This is supposed to be a ping
      {
        term.webSocket.sendBIN(num, payload, 0); //pong
      }
      else
      {
        while (length--) term.rxBuf.write(*payload++);
      }
    default:;
  }
}
