# ESP3266 WiFi Phishing
## Notas
Esse repositório possuí apenas propósito educacional, use e teste apenas com a sua conexão. Não assumo qualquer responsabilidade pela forma em que o conteúdo contido aqui será utilizada.<br>
Além disso, é apenas uma tradução e adaptação do **ESP8266-Captive-Portal**, disponível em: https://github.com/adamff1/ESP8266-Captive-Portal

## Sobre o projeto
- Potal WiFi para coletar informaçẽos com NodeMCU(microcontrolador) ESP826 com DNS falsificado.
- O LED embutido piscará 5 vezes quando uma senha for postada.
- **Aviso!** Suas senhas salvas **não** desaparecerão quando você reiniciar/desligar o ESP3266.
- Se você quiser ver as senhas armazenadas, vá para "172.0.0.1/pass". Para alterar o SSID, vá para "172.0.0.1/ssid"
- Há uma V2.0 com (Login falso) disponível em: https://github.com/125K/ESP8266_WiFi_Captive_Portal_2.0

# Demonstração

<a target="_blank" href="https://youtu.be/v4-5oX3RG94"><img width="700px" src="https://raw.githubusercontent.com/125K/ESP8266_WiFi_Captive_Portal/master/src/thumbnail.png"></a>

# Telas

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

# Instalação (ESP8266 Flasher - Modo fácil)

1. Baixe: <a href="https://github.com/nodemcu/nodemcu-flasher"><b>ESP8266 Flasher</b></a>. Testei para o ESP3266 e funcionou tranquilamente!

2. Beixe o arquivo <b><a href="https://github.com/125K/ESP8266_WiFi_Captive_Portal_2.0/releases/download/2.1/release.bin"></b></a> file.

3. Abra o ESP8266 Flasher e selecione a porta Node MCU

<img width="80%" src="https://raw.githubusercontent.com/BlueArduino20/ESP8266_WiFi_Captive_Portal_2.0/master/src/1_port_selection.PNG">

4. Em seguida, configure a aba e selecione o arquivo .bin que você baixou.

<img width="80%" src="https://raw.githubusercontent.com/BlueArduino20/ESP8266_WiFi_Captive_Portal_2.0/master/src/2_file_selection.png">

5. Depois, volte para a aba anterior e precione o botão "Flash"

6. Tudo pronto, podemos testar!

# Instalação do Arduino IDE

1. Abra o <a href="https://www.arduino.cc/en/main/software">Arduino IDE</a> e vá na aba "File -> Preferences -> Boards Manager URLs" and paste the following link:
``https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json``
2. Vá em "Ferramentas -> Placa -> Gerenciador de Placas", procure "esp3266" e instale o esp3266
3. Vá em "Ferramentas -> Placa" e selecione sua placa "ESP32 Dev Module"
4. Baixe e abra o sketch "<a href="https://github.com/BlueArduino20/ESP8266_WiFi_Captive_Portal_2.0/blob/master/ESP8266_WiFi_Captive_Portal_2.0.ino"><b>ESP8266_WiFi_Captive_Portal_2.0.ino</b></a>"
5. Você pode mudar o nome do SSID e os textos da página como título, subtítulo, texto do corpo...
6. Upload o código na sua placa.
7. Pronto!
