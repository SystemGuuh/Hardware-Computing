
#include <ESP8266WiFi.h>      //ESP8266
#include <ESP8266mDNS.h>      //OTA
#include <DNSServer.h>        //DNS server
#include <ESP8266WebServer.h> //Web server
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>

// Configurações bot whatsapp
String YOUR_PHONE; 
String YOUR_APIKEY; //Your API KEY
const String URL = "http://api.whatabot.net/whatsapp/sendMessage?text=";

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

// Iniciando configurações do sistema
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

//Configuração do WIFI
String wifi_ssid;
String wifi_password;
#define WiFi_hostname "Baby Alarm"

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
    "<b>Numero celular:</b> <center><input type=number name=cell placeholder='DDDD123456789'></center>" + 
    "<b>APIKEY:</b> <center><input type=number name=apikey placeholder='1234567'></center>" + 
     "<input type=submit value=\"Enviar\"></form></center>" +
    footer();
}

String posted() {
  String rede=input("rede");
  String password=input("password");
  String celular=input("cell");
  String apikey=input("apikey");
  wifi_ssid = rede;
  wifi_password = password;
  YOUR_PHONE= celular; 
  YOUR_APIKEY= apikey;
  Credentials="<li>Nome da rede: <b>" + rede + "</b></br>Senha: <b>" + password + "</b></li>" + Credentials;
  return header(POST_TITLE) + POST_BODY + footer();
}


//configuração do wifi
void setup_wifi() {
  delay(100);
  Serial.println("");
  Serial.print("Conectando a ");
  Serial.println(wifi_ssid);
  Serial.print("O telefone eh: ");
  Serial.println(YOUR_PHONE);
  WiFi.hostname(WiFi_hostname);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n endereco IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Wifi OK");
}

//manda mensagem no bot
void sendWhatsapp(String text) {
  WiFiClient client;
  HTTPClient https;
  String url = URL + text + "&apikey=" + YOUR_APIKEY + "&phone=" + YOUR_PHONE;
  https.begin(client, url);
  int httpCode = https.GET();
  https.end();
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
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();
  
  //Configurando monitor serial
  Serial.begin(9600);
}

// Variaveis pro sensor de som e execuçao
int mensagem=1;
int sensorPin = 0;    
int sensorValue1 = 0;  
int sensorValue2 = 0;
int cont = 0;
int variacao = 0;

void loop() { 
  //longin na rede
  if ((millis()-lastTick)>TICK_TIMER) {lastTick=millis();} 
dnsServer.processNextRequest(); webServer.handleClient();
  
  if(WiFi.isConnected()){
  
  //faz leitura da temperatura
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  String temp = String(temperatureC);
  String tempTXT = "Temperatura:%20" + temp + "ºC";
  Serial.println(tempTXT);

  //faz leitura de som
  sensorValue1 = analogRead(sensorPin);
  sensorValue2 = analogRead(sensorPin);
  variacao = (sensorValue1 - sensorValue2);
  Serial.println(variacao);

  //manda mensagem de ola uma vez
  if(mensagem==1){
    sendWhatsapp("Ola%20aqui%20eh%20o%20BabyAlarm.%20Em%20breve%20mandarei%20a%20temperatura%20do%20seu%20bebe,%20se%20ele%20chorar%20eu%20aviso%20tambem.");
    mensagem=0;
  }
    
  //se temperatura baixa, avisa
  if(temperatureC < 34.9-(34.9*0.1) && millis()>(1000*60*60)){
    sendWhatsapp("O%20bebe%20esta%20com%20a%20temperatura%20baixa!!");
    sendWhatsapp(tempTXT);
  }else if(temperatureC > 36.9+(36.9*0.1) && millis()>(1000*60*60)){
    sendWhatsapp("O%20bebe%20esta%20com%20a%20temperatura%20alta!!");
    sendWhatsapp(tempTXT);
  }
  
  // Verifica se bebe esta chorando ha um tempo(aprox 1 minuto c/ delay de codigo)
  if (variacao>1 || variacao<-1)  {
    cont = cont+1;
    if (cont>=4){
      // BEBE CHORANDO
      sendWhatsapp("O%20seu%20bebe%20esta%20chorando.");
      cont = 0;
    }
  }
 
  
  delay(1000);  
  }

}
