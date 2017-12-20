
/* ============================================
  I2Cdev device library code is placed under the MIT license
  Copyright (c) 2012 Jeff Rowberg

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  ===============================================
*/

// ================================================================
// ===       BIBLIOTECAS, CONSTANTES, VARIÃ�VEIS E OBJETOS       ===
// ================================================================

//Bibliotecas necessÃ¡rias
#include "I2Cdev.h" //biblioteca para comunicaÃ§Ã£o I2C
#include "MPU9250_9Axis_MotionApps41.h" //biblioteca para utilizaÃ§Ã£o do DMP do MPU-9250
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h" // biblioteca necessÃ¡ria para comunicaÃ§Ã£o I2C
#endif

//DefiniÃ§Ã£o dos pinos de interrupÃ§Ãµes das IMUs
#define INTERRUPT_PIN 2  //interrupÃ§Ã£o da IMU do braÃ§o
#define INTERRUPT_PIN2 3  //interrupÃ§Ã£o da IMU da mÃ£o
#define LED_PIN 13 //pino do LED do Arduino

//variÃ¡vel para controle do LED
bool blinkState = false;

//VariÃ¡veis para controle das IMUs
bool dmpReady = false;  //true se inicializaÃ§Ã£o do DMP1 ocorreu com sucesso
bool dmpReady2 = false;  //true se inicializaÃ§Ã£o do DMP2 ocorreu com sucesso
uint8_t mpuIntStatus, mpuIntStatus2;  //contÃ©m o byte de status da interrupÃ§Ã£o do MPU-9250
uint8_t devStatus, devStatus2;     //retorna o status apÃ³s cada operaÃ§Ã£o no dispositivo (0=sucesso, !0=erro)
uint16_t packetSize, packetSize2;   //tamanho esperado do pacote do DMP (padrÃ£o Ã© 42 bytes)
uint16_t fifoCount, fifoCount2;    //conta os bytes na FIFO
uint8_t fifoBuffer[64], fifoBuffer2[64]; //buffer de armazenamento da FIFO
String valores; //string com valores para escrever na porta serial

VectorFloat gravity, gravity2;
float ypr[3], ypr2[3]; 

//InstÃ¢ncias das IMUs
MPU9250 mpu(0x68); //IMU do baÃ§o
MPU9250 mpu2(0x69); //IMU da mÃ£o


//contÃ©m os dados de orientaÃ§Ã£o das IMUs
Quaternion q, q2;


// ================================================================
// ===            ROTINA DE DETECÃ‡ÃƒO DE INTERRUPÃ‡Ã•ES            ===
// ================================================================

volatile bool mpuInterrupt = false;     // indica se o pino de interrupÃ§Ã£o da IMU foi para 1
volatile bool mpuInterrupt2 = false;     // indica se o pino de interrupÃ§Ã£o da IMU foi para 1
void dmpDataReady() {
  mpuInterrupt = true;
}

void dmpDataReady2() {
  mpuInterrupt2 = true;
}

// ================================================================
// ===                   CONFIGURAÃ‡ÃƒO INICIAL                   ===
// ================================================================

