#include <SPI.h>
#include <Ethernet.h>

//endereçamento mac
byte mac[] = {0xDe, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

//endereçamento ip
IPAddress ip (172, 22, 13, 210);

//inicia o servidor na porta 80
EthernetServer server(80);

//pino digital output (saida)
int pinLed1 = 3;
int led = 0;
//Mensagem mostrado no browser
String msg = "LED DESLIGADO";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial){
    ;
  }
  //pinLed1 definido como saída
  pinMode(pinLed1, OUTPUT);
  
  //pinLed1 inicia desligado
  digitalWrite(pinLed1, LOW);
  
  //inicia a conexão com o endereçamento mac e ip
  Ethernet.begin(mac, ip);
  
  //inicia o servidor
  server.begin();
  Serial.print("Server is at");
  
  //Printa o ip no monitor serial
  Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  //Cliente disponivel
  EthernetClient client = server.available();
  if(client){//Se client for verdadeiro
    Serial.println("New client");

    boolean currentLineIsBlank = true;

    String DatEth;

    while(client.connected()){
      if(client.available()){

        char c = client.read();
        Serial.write(c);

        DatEth.concat(c);
        if(DatEth.endsWith("/ledon")){
          led = 1; 
          msg = "LED LIGADO";
        }
        if(DatEth.endsWith("/ledof")){
          led = 0;
          msg = "LED DESLIGADO";
        }
        if(c == '\n' && currentLineIsBlank){
          /*************************CODIGO HTML****************************/
          client.println("<!DOCTYPE html>");
          client.println("<html lang=\"pt-br\">");
          client.println("<head>");
          client.println("<title>Projeo led</title>");
          client.println("<meta http-equiv=\"refresh\" content= \"2\" charset=\"UTF-8\">");
          client.println("<style>*{text-align: center;}</style>");
          client.println("</head>");
          client.println("<body style=\"background-color: #DDD;\">");
          client.println("<br>");
          client.println("<form align=\"center\">");
          client.println("<label style=\"font-size: 18pt;\">"+msg+"</label>");
          client.println("<br>");
          client.println("<br>");
          client.println("<button type=\"submit\" formaction=\"ledon\" style=\"margin-right: 50px; padding: 10px;\">Ligar</button>");
          client.println("<button type=\"submit\" formaction=\"ledof\" style=\"padding: 10px;\">Desligar</button>");
          client.println("</form>");
          client.println("<br>");
          client.println("</body>");
          client.println("</html>");
          /*************************CODIGO HTML****************************/

          /***************LED*************/
          if(led == 1){ digitalWrite(pinLed1, HIGH);} //liga o led se o valor for 1
          if(led == 0){ digitalWrite(pinLed1, LOW);} //desliga o led se o valor for 0
          break;
        }

        if(c == '\n'){
          currentLineIsBlank = true;
        }
        else if(c != '\r'){
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);

    client.stop();
    Serial.println("Client disconnected");
  }
}
