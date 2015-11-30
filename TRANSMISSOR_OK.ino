#include <VirtualWire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 3
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
// Armazena temperaturas minima e maxima
float tempMin = 999;
float tempMax = 0;
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
// Declarando pinos para transmissao (RF TX 433 MHz)
const int SENSOR = 3;//A0;
const int TX = 10;
 
void setup(void){
  Serial.begin(9600);
  sensors.begin();
  // Localiza e mostra enderecos dos sensores
  Serial.println("Localizando sensores DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  vw_set_tx_pin(TX);
  vw_set_ptt_inverted(true); // Requerido para DR3100
  vw_setup(2000); // Bits Parada
}

void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
 
void loop(){
  // Le a informacao do sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);

  delay(3000);
  
  //Lendo e armazenando o valor da temperatura
  //Convertndo int para char
  int tempC1 = (int)tempC;
  int tempC2 = ((tempC - tempC1) * 100) ; // Pegar duas casas Decimais
  
  char SensorCharMsg[15];
  //itoa(tempC, SensorCharMsg,10);
  sprintf(SensorCharMsg, "%i.%i", tempC1,tempC2);
  Serial.println(SensorCharMsg);
  vw_send((uint8_t *)SensorCharMsg, strlen(SensorCharMsg));
  vw_wait_tx(); // Espera o envio da informacao
}