void setup() {
  // conexÃ£o do bus I2c
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); // clock de comunicaÃ§Ã£o I2C 400kHz.
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // inicializa comunicaÃ§Ã£o serial
  Serial.begin(115200);
  while (!Serial);

  // inicializa dispositivos
  // Serial.println(F("Inicializando dispositivos I2C..."));
  mpu.initialize();
  delay(100);
  mpu2.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  pinMode(INTERRUPT_PIN2, INPUT);

  // testa conexÃ£o
  // Serial.println(F("Testando conexÃµes..."));
  //Serial.println(mpu.testConnection() ? F("MPU9250 - 1 conexÃ£o OK") : F("MPU9250 - 1 conexÃ£o falhou"));
  //delay(100);
  //Serial.println(mpu2.testConnection() ? F("MPU9250 - 2 conexÃ£o OK") : F("MPU9250 - 2 conexÃ£o falhou"));

  // carregar e configurar o DMP
  //Serial.println(F("Inicializando DMP1..."));
  devStatus = mpu.dmpInitialize(0x68);
  //Serial.println(F("Inicaalizando DMP2..."));
  devStatus2 = mpu2.dmpInitialize(0x69);

  // Testa se inicializaÃ§Ã£o ocorreu sem erros
  if (devStatus == 0 && devStatus2 == 0) {
    // liga o DMP
    //Serial.println(F("Habilitando DMP1..."));
    mpu.setDMPEnabled(true);
    //Serial.println(F("Habilitando DMP2..."));
    mpu2.setDMPEnabled(true);
    // Habilita deteÃ§Ã£o de interrupÃ§Ãµes do Arduino
    //Serial.println(F("Habilitando deteÃ§Ã£o de interrupÃ§Ãµes..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    //Serial.println(F("Habilitando deteÃ§Ã£o de interrupÃ§Ãµes..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN2), dmpDataReady2, RISING);
    mpuIntStatus2 = mpu2.getIntStatus();

    // configura o flag do DMP para o Loop Principal saber se pode utilizÃ¡-lo
    //Serial.println(F("DMP pronto! Esperando primeira interrupÃ§Ã£o..."));
    dmpReady = true;
    //Serial.println(F("DMP2 pronto! Esperando primeira interrupÃ§Ã£o..."));
    dmpReady2 = true;
    //recebe tamanho esperado do pacote do DMP para comparaÃ§Ã£o
    packetSize = mpu.dmpGetFIFOPacketSize();
    packetSize2 = mpu2.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = carregamento inicial de memÃ³ria falhou
    // 2 = autalizaÃ§Ãµes de connfiguraÃ§Ã£o do DMP falharam
//    Serial.println(F("Algo deu errado"));
//    Serial.print(F("InicializaÃ§Ã£o do DMP1 (cÃ³digo "));
//    Serial.print(devStatus);
//    Serial.println(F(")"));
//    Serial.print(F("InicializaÃ§Ã£o do DMP2 (cÃ³digo "));
//    Serial.print(devStatus2);
//    Serial.println(F(")"));
  }
  // configura LED como saÃ­da
  pinMode(LED_PIN, OUTPUT);

  Serial.println("T:\tX1\tY1\tZ1\tX2\tY2\tZ2\tXdiff\tYdiff\tZdiff");
}

// ================================================================
// ===                     LOOP PRINCIPAL                       ===
// ================================================================

void loop() {

  // se programaÃ§Ã£o do DMP falhou, nÃ£o faÃ§a nada
  if (!dmpReady || !dmpReady2) return;

  // espera interrupÃ§Ã£o do DMP ou pacotes disponÃ­veis
  while ((!mpuInterrupt && fifoCount < packetSize) || (!mpuInterrupt2 && fifoCount2 < packetSize2)) {
  }

  // reseta flag de interrupÃ§Ã£o e recebe byte de status da interrupÃ§Ã£o
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  mpuInterrupt2 = false;
  mpuIntStatus2 = mpu2.getIntStatus();

  // recebe contagem da FIFO
  fifoCount = mpu.getFIFOCount();
  fifoCount2 = mpu2.getFIFOCount();
  // verifica se houve overflow
  if (((mpuIntStatus & 0x10) || fifoCount == 1024) || ((mpuIntStatus2 & 0x10) || fifoCount2 == 1024)) {
    // reseta FIFO
    mpu.resetFIFO();
    mpu2.resetFIFO();
    // Serial.println(F("FIFO overflow!"));

    // caso contrÃ¡rio, verifica pela interrupÃ§Ã£o do DMP
  } else if (mpuIntStatus & 0x02 || mpuIntStatus2 & 0x02) {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // ler pacote da FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    mpu2.getFIFOBytes(fifoBuffer2, packetSize2);

    // observa a contagem da FIFO caso haja > 1 pacote disponÃ­vel (para leitura imediata, sem esperar uma interrupÃ§Ã£o)
    fifoCount -= packetSize;
    fifoCount2 -= packetSize2;

  // recebe os valores de orientaÃ§Ã£o na forma de quaterniÃµes
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
    mpu2.dmpGetGravity(&gravity2, &q2);
    mpu2.dmpGetYawPitchRoll(ypr2, &q2, &gravity2);

    valores = String(millis())+"\t";
    valores += String(ypr[0] * 180/M_PI) + "\t"; 
    valores += String(ypr[1] * 180/M_PI) + "\t"; 
    valores += String(ypr[2] * 180/M_PI) + "\t"; 
    valores += String(ypr2[0] * 180/M_PI) + "\t";
    valores += String(ypr2[1] * 180/M_PI) + "\t";
    valores += String(ypr2[2] * 180/M_PI) + "\t";
    valores += String((ypr[0] * 180/M_PI) - (ypr2[0] * 180/M_PI)) + "\t";
    valores += String((ypr[1] * 180/M_PI) - (ypr2[1] * 180/M_PI)) + "\t";
    valores += String((ypr[2] * 180/M_PI) - (ypr2[2] * 180/M_PI));
    
    //envia valores  para porta serial
    Serial.println(valores);

    // pisca LED para indicar atividade
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
  }
  delay(10);
}








