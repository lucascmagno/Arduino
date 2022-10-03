//ADAPTADO SALA PROJETO CENTRAIS DE AR DIVTEL
//Autores: Anderson Moraes, Karina Silva

#include <EEPROM.h>
#include <UIPEthernet.h>
#include "IRremote.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS A3  // DS18B20 pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float temp;

#define UM 1710
#define ZERO 560
#define PinoC1 5
#define PinoC2 6
#define PinoC3 7


IRsend irsend;

byte mac[] = {0x54, 0x55, 0x4f, 0x11, 0x0f, 0x2b};
IPAddress ip(192,168,1,6);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

EthernetServer server(80);

String lerStr;
char* sub;
char central=0;  
int code = 0;
int x=0;
int TmpCentral[]={4,4,4};
int pTmpCentral[]={4,4,4};

//String s1;
unsigned long startTime;
boolean led1Status;
 
boolean StsCentral[]= {false,false,false};
boolean pStsCentral[]= {false,false,false};
boolean backhome;
unsigned int irCMD[100]={
                         4314, 4464,                                                                              // 0-1
                          488, 1710, 488,  560, 488, 1710, 488, 1710, 488,  560, 488,  560, 488, 1710, 488,  560, // 2-17
                          488,  560, 488, 1710, 488,  560, 488,  560, 488, 1710, 488, 1710, 488,  560, 488, 1710, // 18-33
                          488, 1710, 488,  560, 488, 1710, 488, 1710, 488, 1710, 488, 1710, 488, 1710, 488, 1710, // 34-49
                          488,  560, 488, 1710, 488,  560, 488,  560, 488,  560, 488,  560, 488,  560, 488,  560, // 50-65
                          488,  560, 488,  560, 488, 1710, 488,  560, 488,  560, 488,  560, 488,  560, 488,  560, // 66-81
                          488, 1710, 488, 1710, 488,  560, 488, 1710, 488, 1710, 488, 1710, 488, 1710, 488, 1710, // 82-97
                          488, 5190};                                                                             // 98-99
                                     
int     vtrPOS[]={35,37,45,67,69,71,73};  //posições de alteração
boolean vtrTMP[7][7]= {
                        {0, 1, 0, 1, 1, 1, 0},// Desliga
                        {0, 0, 1, 0, 0, 0, 0},// 17 1
                        {0, 0, 1, 0, 0, 0, 1},// 18 2
                        {0, 0, 1, 0, 0, 1, 1},// 19 3
                        {1, 0, 1, 0, 0, 1, 0},// 20 4
                        {1, 0, 1, 0, 1, 1, 0},// 21 5
                        {1, 0, 1, 0, 1, 1, 1} // 22 6
                      };
void setup(){
  Serial.begin(9600);  
  StsCentral[0]=EEPROM.read(0);
  StsCentral[1]=EEPROM.read(1);
  StsCentral[2]=EEPROM.read(2);
  TmpCentral[0]=EEPROM.read(3);
  TmpCentral[1]=EEPROM.read(4);
  TmpCentral[2]=EEPROM.read(5);
    
  Ethernet.begin(mac, ip, dns, gateway, subnet);
//  server.begin();  
  pinMode(PinoC1, OUTPUT);
  pinMode(PinoC2, OUTPUT);
  pinMode(PinoC3, OUTPUT);
  digitalWrite(PinoC1, HIGH);
  digitalWrite(PinoC2, HIGH);
  digitalWrite(PinoC3, HIGH);
  pinMode(4, OUTPUT);
//  Serial.println("*");
  startTime = millis();
}
 
