# Incubadora Automática de Huevos con Monitoreo de Temperatura y Humedad

Incubadora básica de huevos, utilizando un **sensor DHT11** para monitorear la temperatura y la humedad del entorno. La incubadora ajusta automáticamente la temperatura mediante un **relé** o un transistor, y alerta al usuario si las condiciones no son ideales para el proceso de incubación. Además, tiene una funcionalidad de **silenciar la alerta sonora** mediante un **botón**.

Este proyecto requiere una **fuente de calor** como una **resistencia de calor** junto con un **ventilador** para generar la temperatura adecuada para la incubación de los huevos. El **relé** controlará la alimentación de esta fuente de calor para mantener la temperatura dentro del rango óptimo.

## Componentes Necesarios

- **Arduino (cualquier modelo)**
- **Sensor DHT11** (para medir temperatura y humedad)
- **Pantalla LCD 16x2 con I2C** (para visualizar temperatura y humedad)
- **Buzzer** (para emitir una alerta sonora)
- **Relé** (para controlar un dispositivo como una resistencia de calor o ventilador)
- **Botón** (para silenciar el buzzer durante 30 segundos)
- **Resistencias** (una para el botón, de 10kΩ)
- **Resistencia de calor y ventilador** (para generar la temperatura necesaria en la incubadora)

## Descripción del Funcionamiento

1. **Lectura de temperatura y humedad**: El **sensor DHT11** mide constantemente la temperatura y la humedad del entorno de la incubadora.
2. **Pantalla LCD**: Muestra en tiempo real la temperatura y la humedad del ambiente.
3. **Control de temperatura**: Si la temperatura cae por debajo de los 34°C o supera los 40°C, se activa el **buzzer** para alertar al usuario, y el **relé** ajusta la fuente de calor (por ejemplo, una **resistencia de calor** y un **ventilador**) para estabilizar la temperatura.
4. **Alerta sonora**: El **buzzer** emite un sonido cada vez que la temperatura se sale del rango ideal de incubación.
5. **Botón de silenciado**: Si el **botón** es presionado, el buzzer se apaga durante 30 segundos, permitiendo al usuario silenciar la alerta sin apagar todo el sistema.

## Rango de Temperatura para Incubación

- **Temperatura objetivo**: 37°C (ideal para incubar los huevos)
- **Temperatura mínima**: 34°C (por debajo de este valor, el proceso de incubación no será efectivo)
- **Temperatura máxima**: 40°C (temperaturas superiores pueden dañar los huevos)

## Conexiones

- **Sensor DHT11**:
  - VCC → 5V
  - GND → GND
  - DATA → Pin 2 (DHTPIN)

- **Pantalla LCD (I2C)**:
  - VCC → 5V
  - GND → GND
  - SDA → A4
  - SCL → A5

- **Buzzer**:
  - Positivo → Pin 11 (BUZZER_PIN)
  - Negativo → GND

- **Relé**:
  - IN → Pin 7 (RELAY_PIN)
  - VCC → 5V
  - GND → GND

- **Botón**:
  - Un terminal → Pin 0 (BUTTON_PIN)
  - Otro terminal → GND

---

Si necesitas ayuda o tienes alguna pregunta, no dudes en abrir una [issue](https://github.com/LuciaNishimiya/HuevitoSmart/issues) en este repositorio.

