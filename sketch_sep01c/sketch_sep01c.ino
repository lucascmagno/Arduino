int pinoLed = 13; //DEFINE O PINO EM QUE O LED ESTÁ CONECTADO
int status = 0; //VARIÁVEL QUE CONTROLA O STATUS (LIGADO / DESLIGADO)
 
void setup(){
  pinMode(pinoLed, OUTPUT); //DECLARA O PINO COMO SAÍDA
  digitalWrite(pinoLed, LOW); //LED INICIA DESLIGADO
  Serial.begin(9600); //INICIALIZA A SERIAL COM TAXA DE 9600 BAUDS
}    
void loop(){
  char c = Serial.read(); //VARIÁVEL RESPONSÁVEL POR RECEBER O CARACTER DIGITADO NA ENTRADA DE DADOS DO MONITOR SERIAL
 
  if (c == 'b'){ //SE CARACTER DIGITADO FOR IGUAL A "b", FAZ
    if (status == 0){ //SE VARIÁVEL FOR IGUAL A 0(DESLIGADO), FAZ
      digitalWrite(pinoLed, HIGH); //LIGA O LED
      status = 1; //VARIÁVEL RECEBE O VALOR 1 (LIGADO)
      Serial.println("LED LIGADO"); //IMPRIME O TEXTO NO MONITOR SERIAL
    }else{ //SENÃO, FAZ
      digitalWrite(pinoLed, LOW); //DESLIGA O LED
      status = 0; //VARIÁVEL RECEBE O VALOR 0 (DESLIGADO)
      Serial.println("LED DESLIGADO"); //IMPRIME O TEXTO NO MONITOR SERIAL
    }
  }
}
