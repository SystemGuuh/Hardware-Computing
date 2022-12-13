#include <ESP8266WiFi.h>      //ESP8266
#include <ESP8266mDNS.h>      //OTA
#include <DNSServer.h>        //DNS server
#include <ESP8266WebServer.h> //Web server
#include <WiFiUdp.h>          //OTA
#include <ArduinoOTA.h>       //OTA
#include <Arduino.h>
#include <U8g2lib.h>          // lembre de adicionar a biblioteca U8g2  no Arduino IDE  -- Biblioteca para display monocromatico
#include <SPI.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>

//configurações
#define OLED_SDA  2
#define OLED_SCL 14
#define OLED_RST  4
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, OLED_SCL, OLED_SDA , OLED_RST);
const char *text1 = "Baby Alarm";  // texto para escrever no display

// Configurações de usuário
#define SSID_NAME "BabyAlarm"
#define SUBTITLE "Configuracao de dispositivo."
#define TITLE "Forms de configuracao"
#define BODY "Escreva o nome e a senha da rede para conectar o dispositivo. Usaremos essas informacoes para acessar a internet e mandar os dados para seu celular."
#define POST_TITLE "Dispositivo conectado."
#define POST_BODY "O dispostivo esta conectando, aguarde alguns minutos.</br>Obrigado."
#define PASS_TITLE "Credenciais"
#define CLEAR_TITLE "Limpo"

// Configuracoes para o sensor de temperatura
const int oneWireBus = 4;  
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Variaveis pro sensor de som
int sensorPin = 0;    // select the input pin for the potentiometer
int sensorValue1 = 0;  // variable to store the value coming from the sensor
int sensorValue2 = 0;
int cont = 0;
int variacao = 0;

// Iniciando configurações do sistema
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

//Configuração do WIFI
String wifi_ssid;
String wifi_password;
#define WiFi_hostname "Baby Alarm"

//Variaveis para o bot de wpp
String YOUR_PHONE; //Numero do celular sem o +
String YOUR_APIKEY; //Chave da API(APIKEY)
const String URL = "http://api.whatabot.net/whatsapp/sendMessage?text=";
WiFiUDP ntpUDP;

//Importante para fazer o Arduino detectar o dispositivo OTA 
WiFiServer TelnetServer(8266);

// Pagina de login
String Credentials="";
unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String input(String argName) {
  String a=webServer.arg(argName);
  a.replace("<","&lt;");a.replace(">","&gt;");
  a.substring(0,200); return a; }

String footer() { return 
  "</div><div class=q><a>Viva bem, viva alerta.</a></div>";
}

String header(String t) {
  String a = String(SSID_NAME);
  String CSS = "article { background: #f2f2f2; padding: 1.3em; }" 
    "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
    "div { padding: 0.5em; }"
    "h1 { margin: 0.5em 0 0 0; padding: 0.5em; }"
    "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; }"
    "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
    "nav { background: #0066ff; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
    "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
    "textarea { width: 100%; }";
  String h = "<!DOCTYPE html><html>"
    "<head><title>"+a+" :: "+t+"</title>"
    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
    "<style>"+CSS+"</style></head>"
    "<body><nav><b>"+a+"</b> "+SUBTITLE+"</nav><div><h1>"+t+"</h1></div><div>";
  return h; }

  String creds() {
  return header(PASS_TITLE) + "<ol>" + Credentials + "</ol><br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p></center>" + footer();
}

String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action=/post method=post>" +
    "<b>Nome da rede:</b> <center><input type=text name=rede placeholder='SSID do reteador'></input></center>" +
    "<b>Senha:</b> <center><input type=password name=password placeholder='Senha do roteador'></center>" + 
    "<b>Numero de celular:</b> <center><input type=text name=cell placeholder='0000000000000'></input></center>" +
    "<b>Chave APIKEY:</b> <center><input type=text name=apikey placeholder='12345678'></input><input type=submit value=\"Enviar\"></form></center>" +
    footer();
}

String posted() {
  String rede=input("rede");
  String password=input("password");
  String celular=input("cell");
  String apikey=input("apikey");
  wifi_ssid = rede;
  wifi_password = password;
  YOUR_PHONE = celular;
  YOUR_APIKEY = apikey;
  Credentials="<li>Nome da rede: <b>" + rede + "</b></br>Senha: <b>" + password + "</b></li>" + Credentials;
  return header(POST_TITLE) + POST_BODY + footer();
}

String clear() {
  String rede="<p></p>";
  String password="<p></p>";
  Credentials="<p></p>";
  return header(CLEAR_TITLE) + "<div><p>A lista foi limpa.</div></p><center><a style=\"color:blue\" href=/>Volta ao inicio</a></center>" + footer();
}


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
  //Coletando credenciais
  bootTime = lastActivity = millis();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID_NAME);
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Apenas HTTP)
  webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); setup_wifi(); });
  webServer.on("/creds",[]() { webServer.send(HTTP_CODE, "text/html", creds()); });
  webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  
  //Configurando para printar no display
  Serial.begin(9600);   
  u8g2.begin();

}

// Manda mensagem apos conectar o celular
void sendWhatsapp(String text) {
  WiFiClient client;
  HTTPClient https;
  String url = URL + text + "&apikey=" + YOUR_APIKEY + "&phone=" + YOUR_PHONE;
  https.begin(client, url);
  int httpCode = https.GET();
  https.end();
  //Serial.println("mensagem enviada");
}

// Manda mensagem padrao pro wpp
void mensagemWPP(String txt){
  sendWhatsapp("Aqui estao os dados do bebe");
  sendWhatsapp(txt);
}


int msg=1;
int tempo=0;
int chave =0;

void loop() { 
  //longin na rede
  if ((millis()-lastTick)>TICK_TIMER) {lastTick=millis();} 
dnsServer.processNextRequest(); webServer.handleClient();

  if(WiFi.isConnected()){
    if(msg == 1){
      sendWhatsapp("BabyAlarm diz oi");
      sendWhatsapp("seguir vou mandando atualizacoes a cada 1 hora do seu bebe, lembresse de conectar novamente da proxima vez que nos vermos");
      msg = 0;  
    }
    
  //acende o display  
  ArduinoOTA.handle();
  u8g2.clearBuffer();          // limpa a memória interna
  u8g2.setFont(u8g2_font_8x13B_mf); // escolhe a fonte
  u8g2.drawStr(0,10,text1);  // escreve na memória interna    
  u8g2.sendBuffer();          // transfere da memória interna pro display

  //faz leitura da temperatura
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  String temp = String(temperatureC);
  String tempTXT = "Temperatura: " + temp + "ºC";
  //Serial.println(tempTXT);

  if(tempo > 60*60){
    mensagemWPP(tempTXT);
    chave = 0;
   }

  //faz leitura de som
  sensorValue1 = analogRead(sensorPin);
  sensorValue2 = analogRead(sensorPin);
  variacao = (sensorValue1 - sensorValue2);
  if (variacao>1 || variacao<-1)  {
    cont = cont+1;
    if (cont>4 && chave == 0){
      // BEBE CHORANDO
      sendWhatsapp("Seu bebe esta chorando");
      //Serial.println("Seu bebê está chorando!");
      cont = 0;
      chave = 1;
    }
  }

  
  delay(1000);  
  tempo++;
  }
}
