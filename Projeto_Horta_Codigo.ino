// Definir Bibliotecas
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Frenquencia de notas musical para buzzer
#define NOTE_C6  1047
#define NOTE_C3  131
#define NOTE_G3  196

// Definição dos Sensores
int SensorDeTemperaturaPin = A0;
int BuzzerPin = 7;
int SensorDeUmidadePin = A1;
int SensorDeNivelPin = 3;
int BombaAguaPin = 5;
// Difinição dos LEDs
int SoloSecoLEDPin = 13;       // LED Vermelho
int SoloUmidoLEDPin = 11;      // LED Verde
int SoloEncharcadoLEDPin = 12; // LED Amarelo
int BombaAguaLEDPin = 10;      // LED Verde

//Declarar Variáveis
int SensorDeNivelValue;       // Armazena os valores do sensor de nível
int SensorDeUmidadeValue;     // Armazena os valores do sensor de humidade
int buff_1;
int buff_2;
int buff_3;
int SensorDeNivelAvg;         // Armazena os valores do sensor de nível
int SensorDeUmidadeAvg;       // Armazena os valores do sensor de humidade
int SensorDeTemperaturaAvg;   // Armazena os valores do sensor de temperatura
int BUFFER_SIZE = 10;         // Quantidade de leituras dos sensores para cálculo da média (Avg)
int Val_Umidade_Map;
int Val_Nivel_Map;
int Val_Temperatura_Map;
int Situacao_Bomba;
//const float CELSIUS_BASE = 0.4887585532746823069403714565; //Base de conversão para Graus Celsius
String texto_php = String();



//Mensagens do Sistema
const char *string_table[] =
{   
  "     Welcome!" ,
  "Reservatorio com nível baixo",
  "     Solo Seco",
  "     Solo Umido",
  "     Solo Encharcado",
  "A bomba da água está Ligada",
  "     DL MODDY",
  "   Sistema de Irrigação",
  "     Por favor...!"
};

//Definição de Objetos
//LiquidCrystal_I2C lcd(0x3F,20,4);

void setup(){
 // Inicialização do Serial
  Serial.begin(9600);
  
  // Inicialização do LCD 
 /* lcd.init();          
  lcd.backlight();     // Com Luz
  lcd.clear();         // Limpar tela
  */
  // Inicialização do Wire 
  Wire.begin();
  
  // Inicializando pinos do Arduino
  pinMode(BuzzerPin, OUTPUT);
  pinMode(SoloEncharcadoLEDPin, OUTPUT);
  pinMode(SoloUmidoLEDPin,OUTPUT);
  pinMode(SoloSecoLEDPin,OUTPUT);
  pinMode(BombaAguaLEDPin,OUTPUT);
  pinMode(BombaAguaPin,OUTPUT);
  
   // Iniciar mensagens no LCD
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(string_table[6]); 
  lcd.setCursor(0,1);
  lcd.print(string_table[7]); 
  lcd.setCursor(0,3);
  lcd.print(string_table[0]); */
  delay(9900); 
}

void loop(){
  // Lê os sensores
  SensorDeNivelValue = analogRead(SensorDeNivelPin);
	SensorDeUmidadeValue = analogRead(SensorDeUmidadePin);
	buff_1     = 0;
	buff_2     = 0; 
        buff_3     = 0;
	for (int i = 0; i < BUFFER_SIZE; i++){
		buff_1+= analogRead(SensorDeNivelPin);
		buff_2+= analogRead(SensorDeUmidadePin);
                buff_3+= (5.0 * analogRead(SensorDeTemperaturaPin) * 100.0) / 1024.0;
	}
	SensorDeNivelAvg       = (buff_1/BUFFER_SIZE);
	SensorDeUmidadeAvg     = (buff_2/BUFFER_SIZE);
        SensorDeTemperaturaAvg = (buff_3/BUFFER_SIZE);

        Val_Nivel_Map       = map(SensorDeNivelAvg, 0, 1023, 0, 100);
        Val_Umidade_Map     = map(SensorDeUmidadeAvg, 0, 1023, 0, 100); 
  
  // Se nível da água está baixo : Ligar o alarme de nível baixo e depois desligar a bomba da água
  if(SensorDeNivelValue > 600){
    // Mensagens para o LCD
    //lcd.clear();
    //lcd.setCursor(0,3);
    //lcd.print(string_table[1]); 
    // Tocar o som do Alarme
    for(int i=0;i<2;i++){
      tone(BuzzerPin, NOTE_G3, 200);
      delay(200);
      tone(BuzeerPin, NOTE_C3, 200);
      delay(200);
      noTone(BuzzerPin);
    } 
    //desligar a Bomba
      digitalWrite(BombaAguaPin,LOW);
      digitalWrite(BombaAguaLEDPin,LOW);
  }
  
  // Se o solo estiver seco, tocar o alarme
  if(SensorDeUmidadeValue >= 700){
    // Em caso de solo seco:
    // Mensagens para o LCD
    //lcd.clear();
    //lcd.setCursor(0,3);
    //lcd.print(string_table[2]);
    // Ligar o LED
    digitalWrite(SoloSecoLEDPin,HIGH);
    digitalWrite(SoloUmidoLEDPin,LOW);
    digitalWrite(SoloEncharcadoLEDPin,LOW);
    // Toca o som o Alarme
    tone(BuzzerPin, NOTE_C6, 100);
    delay(250);
    noTone(BuzzerPin);
  }
  
  //Se o Solo estiver Encharcado, mostrar no lcd
  if((SensorDeUmidadeValue < 700) && (SensorDeUmidadeValue >=300)){
  //Messagem para o LCD
  //lcd.clear();
  //lcd.setCursor(0,3);
  //lcd.print(string_table[4]);
  //Acender os LEDs
    digitalWrite(SoloSecoEDPin,LOW);
    digitalWrite(SoloUmdioLEDPin,LOW);
    digitalWrite(SoloEncharcadoLEDPin,HIGH);
    delay(100);
  }
  
  //Verificar se o solo está seco 
  SensorDeUumidadeValue = analogRead(SensorDeUmidadePin);
  //Se o solo estiver seco, ligue a bomba durante 1 minuto
  if(SensorDeUmidadeValue >= 700){
  //Mostrar no LCD
     // lcd.clear();
     // lcd.setCursor(0,1);
      //lcd.print(string_table[5]);
   //Ligar a bomba   
      digitalWrite(BombaAguaPin,HIGH);
      digitalWrite(BombaAguaLEDPin,HIGH);
      delay(10000);
  }
 
  //Verificar se o solo continua seco 
  SensorDeUumidadeValue = analogRead(SensorDeUmidadePin);
  
  // Se o solo estiver seco, continua irrigando. Senão desligue a bomba
   if(SensorDeUmidadeValue >= 700){
      digitalWrite(BombaAguaPin,HIGH);
      digitalWrite(BombaAguaLEDPin,HIGH);
      delay(10500);
   }else{
   // Desligar a bomba   
      digitalWrite(BombaAguaPin,LOW);
      digitalWrite(BombaAguaLEDPin,LOW);
  }
}
