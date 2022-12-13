#include <ESP8266WiFi.h>  //ESP8266
#include <ESP8266mDNS.h>  //OTA
#include <WiFiUdp.h>      //OTA
#include <ArduinoOTA.h>   //OTA
#include <Arduino.h>
#include <U8g2lib.h>      // lembre de adicionar a biblioteca U8g2  no Arduino IDE  -- Biblioteca para display monocromatico
#include <SPI.h>
#include <Wire.h>

//configurações
#define OLED_SDA  2
#define OLED_SCL 14
#define OLED_RST  4
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA , OLED_RST);
const char *text1 = "Baby Alarm";  // texto para escrever no display
const char *text2 = "Temperatura:";  // texto para escrever no display
const char *text3 = "Batimentos:";  // texto para escrever no display

//configuração do WIFI
#define wifi_ssid "Guuh"
#define wifi_password "BDC5624FC8"
#define WiFi_hostname "Baby Alarm"

//Importante para fazer o Arduino detectar o dispositivo OTA 
WiFiServer TelnetServer(8266);

//configuração do wifi
void setup_wifi() {
  delay(100);
  Serial.println("");
  Serial.print("Conectando a ");
  Serial.println(wifi_ssid);
  WiFi.hostname(WiFi_hostname);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n endereco IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("Configurando o dispositivo OTA ...");
  TelnetServer.begin();   //Importante para fazer o Arduino detectar o dispositivo OTA 

  ArduinoOTA.onStart([]() {Serial.println("OTA ligando...");});
  ArduinoOTA.onEnd([]() {Serial.println("atualizacaoo terminada!");Serial.println("Reiniciando...");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {Serial.printf("OTA carregando: %u%%\r\n", (progress / (total / 100)));});  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Autenticacao falhou");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Inicializçao falhou");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Conexao Falhou");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Recebimento falhou");
    else if (error == OTA_END_ERROR) Serial.println("Eroo ao encerrar");
  });
  ArduinoOTA.begin();
  Serial.println("Wifi OK");
}


void setup() {
  Serial.begin(9600);
  setup_wifi();  
  u8g2.begin();
}

int print=1;
void loop() { 
  ArduinoOTA.handle();
  u8g2.clearBuffer();          // limpa a memória interna
  u8g2.setFont(u8g2_font_8x13B_mf); // escolhe a fonte
    u8g2.drawStr(0,10,text1);  // escreve na memória interna    
  if(print == 1){ 
    u8g2.drawStr(0,29,text2);  // escreve na memória interna
    print = 2;
  }else if(print == 2){
    u8g2.drawStr(0,29,text3);  // escreve na memória interna
    print = 1;
  }
  
  u8g2.sendBuffer();          // transfere da memória interna pro display
  delay(3000);  
}