void loop(){
  int index = 0,central=0, code=0;
  EthernetClient client = server.available();
  if (client) {    
   // Serial.print('.');
    while (client.connected()) {       
    //  Serial.print('#');
      if (client.available()) {             
        char c = client.read();                     
        if (lerStr.length() < 100) lerStr += c; //store characters to string           
        
        if (c == '\n') {
          //Serial.println(lerStr);
          index = lerStr.indexOf("/C");                   
          if(index>0){
       //     Serial.println(lerStr);
            central = lerStr.substring(index+2,index+3).toInt();  
            code = lerStr.substring(index+4,index+5).toInt();              
            switch (central){
              case 1:                
                enviaCMD(PinoC1,code); // code pode ser 0-desliga 1-liga+ 2-liga- 3-last
         //       Serial.println('1');                    
              break;
              case 2:
                enviaCMD(PinoC2,code);  
         //       Serial.println('2');                    
              break;
              case 3:
                enviaCMD(PinoC3,code);        
         //       Serial.println('3');                    
              break;
              default:
          //      Serial.println('4');                    
                backhome = false;              
            }                        
          }                  
          lerStr="";
                      client.println(F("<!DOCTYPE HTML>"));
                      client.print(F("<html><head><title>AR-SETEC</title>"));
                      client.print(F("</head><body><br><h2><center><b>SETEC-CRBE (Sala 32) - Temperatura: "));                      
                      client.print(temp);
                      client.print(F("&deg;C</b></center></h2><br>"));
          if(backhome)client.println(F("<meta http-equiv=\"refresh\" content=\"2;URL=/\">"));
          //else        client.println(F("<meta http-equiv=\"refresh\" content=\"10;URL=/\">"));
                      client.println(F("<table align=\"center\" width=\"40%\" border='4'><tr><td>CENTRAIS</td><td align=\"center\">STATUS</td><td align=\"center\">AUMENTAR</td><td align=\"center\">DIMINUIR</td><td align=\"center\">AJUSTE</td></tr>"));                      
                      client.print(F("<tr><td height=\"50%\">1-Entrada</td><td align=\"center\" bgcolor=\""));                      
     if(StsCentral[0])client.print(F("#00FF00\"><a href='C1T0'>ON"));
                 else client.print(F("#FF0000\"><a href='C1T3'>OFF"));                                                    
                      client.print(F("</a></td><td align=\"center\"><a href='C1T1'>[+UP+]</a></td><td align=\"center\"><a href='C1T2'>[-DW-]</a><br></td><td align=\"center\">"));
                      client.print(TmpCentral[0]+16);                      
                      client.print(F(" &deg;C</td></tr><tr><td>2-Reserva</td><td align=\"center\" bgcolor=\""));
     if(StsCentral[1])client.print(F("#00FF00\"><a href='C2T0'>ON"));
                 else client.print(F("#FF0000\"><a href='C2T3'>OFF"));                                                    
                      client.print(F("</a></td><td align=\"center\"><a href='C2T1'>[+UP+]</a></td><td align=\"center\"><a href='C2T2'>[-DW-]</a><br></td><td align=\"center\">"));
                      client.print(TmpCentral[1]+16);
                      client.print(F(" &deg;C</td></tr><tr><td>3-RACKS</td><td align=\"center\" bgcolor=\""));
     if(StsCentral[2])client.print(F("#00FF00\"><a href='C3T0'>ON"));
                 else client.print(F("#FF0000\"><a href='C3T3'>OFF"));                                                    
                      client.print(F("</a></td><td align=\"center\"><a href='C3T1'>[+UP+]</a></td><td align=\"center\"><a href='C3T2'>[-DW-]</a></td><td align=\"center\">"));
                      client.print(TmpCentral[2]+16);
                      client.println(F(" &deg;C</td></tr></table><center><br>OBS: Monitorar temperatura para confirmar recebimento dos comandos.</center>"));          
                      client.println(F("</body>"));
                      client.println(F("</html>"));   
                      delay(2);
                      client.stop();
                      backhome = false;
        }
        pStsCentral[0] = StsCentral[0];
        pStsCentral[1] = StsCentral[1];                     
        pStsCentral[2] = StsCentral[2];
        pTmpCentral[0] = TmpCentral[0];
        pTmpCentral[1] = TmpCentral[1];
        pTmpCentral[2] = TmpCentral[2];
      }
    }//fim do while cliente conectado    
  }  

  if(millis() - startTime > 10000){      
      led1Status = !led1Status;
      digitalWrite(4,led1Status);           
      do {
        DS18B20.requestTemperatures();
        temp = DS18B20.getTempCByIndex(0);
      } while (temp == 85.0 || temp == (-127.0));      
   //   s1 = temp;   
 //     Serial.print(s1);     
 //     Serial.print(' ');           
//      Serial.print(StsCentral[0]);        
//      Serial.print(StsCentral[1]);
 //     Serial.println(StsCentral[2]);
      startTime = millis();     
  }

 
}
//==================================================================================================
void buildCMD(int tmp){
  for(int n=0;n<=6;n++){
    if(vtrTMP[tmp][n]){
      irCMD[vtrPOS[n]]=UM;
      irCMD[vtrPOS[n]+16]=ZERO;                        
    }else{
      irCMD[vtrPOS[n]]=ZERO;
      irCMD[vtrPOS[n]+16]=UM;                            
    }
  }  
}
 
void enviaCMD(int Pino, int cod){ // cod 0-desliga 1-liga+ 2-liga- 3-last   
  digitalWrite(Pino,LOW);
  switch (cod){    
    case 0://Desligar
        buildCMD(0);
        StsCentral[Pino-5]= false;
    break;
    case 1://Aumentar Temperatura
        TmpCentral[Pino-5]++;
        if(TmpCentral[Pino-5]>6)TmpCentral[Pino-5]=6;
        if(StsCentral[Pino-5]){
          StsCentral[Pino-5]= true;
          buildCMD(TmpCentral[Pino-5]);
        }
    break;
    case 2://Diminuir Temperatura
        TmpCentral[Pino-5]--;
        if(TmpCentral[Pino-5]<1)TmpCentral[Pino-5]=1;
        if(StsCentral[Pino-5]){
          StsCentral[Pino-5]= true;
          buildCMD(TmpCentral[Pino-5]);
        }
    break;
    case 3://Ligar
        buildCMD(TmpCentral[Pino-5]);
        StsCentral[Pino-5]= true;
    break;    
  }  
  irsend.sendRaw(irCMD, sizeof(irCMD) / sizeof(irCMD[0]), 38);  
  irsend.sendRaw(irCMD, sizeof(irCMD) / sizeof(irCMD[0]), 38);  
  digitalWrite(Pino,HIGH);
  if(StsCentral[0] != pStsCentral[0])EEPROM.write(0, StsCentral[0]);
  if(StsCentral[1] != pStsCentral[1])EEPROM.write(1, StsCentral[1]);
  if(StsCentral[2] != pStsCentral[2])EEPROM.write(2, StsCentral[2]);  
  if(TmpCentral[0] != pTmpCentral[0])EEPROM.write(3, TmpCentral[0]);
  if(TmpCentral[1] != pTmpCentral[1])EEPROM.write(4, TmpCentral[1]);
  if(TmpCentral[2] != pTmpCentral[2])EEPROM.write(5, TmpCentral[2]);
 
  backhome = true;
}
