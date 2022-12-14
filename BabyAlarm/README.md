# BabyAlarm
Projeto para a máteria de Computação Física, ministrada pelo @FNakano na Universidade de Sâo Paulo. O projeto é um dispositivo de monitoramento de bebês.
Documentação com código Fonte

# Como o dispositivo funciona
  <table>
  <tr>
    <th>172.0.0.1/index</th>
    <th>172.0.0.1/post</th> 
    <th>172.0.0.1/creds</th>
  </tr>
  <tr>
    <td>Essa é a página principal. Aqui o usuário escreve a senha dele e manda.</td>
    <td>Aqui a página de validação, onde o usuário vai após mandar a senha.</td>
    <td>A página de credenciais mostra os dados enviados ao atacante.</td>
  <tr>
    <td><img src="https://raw.githubusercontent.com/125K/ESP8266_WiFi_Captive_Portal_2.0/master/src/1_index.png" title="Index"></td>
    <td><img src="https://raw.githubusercontent.com/BlueArduino20/ESP8266_WiFi_Captive_Portal_2.0/master/src/2_post.png" title="Post"></td>
    <td><img src="https://raw.githubusercontent.com/BlueArduino20/ESP8266_WiFi_Captive_Portal_2.0/master/src/3_creds.png" title="Creds"></td>
  </tr>
</table>

# Materiais 
1. ESP8266: comunicação via Wi-Fi, responsável pela emissão do sinal do bot do Whatsapp, criando um canal de comunicação entre as informações do arduino e o usuário. Custa cerca de R$40,00.</br>
2. DS18B20: sensor de temperatura digital, que é capaz de medir temperaturas entre -55° até 125°C, em meio seco, úmido ou submerso, não necessitando de um componente externo, além de já apresentar os valores em graus celsius. Custa cerca de R$20,00.</br>
3. KY038: sensor capaz de medir as variações de som em um ambiente a parti de um microfone no módulo. Custa cerca de R$15,00.</br>

**Valor estimado em materiais para o projeto: R$75,00**

# Descrever o circuito pela lista de conexões
Inicialmente o ESP8266 emite um sinal de hotspot para conexão, para acessar a rede é necessário fazer login. No formulário de acesso, é solicitado dados como o SSID, Senha da rede Wifi, Número de celular e APIKEY(chave usada para o bot). Quando a placa recebe esses daddos, ela tenta fazer a conexão, a partir daí o resto do código é ativado. Nesse momento os sensores começam a operar, medindo a temperatura pela porta digital 2(D2) e o som ambiente pela porta analógica 0(A0), ambos com um VCC de 3,3V. Caso a temperatura exceda o esperado ou caso o sensor de som capte um ruído muito alto uma sequência de vezes é feita uma requisição via Wifi para mandar mensagem pro whatsapp indicado.

A requesição passa pelo URL o texto em questão que será enviado para o bot. Enquanto houver conexão a leitura será feita, isto é, enquanto a função  `WiFi.isConnected()` retornar TRUE. Caso a conexão se perca ou o dispositivo desligue(por falta de alimentação), haverá necessidade de relogar na rede. Ademais, as leituras também cessarão, pois não há como enviar os dados.

# Foto do circuito

<table>
  <tr>
    <td>Sensor térmico DS18B20</td>
    <td><img src="https://raw.githubusercontent.com/SystemGuuh/Computacao-Fisica/main/BabyAlarm/imgs/cicuito2.jpg" title="SensorTemp"></td>
    <td>Após realização a ligação em paralelo destes, conectamos os pinos VCC no pino 3.3V do ESP, o pino GND no GND e os pinos de sinal no pino Digital 2.</td>
  </tr>
  <tr>
    <td>Sensor de som KY038</td>
    <td><img src="https://raw.githubusercontent.com/SystemGuuh/Computacao-Fisica/main/BabyAlarm/imgs/sensorSom.jpg" title="SensorSom"></td>
    <td>Para o uso desse dispositivo conectamos os pinos VCC no pino 3.3V do ESP, o pino GND no GND e os pinos de sinal no pino Analógico 0.</td>

  <tr>
    <td>ESP8266</td>
    <td><img src="https://raw.githubusercontent.com/SystemGuuh/Computacao-Fisica/main/BabyAlarm/imgs/ESP.jpg" title="ESP"></td>
    <td>
O módulo WiFi ESP8266 é um SOC com protocolo TCP/IP integrado que consegue dar a qualquer microcontrolador acesso a sua rede WiFi. O ESP8266 é capaz tanto de hospedar uma aplicação quanto descarregar todas as funções de redes WiFi a partir de outro processador de aplicação.</td>
  </tr>
</table>

# Como usar o programa

Para execurtar o `BabyAlarmESP8266.ino` no ArduinoIDE é necessário adicionar o pacote de placas do módulo ESP8266. Há um tutorial disponível de como adicionar o pacote,
<a href="https://www.robocore.net/tutoriais/programando-o-esp8266-pela-arduino-ide">veja aqui</a>. No `BabyAlarmESP8266.ino` são definidas diversas funções que carregam a página em HTML e CSS como `header()`, ou que enviam mensagens no whatsapp em `sendWhatsapp()`.

