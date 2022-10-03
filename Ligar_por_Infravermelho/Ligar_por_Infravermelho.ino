#include <IRremote.h>

#define receptor 2
float valor;

IRrecv recIR(receptor); //IRrecv -> objeto recIR(receptor) -> passa o construtor, receptor
decode_results resultado;

void setup() {
  pinMode(receptor,INPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  recIR.enableIRIn(); //habilita e inicia receptor infravermelho
}

void loop() {
  // put your main code here, to run repeatedly:
  if(recIR.decode(&resultado)){//identifica se houve uma recepçao
    Serial.println("Iniciado...");
    valor = (resultado.value); //!!!CODIFICA O RESULTADO OBTIDO DO CONTROLE
    Serial.print("Valor: ");
    Serial.println(valor, HEX); //printa o valor em hexadecimal
    //Serial.println(resultado.value, HEX);
    recIR.resume(); //prepara para receber um novo comando
  }

  

}
