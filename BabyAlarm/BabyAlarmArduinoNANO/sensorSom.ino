// select the input pin for the potentiometer
int sensorValue =0;

void setup(){
  pinMode(A2, INPUT); //DEFINE O PINO COMO ENTRADA
  Serial.begin(9600);
}

void loop(){
  sensorValue = analogRead(2);
  if (digitalRead(sensorValue) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
        Serial.print(sensorValue);
        Serial.print("\n");
  }
}
