#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Configuración del LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuración del sensor DHT
#define RELAY_PIN 7           // Pin del relé que controla la alimentación
#define DHTPIN 2              // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11         // Tipo de sensor DHT11
#define BUZZER_PIN 13        // Pin donde está conectado el buzzer
#define BUTTON_PIN 10         // Pin donde está conectado el botón
#define BUZZER_INTERVAL 8000  // Intervalo de tiempo en ms para la alerta
#define BUZZER_FREQUENCY 180 // Frecuencia del sonido en Hz

DHT dht(DHTPIN, DHTTYPE);

float tempObjetivo = 37.0;  // Temperatura objetivo en grados Celsius
float tempMinima = 34.0;    // Temperatura mínima para activar la alerta
float tempMaxima = 40.0;    // Temperatura máxima para activar la alerta

unsigned long lastBuzzerTime = 0;  // Variable para almacenar el tiempo de la última alerta
unsigned long buzzerMuteTime = 0;  // Variable para gestionar el silencio temporal
bool buzzerActive = false;         // Indica si el buzzer está sonando
bool buzzerMuted = false;          // Indica si el buzzer está en modo silencio

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);                // Inicia la comunicación serial
  dht.begin();                       // Inicia el sensor DHT
  pinMode(RELAY_PIN, OUTPUT);        // Configura el pin del relé como salida
  pinMode(BUZZER_PIN, OUTPUT);       // Configura el pin del buzzer como salida
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configura el botón como entrada con resistencia pull-up
  digitalWrite(RELAY_PIN, HIGH);     // Corta la alimentación inicialmente
  noTone(BUZZER_PIN);                // Apaga el buzzer inicialmente
  lcd.setCursor(0, 0);
  lcd.print("Sistema Iniciado");
  delay(2000);  // Mostrar mensaje inicial por 2 segundos
  lcd.clear();
}

void loop() {
  float tempActual = dht.readTemperature();  // Lee la temperatura
  float humedadActual = dht.readHumidity();  // Lee la humedad

  // Verifica si el botón fue presionado para silenciar el buzzer
  if (digitalRead(BUTTON_PIN) == LOW) {
    buzzerMuted = true;                 // Activa el modo silencio
    buzzerMuteTime = millis();          // Guarda el tiempo actual
    noTone(BUZZER_PIN);                 // Asegura que el buzzer se apague
    lcd.setCursor(0, 1);
    lcd.print("Buzzer silenciado");
    delay(200);                         // Evita rebotes
  }

  // Restablece el buzzer después de 30 segundos de silencio
  if (buzzerMuted && millis() - buzzerMuteTime >= 30000) {
    buzzerMuted = false; // Desactiva el modo silencio
    lcd.setCursor(0, 1);
    lcd.print("                "); // Limpia el mensaje
  }

  // Verificar si la lectura del sensor es válida
  if (isnan(tempActual) || isnan(humedadActual)) {
    Serial.println("Error en la lectura del sensor DHT11");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALERTA: Error");
    lcd.setCursor(0, 1);
    lcd.print("en sensor DHT11");
    activarAlerta();  // Activa la alerta si el sensor falla
  } else {
    // Mostrar temperatura y humedad en el LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempActual, 1);  // Muestra la temperatura con 1 decimal
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humedadActual, 1);  // Muestra la humedad con 1 decimal
    lcd.print("%");

    // Enviar los datos a la computadora
    Serial.print("Temperatura: ");
    Serial.print(tempActual);
    Serial.print(" °C, Humedad: ");
    Serial.print(humedadActual);
    Serial.println(" %");

    // Control del relé
    controlarRele(tempActual);

    // Verificar si la temperatura está fuera del rango
    if (tempActual < tempMinima) {
      lcd.setCursor(0, 1);
      lcd.print("Temp BAJA!");
      activarAlerta();  // Activa la alerta
    } else if (tempActual > tempMaxima) {
      lcd.setCursor(0, 1);
      lcd.print("Temp ALTA!");
      activarAlerta();  // Activa la alerta
    } else {
      desactivarAlerta();  // Desactiva la alerta si está dentro del rango
    }
  }

  // Control del buzzer para la alerta
  controlarBuzzer();

  delay(500);  // Espera medio segundo antes de la siguiente lectura
}

// Función para activar la alerta del buzzer
void activarAlerta() {
  buzzerActive = true;
}

// Función para desactivar la alerta del buzzer
void desactivarAlerta() {
  buzzerActive = false;
  noTone(BUZZER_PIN);  // Apaga el buzzer por seguridad
}

// Función para controlar el buzzer pasivo
void controlarBuzzer() {
  if (buzzerActive && !buzzerMuted) {
    unsigned long currentTime = millis();
    if (currentTime - lastBuzzerTime >= BUZZER_INTERVAL) {
      tone(BUZZER_PIN, BUZZER_FREQUENCY);  // Genera una señal de 1 kHz
      delay(500);                          // Suena el buzzer durante 0.5 segundos
      noTone(BUZZER_PIN);                  // Detiene el tono
      lastBuzzerTime = currentTime;        // Actualiza el tiempo de la última alerta
    }
  }
}

// Función para controlar el relé
void controlarRele(float tempActual) {
  if (tempActual < tempObjetivo) {
    digitalWrite(RELAY_PIN, LOW);  // Activa la alimentación
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Corta la alimentación
  }
}
