#include <Ultrasonic.h>
#include <SoftwareSerial.h>

// Pinos não usados: A5, A6, A7

// SENSORES:
const int pinoRX = 0; //PINO DIGITAL 0 (RX) BLUETOOTH
const int pinoTX = 1; //PINO DIGITAL 1 (TX) BLUETOOTH
const int trigger[4] = {2,4,7,8}; // define os pinos para mandar o sinal do sensor de detecção do adversário.
const int echo[4] = {10,11,12,13}; // define os pinos para recebe o sinal do sensor de detecção do adversário.
const int sensores_down[4] = {A0,A1,A2,A3}; // sensores para detectar o tatâme.


SoftwareSerial bluetooth(pinoRX, pinoTX); //PINOS QUE EMULAM A SERIAL, ONDE O PINO 2 É O RX E O PINO 3 É O TX


//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(trigger[0], echo[0]); //sensor 1
Ultrasonic ultrasonic2(trigger[1], echo[1]); //sensor 2
Ultrasonic ultrasonic3(trigger[2], echo[2]); //sensor 3
Ultrasonic ultrasonic4(trigger[3], echo[3]); //sensor 4

// MOTOR ESQUERDO:

const int in1 = 3;   // in1 = 1 (horário)
const int in2 = 5;   // in2 = 1 (anti-horário)

// MOTOR DIREITO:

const int in3 = 6;    // in3 = 1 (horário)
const int in4 = 9;     // in4 = 1 (anti-horário)

// VARIÁVEIS À PARTE:

const int bot = A4; // // variável utilizada para receber um valor do botao.
int ladoadv;  // variável utilizada para receber um valor caso o adversário seja detectado.
int chao;     // variável utilizada para receber um valor caso a linha seja detectada.
int lersp;    // variável utilizada para receber o valor dos sensores de presença.
int lerdown;  // variável utilizada para receber o valor dos sensores de linha.
float limiar; // limite para detectar luz no sensor de linha.
float dist[4] = {}; // Armazena a distância de cada sensor, para verificar se o adversário está perto.
int dadoBluetooth = 0; //VARIÁVEL QUE ARMAZENA O VALOR ENVIADO PELO BLUETOOTH.
int ligabt; // Variável que liga o sistema pelo bluetooth.

void setup() {

  pinMode(sensores_down[0],INPUT);
  pinMode(sensores_down[1],INPUT);
  pinMode(sensores_down[2],INPUT);
  pinMode(sensores_down[3],INPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(bot,INPUT);

}

void loop() 
{ 
 if ( digitalRead(bot) == HIGH || ligabt == 1) 
 {
     lersensores();
     procurando();
     achou();
     ladoadv = 0; // limpa a variável no fim de cada loop, senão a variável fica com o valor lido até a de infinito. ????????
     chao = 0; // limpa a variável no fim de cada loop, senão a variável fica com o valor lido até a de infinito. ????????
 }
}
void lerbluetooth() {
   if(bluetooth.available()){ //SE O BLUETOOTH ESTIVER HABILITADO, FAZ
       dadoBluetooth = bluetooth.read(); //VARIÁVEL RECEBE O VALOR ENVIADO PELO BLUETOOTH
 
    if(dadoBluetooth == '1'){ //SE O VALOR RECEBIDO FOR IGUAL A 1, FAZ
      ligabt = 1;
    }
    if(dadoBluetooth == '0'){ //SE O VALOR RECEBIDO FOR IGUAL A 0, FAZ
      ligabt = 0;
    }
  }
}
void lersensores() {
  for (int i = 0; i < 4; i++) {
    lerdown = analogRead(sensores_down[i]);
    limiar = 500; //****** EXEMPLO
    
    if(lerdown > limiar) {
      chao = i + 1;
    }
   
  
    //Lê as informacoes do sensor frontal
    long microsec = ultrasonic.timing();
    dist[0] = ultrasonic.convert(microsec, Ultrasonic::CM);
    
    //Le as informacoes do sensor direito
    long microsec2 = ultrasonic2.timing();
    dist[1] = ultrasonic2.convert(microsec2, Ultrasonic::CM);
   
    //Le as informacoes do sensor traseiro
    long microsec3 = ultrasonic3.timing();
    dist[2] = ultrasonic3.convert(microsec3, Ultrasonic::CM);
  
    //Le as informacoes do sensor esquerdo
    long microsec4 = ultrasonic4.timing();
    dist[3] = ultrasonic4.convert(microsec4, Ultrasonic::CM);

    
  delay(10);
  for (int e = 0; e < 4; e++) {
    if (dist[e] > 2 && dist[e] < 30) {
      ladoadv = e + 1;
    }
  }
  

void procurando() {
  
 
  if((chao == 0) && (ladoadv == 0)) { // Se não tiver chao nem adversário
    frente();
  }
  if((chao == 1) && (ladoadv == 0)) { // Detecta a linha frontal com sensor esquerdo e não tem adversário
    do {
       analogWrite(in1,30);
       analogWrite(in2,0);
       analogWrite(in3,0);
       analogWrite(in4,200);
       lersensores();
    } while ((chao != 3) && (ladoadv == 0));
  }
  if((chao == 2) && (ladoadv == 0)) { // Detecta a linha frontal com sensor direito e não tem adversário
    do {
       analogWrite(in1,0);
       analogWrite(in2,200);
       analogWrite(in3,30);
       analogWrite(in4,0);
       lersensores();
    } while ((chao != 4) && (ladoadv == 0));
  }
  
}

void achou() {
  if((ladoadv == 1) && (chao == 0)) { // Adversário à frente
    frentemax();
  }
  if(ladoadv == 1 && chao == 0) { // Adversário à frente
    frentemax();
  }  
  if((ladoadv == 2) && (chao == 0)){ // Adversário à direita
    giraDir();
  }
  if((ladoadv == 3) && (chao == 0)){ // Adversário à esquerda
    giraEsq();   
  }
  if((ladoadv == 4) && (chao == 0)){ // Adversário atrás
    giraEsq();    
  }
}


void frente() {
 analogWrite(in1,200);
 analogWrite(in2,0);
 analogWrite(in3,200);
 analogWrite(in4,0);
}
void re() {
 analogWrite(in1,0);
 analogWrite(in2,255);
 analogWrite(in3,0);
 analogWrite(in4,255);
}
void giraDir() {
 analogWrite(in1,0);
 analogWrite(in2,255);
 analogWrite(in3,0);
 analogWrite(in4,0);
}
void giraEsq() {
 analogWrite(in1,0);
 analogWrite(in2,0);
 analogWrite(in3,0);
 analogWrite(in4,255);
}

void pare() {
 analogWrite(in1,0);
 analogWrite(in2,0);
 analogWrite(in3,0);
 analogWrite(in4,0);
}

void frentemax() {
 analogWrite(in1,0);
 analogWrite(in2,255);
 analogWrite(in3,0);
 analogWrite(in4,255);  
}