# Público e Necessidades

Nosso público-alvo são mães e pais de primeira viagem que se sentem inseguros com os cuidados de seu primeiro filho, desenvolvendo  alto nível de stress, sobrecarga e ansiedade. Estudos realizados pela OnePoll e pela Universidade Estadual de Ohio indicam que mães de primeira viagem chegam a passar mais de 1.400 horas preocupadas com a saúde do seu bebê. Dados também indicam que 68% das mães e 42% dos pais sofrem com burnout.

>Fonte: revistacrescer.globo.com

# Objetivos

Nosso principal objetivo com esse projeto seria unificar as diferentes funções em um único dispositivo, projetar um dispositivo que possa ser usado conforme o crescimento da criança, barateando e aumentando sua acessibilidade para a população, além, é claro, de reduzir a sobrecarga, preocupação e estresse dos pais.

# Resultados esperados

Com a elaboração desse projeto, esperamos que o produto se torne acessível e mais famílias possam comprá-lo, além de ser usado ao longo do crescimento do bebê/;criança, dessa forma, fazendo com que os pais e mães fiquem mais tranquilos com a saúde do seu bebê, consequentemente diminuindo o tempo de preocupação. Buscamos um dispositivo de fácil monitoramento, com consulta rápida e precisa, além de um alerta para casos de risco.

# Pesquisa de concorrentes

Esses são alguns wearables de referência no mercado que são relacionados com essas intervenções em bebês:</br>
Baby Hug: uma espécie de botton que é poscionado na roupa do bebê, verifica respiração, movimentos bruscos, barulhos, quedas e distanciamento do aparelho. Os dados coletados são enviados para os pais através de um aplicativo. Custa cerca de R$700. </br>
Lineable: pulseira de silicone com sistema de GPS,com bateria Bluetooth. Custa cerca de US$10.</br>

1. **Sproutling:** fica no tornozelo do bebê, detecta padrões de sono e avisa via celular se o bebê está rolando, dormindo ou acordado, existem até previsões que informam quando o bebê pode acordar e fornece conselhos personalizados com base nos dados que obtém. Custa cerca de R$300.</br>
2. **Mimo Smart Baby Monitor:** esse dispositivo se conecta a um macacão específico e monitora o sono do bebe, com o celular é possível monitorar a respiração do bebê, posição ao dormir, quando acorda ou adormece e temperatura. Custa cerca de US$150.</br>
3. **Snuza:** conecta-se na fralda do bebê e dispara um alarme caso detectar que o bebe não está respirando, também detecta temperatura, posição do corpo e quedas. Custa cerca de R$400.</br>
4. **Moonbaby:** espécie de botão que se encaixa em qualquer roupa do bebê, monitora respiração, rolamentos e quedas, podendo ser usado conforme o crescimento do bebê. Custa cerca de US$75.</br>
5. **Pacif-i:** “chupeta inteligente” que monitora a temperatura do bebê enquanto a chupeta estiver na boca. Custa cerca de US$40.</br>
6. **Neebo:** usado no pulso como uma pulseira, monitora frequência cardíaca,  filtra outros ruídos de fundo para que você ouça apenas o seu bebê, possui um sistema de alerta para emergências e mede níveis de oxigênio. Custa cerca de US$300.</br>

> https://inovasocial.com.br/tecnologias-sociais/wearables-para-criancas-mais-funcionalidade-para-rontina-com-os-pequenos/ 
> https://www.romper.com/p/7-wearables-for-keeping-your-baby-alive-so-everyone-can-get-some-sleep-7524165 

# Explicando um pouco do código

<h3>Para a montagem das páginas em HTML através de funções em C:</h3>

```C
String index() {
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action=/post method=post>" +
    "<b>Nome da rede:</b> <center><input type=text name=rede placeholder='SSID do reteador'></input></center>" +
    "<b>Senha:</b> <center><input type=password name=password placeholder='Senha do roteador'></center>" + 
    "<b>Numero de celular:</b> <center><input type=text name=cell placeholder='0000000000000'></input></center>" +
    "<b>Chave APIKEY:</b> <center><input type=text name=apikey placeholder='12345678'></input><input type=submit value=\"Enviar\"></form></center>" +
    footer();
}
```
Para a criação do CSS através de funções em C:

```C
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
  ```
Além disso, para mandar mensagem, através de uma biblioteca conseguimos usar a seguinte função, que faz uso do cliente wi-fi:

```C
void sendWhatsapp(String text) {
  WiFiClient client;
  HTTPClient https;
  String url = URL + text + "&apikey=" + YOUR_APIKEY + "&phone=" + YOUR_PHONE;
  https.begin(client, url);
  int httpCode = https.GET();
  https.end();
}
```


# Conclusão

foi possível baraterar?
possível usar ao longo do crescimento da criança?
possível usar em idosos ou pessoas doentes?
possível atribuir mais sensores e funções com mais tempo e pesquisas?
	_ qualidade de sono
	_ quedas
	_ posição
	_ acordado/dormindo
	_ rolamento
	_ oxigenação


