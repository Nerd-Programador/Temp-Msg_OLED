//****************************************************************************************************
//* Projeto para leitura e exibição da temperatura em tempo real no monitor Seria e em um Display.   *
//* Me acompanha nas redes.                                                                          *
//* https://github.com/Felipe-Geek/Temp+Msg_OLED.git                                                       *
//* https://tiktok.com/@.Felipe.Geek/                                                                *
//****************************************************************************************************

#include <Wire.h>               // Biblioteca necessária para se comunicar com o display OLED
#include <OneWire.h>            // Biblioteca necessária para se comunicar com o sensor de temperatura DS18B20
#include <DallasTemperature.h>  // Biblioteca necessária para se comunicar com o sensor de temperatura DS18B20
#include <Adafruit_SSD1306.h>   // Biblioteca necessária para utilizar o display OLED

#define ONE_WIRE_BUS A2  // Pino do arduino que está conectado ao barramento do sensor de temperatura
#define OLED_RESET 4     // Pino do arduino que controla o reset do display OLED

DeviceAddress thermometerAddress;  // Endereço do sensor de temperatura

Adafruit_SSD1306 display(OLED_RESET);    // Instância do display OLED
OneWire oneWire(ONE_WIRE_BUS);           // Instância da biblioteca OneWire
DallasTemperature tempSensor(&oneWire);  // Instância da biblioteca DallasTemperature

void setup() {
  Serial.begin(9600);  // Inicia a comunicação serial com baud rate de 9600

  // Inicializa o display OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  // Mensagem de saudação
  Serial.println("DS18B20 Temperature IC Test");
  Serial.println("Locating devices...");

  // Inicializa o sensor de temperatura
  tempSensor.begin();

  // Verifica se há dispositivo conectado ao barramento
  if (!tempSensor.getAddress(thermometerAddress, 0))
    Serial.println("Unable to find Device.");
  else {
    Serial.println("Device 0 Address: ");
    printAddress(thermometerAddress);  // Mostra o endereço do dispositivo
    Serial.println();
  }
  tempSensor.setResolution(thermometerAddress, 11);  // Define a resolução do sensor de temperatura
}

void loop() {

  tempSensor.requestTemperatures();                      // Requisita a temperatura
  displayTemp(tempSensor.getTempC(thermometerAddress));  // Exibe a temperatura na tela
  delay(500);                                            // Espera por 500ms antes de requisitar novamente a temperatura
}

// Função para exibir a temperatura na tela
void displayTemp(float temperatureReading) {

  tempSensor.requestTemperatures();  // Requisita a temperatura

  // Limpa a tela do display OLED
  display.clearDisplay();

  // Configurações para a exibição do texto na tela
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 0);

  // Exibe a mensagem "Temperatura:"
  display.println("Temperatura:");

  // Configurações para a exibição da temperatura na tela
  display.setTextSize(2);
  display.print(temperatureReading, 1);
  display.print((char)247);  // Caractere para exibir graus Celsius
  display.println("C");

  // Exibe a temperatura na serial
  Serial.print(temperatureReading);
  Serial.print("°");
  Serial.print("C ");

  // Exibe o conteúdo na tela
  display.display();
  delay(5000);

  // Exibe a primeira mensagem
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.println("Mensagem 1");
  display.display();
  delay(3000);

  // Exibe a segunda mensagem
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 8);
  display.println("Mensagem 2");
  display.display();
  delay(3000);
}

// Função para exibir o endereço do termômetro na serial
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}