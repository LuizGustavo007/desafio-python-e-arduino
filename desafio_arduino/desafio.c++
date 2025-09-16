// Cibely Cristiny e Luiz Gustavo

#include <DHT.h>
#include <Servo.h>

#define PINO_SENSOR_TEMP       2   
#define PINO_SERVO_VENT        9   
#define PINO_BUZZER_ALERTA     8   
#define PINO_BOTAO_EMERGENCIA  3   
#define TIPO_DHT DHT22
DHT sensorTemperatura(PINO_SENSOR_TEMP, TIPO_DHT);

Servo servoVentilacao;

// --- Limites de Temperatura (°C) ---
float LIMITE_FECHADO    = 18.0;  
float LIMITE_25_ABERT   = 22.0;  
float LIMITE_50_ABERT   = 26.0;  
float LIMITE_75_ABERT   = 29.0;  
float LIMITE_TOTAL_ABERT= 29.0;  

float ALERTA_ALTO  = 34.0;       
float ALERTA_BAIXO = 12.0;       

// --- Configurações de Histerese ---
float MARGEM_HISTER = 0.5;

// --- Estado do Sistema ---
bool modoManualAtivo = false;
float ultimaTempLida = 25.0;
int ultimoAnguloVent = 0;
unsigned long ultimaLeitura = 0;
const unsigned long INTERVALO_LEITURA = 5000; 

// --- Ângulos do Servo (ajuste conforme mecânica) ---
int ANGULO_FECHADO = 0;
int ANGULO_25      = 45;
int ANGULO_50      = 90;
int ANGULO_75      = 135;
int ANGULO_ABERTO  = 180;

void setup() {
  Serial.begin(9600);
  sensorTemperatura.begin();
  servoVentilacao.attach(PINO_SERVO_VENT);

  pinMode(PINO_BUZZER_ALERTA, OUTPUT);
  pinMode(PINO_BOTAO_EMERGENCIA, INPUT_PULLUP);

  servoVentilacao.write(ANGULO_FECHADO);
  ultimoAnguloVent = ANGULO_FECHADO;
}

void loop() {
  if (digitalRead(PINO_BOTAO_EMERGENCIA) == LOW) {
    delay(50); // debounce
    if (digitalRead(PINO_BOTAO_EMERGENCIA) == LOW) {
      modoManualAtivo = !modoManualAtivo;
      tone(PINO_BUZZER_ALERTA, 1000, 150);
      Serial.print("Modo manual: ");
      Serial.println(modoManualAtivo ? "ATIVADO" : "DESATIVADO");
      delay(500);
    }
  }

  // Leitura periódica da temperatura
  if (millis() - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = millis();
    float temperatura = sensorTemperatura.readTemperature();

    if (isnan(temperatura)) {
      Serial.println("Falha no sensor! Colocando posição segura.");
      servoVentilacao.write(ANGULO_50);
      tone(PINO_BUZZER_ALERTA, 800);
      delay(300);
      noTone(PINO_BUZZER_ALERTA);
      return;
    }

    Serial.print("Temperatura atual: ");
    Serial.println(temperatura);

    if (modoManualAtivo) {
      servoVentilacao.write(ANGULO_ABERTO);
      verificarAlertas(temperatura);
      return;
    }

    int anguloDestino = calcularAnguloVent(temperatura);
    moverVentilacao(anguloDestino);
    verificarAlertas(temperatura);
  }
}

int calcularAnguloVent(float temp) {
  int destino;
  if (temp <= LIMITE_FECHADO) destino = ANGULO_FECHADO;
  else if (temp <= LIMITE_25_ABERT) destino = ANGULO_25;
  else if (temp <= LIMITE_50_ABERT) destino = ANGULO_50;
  else if (temp <= LIMITE_75_ABERT) destino = ANGULO_75;
  else destino = ANGULO_ABERTO;

  if (abs(temp - ultimaTempLida) < MARGEM_HISTER) {
    return ultimoAnguloVent; 
  }
  ultimaTempLida = temp;
  return destino;
}

void moverVentilacao(int angulo) {
  if (angulo != ultimoAnguloVent) {
    servoVentilacao.write(angulo);
    ultimoAnguloVent = angulo;
    Serial.print("Ventilação ajustada para: ");
    Serial.print(angulo);
    Serial.println("°");
  }
}

void verificarAlertas(float temp) {
  if (temp >= ALERTA_ALTO) {
    for (int i = 0; i < 6; i++) {
      tone(PINO_BUZZER_ALERTA, 2000, 200);
      delay(300);
      noTone(PINO_BUZZER_ALERTA);
      delay(200);
    }
    Serial.println("ALERTA: Temperatura CRÍTICA ALTA!");
  } else if (temp <= ALERTA_BAIXO) {
    for (int i = 0; i < 6; i++) {
      tone(PINO_BUZZER_ALERTA, 700, 300);
      delay(350);
      noTone(PINO_BUZZER_ALERTA);
      delay(150);
    }
    Serial.println("ALERTA: Temperatura CRÍTICA BAIXA!");
  }
}