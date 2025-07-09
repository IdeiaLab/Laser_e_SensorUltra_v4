// IdeiaLab, UERJ/FCEE/DEPROIN

/*

Observação
Lembrar que o circuito para alimentação do laser deve respeitar o mesmo formato do encontrado no site: http://www.bosontreinamentos.com.br/eletronica/arduino/controlando-velocidade-de-motor-dc-com-arduino-e-potenciometro-projeto/

De modo que o laser irá o lugar do motor no circuito.

*/

const int pinoLDR = A0;  // Pino onde o LDR está conectado

// Array para armazenar as leituras
int leituras[4];

// Limites para as leituras do LDR
int limiteInferior = 900;
int limiteSuperior = 1010;

#define AlimentacaoLaser 9  //pino INDIVIDUAL de alimentação do LDR, para que o mesmo não tenha sua tensão dividida com a do laser, o que faz variar a leitura quando o mesmo é ligado

//*******Inclusão sensor ultrassônico*******
const int TRIG = 3;
const int ECHO = 2;
const int distancia_obstaculo = 5;
//******************************************

void setup() {
  Serial.begin(9600);       // Inicia a comunicação serial
  pinMode(pinoLDR, INPUT);  // Configura o pino do LDR como entrada

  // configura o pino de alimentação do LDR como saída
  pinMode(AlimentacaoLaser, OUTPUT);

  //*******Inclusão sensor ultrassônico*******
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  //******************************************
}

void loop() {

  //*******Inclusão sensor ultrassônico*******

  int distancia = sensor_morcego(TRIG, ECHO);
  if (distancia <= distancia_obstaculo) {

    delay(1000); //delay para que o objeto alcance o feixe do laser. Este podendo ser alterado de acordo com a disposição final do sensor ultrassônico.

    //###### ______ATIVAÇÃO DO SISTEMA DO LAZER______######

    //saída digital alta para a alimentação do laser
    digitalWrite(AlimentacaoLaser, HIGH);

    // Realiza quatro leituras do LDR com intervalos de 250ms
    for (int i = 0; i < 4; i++) {
      leituras[i] = analogRead(pinoLDR);
      delay(250);
    }

    // Verifica se alguma das leituras está dentro do intervalo predefinido
    bool IntervaloTransl = false;
    for (int i = 0; i < 4; i++) {
      if (leituras[i] >= limiteInferior && leituras[i] <= limiteSuperior) {
        IntervaloTransl = true;
        break;  // Saia do loop se encontrar uma leitura dentro do intervalo
      }
    }

    // Imprime as leituras
    for (int i = 0; i < 4; i++) {
      Serial.print("Leitura ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(leituras[i]);
    }

    // Verifica e imprime o resultado da verificação
    if (IntervaloTransl) {
      Serial.println("Objeto translúcido detectado! Ativar mecanismo de separação.");
    } else {
      Serial.println("Objeto NÃO translúcido detectado! NÃO ativar mecanismo de separação.");
    }

    delay(1000);  // Aguarda 2 segundos para uma nova iteração
  }
  else {

    Serial.println("Nenhum objeto detectado na esteira.");
    digitalWrite(AlimentacaoLaser, LOW);
  }
  delay(1000);

  //******************************************
}

//*******Inclusão sensor ultrassônico*******
int sensor_morcego(int pinotrig, int pinoecho) {
  digitalWrite(pinotrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinotrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig, LOW);

  return pulseIn(pinoecho, HIGH) / 58;
}
//******************************************