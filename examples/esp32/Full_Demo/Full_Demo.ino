//WiFiTerm full demo
//
//Please open Serial terminal and follow instructions
//
#include <WebServer.h>
const char* ssid = "";     //adjust to your own values
const char* password = ""; //adjust to your own values

#include "WiFiTerm.h"

WebServer server(80);

void setup()
{
  Serial.begin(115200);
  term.link(Serial); //optional : echo every print() on Serial

  term.println("Welcome to WiFiTerm full demo example");

  term.print("\nConnecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    term.print('.');
  }
  term.println("connected");

  server.begin();
  term.begin(server);

  term.setAsDefaultWhenUrlNotFound(); //optional : redirect any unknown url to /term.html
  term.activateArduinoFavicon(); //optional : send Arduino icon when a browser asks for favicon

  term.println("WiFiTerm started");
  term.println();

  Serial.print("I'm waiting for you at http://");
  Serial.print(WiFi.localIP());
  Serial.println("/term.html");

  term.unlink(); //optional : stop echo on Serial
  term.println("Welcome here");
  term.println("Please note that previous lines were printed BEFORE WiFiTerm connection");
  term.println("Now send me some text...");
}

void loop()
{
  server.handleClient();
  term.handleClient();

  static int step = 0;
  static long unsigned int timer, speed, nb;

  if (step == 0)
  {
    if (term.available())
    {
      term.print("Ok I received : ");
      while (term.available())         //standard way
      {
        char c = term.read();
        term.print(c);
      }
      term.println();
      step++;
    }
  }
  else if (step == 4)
  {
    if (term.available())
    {
      term.print("Ok I received : ");
      term.println(term.readString()); //shortest way
      step++;
    }
  }
  else if (step == 2)
  {
    term.println("\nNow you can test window resizing");
    term.println("Send me a number to continue...");
    step++;
  }
  else if (step == 3)
  {
    if (term.available())
    {
      int i = term.parseInt(); //coolest way to get an int
      term.print("Ok I received ");
      term.println(i);
      
      term.println("\nNow I will freeze for 15 seconds");
      term.println("The above button turns red while offline");
      term.handleClient();
      delay(15000); //Don't use big delays in your programs !
      term.println("Ok I'm back");
      term.println("Please send me something to continue...");
      step++;
    }
  }
  else if (step == 1)
  {
    term.println();
    term.println("Let's try to print a very long text :");
    term.println();
    term.println(F("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur interdum arcu eget convallis imperdiet. "
    "Nam eleifend pellentesque dui nec viverra. Maecenas a faucibus mi, nec tempus orci. Aenean id ligula luctus, "
    "mollis libero non, semper urna. Donec viverra enim sed turpis vestibulum, ac fermentum erat porta. Nam ac rhoncus "
    "risus. Quisque eu ornare enim. Duis et arcu vel turpis molestie sagittis eu consectetur metus.\n\n"
    "Nam tempus finibus neque, sed ornare ante porta ut. Nam iaculis, augue vel egestas auctor, elit tellus placerat tellus, "
    "sit amet dignissim nunc metus nec magna. In id facilisis augue, sed ornare sapien. Ut est quam, tincidunt aliquam varius nec, "
    "tempor in risus. Nam vehicula ac quam eget dictum. Nulla euismod, mi non bibendum vehicula, nibh metus sagittis nulla, nec "
    "ullamcorper nunc felis sed eros. Quisque quis bibendum turpis.\n\n"
    "Phasellus vestibulum diam ante, non feugiat augue fermentum eu. Nam ullamcorper condimentum ultricies. Duis placerat eget mauris "
    "ut faucibus. Nam vitae sapien quis sapien ullamcorper condimentum nec ut dolor. Nunc aliquet pellentesque purus, tristique eleifend "
    "lectus sodales sit amet. Nam nec viverra metus. Proin id pretium lectus, id mattis enim. Cras ultricies iaculis hendrerit. "
    "Duis fringilla cursus ipsum non ullamcorper. Aliquam maximus ante eu iaculis vestibulum. Curabitur in enim non est viverra lacinia.\n\n"
    "Proin ornare massa eu ex scelerisque, id ultricies ipsum interdum. Duis congue, odio nec euismod feugiat, orci est lacinia arcu, "
    "vitae placerat orci ante sed dolor. Sed ac vestibulum orci. Nulla at efficitur lacus, a tincidunt diam. Morbi interdum felis sapien, "
    "ac mollis erat semper a. Suspendisse potenti. Pellentesque bibendum efficitur ante. Nullam elit diam, ultricies sed condimentum nec, "
    "accumsan imperdiet nisl. Praesent non nunc quis mi lobortis mollis. Duis convallis justo vitae sagittis gravida.\n\n"
    "Fusce ligula massa, ornare eu feugiat quis, ornare volutpat neque. Vivamus lacus nisi, ultricies vel imperdiet a, "
    "interdum semper est. Etiam aliquam fermentum mauris ut vehicula. Morbi congue tempor finibus. Maecenas laoreet vitae dui non interdum. "
    "Aliquam a cursus nisl, vel eleifend mi. Sed sit amet hendrerit dolor. Aenean cursus volutpat mi eu vestibulum. Praesent ultrices, "
    "elit ac tincidunt fringilla, felis erat ullamcorper urna, id sagittis felis nibh non libero. Mauris rhoncus erat sed sollicitudin congue. "
    "Mauris mollis quam a magna mattis, quis tempus nulla eleifend. Cras volutpat tempus est, at faucibus lorem hendrerit sed. Phasellus et lobortis ex, "
    "non sollicitudin elit. Curabitur ornare mauris tellus, at lobortis velit luctus non."));
    step++;
  }
  else if (step == 5 || step == 7 || step == 9)
  {
    speed = (step == 5) ? 1000 : (step == 7 ? 500 : 100);
    term.printf("\nSpeed test %lums", speed); //By the way : printf is working fine !
    term.println();
    timer = millis();
    step++;
    nb = 0;
  }
  else if (step == 6 || step == 8 || step == 10)
  {
    if (nb * speed < 10000UL)
    {
      if (millis() - timer >= speed)
      {
        term.println(++nb);
        timer += speed;
      }
    }
    else
    {
      step++;
    }
  }
  else if (step == 11)
  {
    term.println();
    term.println("Did you notice?");
    term.println("Automatic scroll can be disabled/enabled by an action on the scrollbar");
    term.println();
    if (term.connectedClients() < 2)
    {
      term.println("Now reset your ESP and test it again using multiple terms simultaneously");
      term.println("WiFiTerm can handle up to 5 clients");
      term.println("Test with your computer, smartphone, tablet, etc.");
    }
    else
    {
      term.println("End of demo\nI hope you enjoyed");
    }
    step++;
  }
}
