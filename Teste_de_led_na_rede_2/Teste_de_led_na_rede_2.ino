#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

byte ip[] = {172, 22, 13, 214};

EthernetServer server(80);

String readString;
int pin = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  EthernetClient client = server.available();
  if(client){
    while(client.connected()){
      if(client.available()){
        char c = client.read();

        if(readString.length() < 100){
          readString += c;
        }
        if(c == '\n'){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          //codigo html
          client.println("");
          client.println("");
          client.println("");
          client.println("Ethernet");
          client.println("");
          client.println("");

          client.println("Acender led");
          client.println("Apagar led");

          client.println("");
          client.println("");

          delay(1);
          client.stop();

          if(readString.indexOf("facacomarduino/LedOn") >0){
            digitalWrite(pin, HIGH); //liga o led
          }
          else{
            if(readString.indexOf("facacomarduino/LedOff") > 0){
              digitalWrite(pin, LOW); //desliga o led
            }
          }
          readString="";
        }
       }
     }
   }
}
