//Agregamos las librerias necesarias
#include <EEPROM.h>
#include <SPI.h>
#include <RFID.h> 
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
//Finger Print
int getFingerprintIDez();
SoftwareSerial mySerial(10, 11); //10-11 68VERDE 69AZUL 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
// Inicializamos las variables del RFID
#define SS_PIN 8 // pin SDA hacia el pin 10
#define RST_PIN 33 // pin RST hacia el pin 9
RFID rfid(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd( 0x27, 2,   1,  0,  4,  5,  6,  7, 3, POSITIVE );
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;
String modulo = "1";
String codigo = "RB1001";
///Keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {54, 55, 56, 57}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {58,59,60,61}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
int ledRojo = 15;
int ledAzul = 16;
int ledVerde = 14;
/////variables para lectura serial
String Msj[5];
int mensaje = 0;
boolean stringComplete = false;  // whether the string is complete
int lcdAction = 0;
//// Variables
String FingerPrint = "OFF";
int clave = 1;
int Col = 8;
char password[6];
char dni[10];
int pass_master, dni_user = 0;
int N;
char key;
long tiempoRFID, TiempoCheck, TiempoEnvio, timeA, timeB = 0;
int menuadm = 0;
int menubio = 0;
int menurfid = 0;
int fp_detec = 0;
int rfid_detec = 0;
int conextion = 0;
int connection = 0;
void setup() { 
  pinMode(ledRojo, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  digitalWrite(ledAzul, HIGH);
  Serial.begin(9600); //Establecemos una velocidad de 9600 baudios
  SPI.begin(); 
  lcd.begin(20, 4);
  RGB(1,0,0);
  // Print a message to the LCD.
  rfid.init();
  finger.begin(57600);  
  if (finger.verifyPassword()) {
    Serial.print("FP-ON");
    FingerPrint = "ON";
  } else {
    Serial.print("FP-OFF");
  }
  Serial.print("*");
  delay(50);
  Serial.print("CHECK-");
  Serial.print(modulo);        
    Serial.print('-');    
    Serial.print(codigo);
    Serial.print("*");
  
}
 ////////////////////////////********************************************LOOP PRINCIPAL************************\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
void loop() {
  if((millis() - TiempoCheck) > 10000){
    Serial.print("CHECK-");
    Serial.print(modulo);        
    Serial.print('-');    
    Serial.print(codigo);
    Serial.print("*");
    TiempoCheck = millis();
  }
  if((millis() - TiempoEnvio) > 15000){
    lcd.clear();
    TiempoEnvio = millis();
    connection = 0;
    RGB(1,0,0);
  }
  if(connection == 1){
    if(lcdAction == 0 && clave == 1){
      lcd.setCursor(4,0);
      lcd.print("PASS-CONTROL");
      lcd.setCursor(13,3);
      lcd.print("MENU[D]");
    }
    char key = customKeypad.getKey();  
    if (key != NO_KEY ){    
      lcdAction = 1;
      timeA = millis();      
      if((key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') && clave <= 4){          
        password[clave] = key;
        lcd.setCursor(Col, 2);       
        lcd.write("*");
        Col++;
        clave++;
        if(clave > 1){
          lcd.setCursor(11,3);
          lcd.write("BORRAR[B]");
        }
        if(clave == 5){
          lcd.setCursor(0,3);
          lcd.write("ACEPTAR[A]");
        }
      }else if(key == 'B' && clave > 1 ){ 
        password[clave] = ' ';
        Col--;
        clave--;
        lcd.setCursor(Col, 2);
        lcd.write(" ");     
        if(clave == 1){
          lcdAction = 0;
          lcd.clear();     
        }else if(clave > 1 && clave < 5){
          lcd.setCursor(0,3);
          lcd.write("          ");
        }
      }else if(key == 'A' && clave == 5){
        clave = 1;
        Col=8;
        Serial.print("USER-");
        Serial.print(password[1]);
        Serial.print(password[2]);
        Serial.print(password[3]);
        Serial.print(password[4]);
        Serial.print("*");
        Waiting();
      }else if(key == 'D' && clave == 1) {
         pass_Master();
      }
    }else if((millis() - timeA) > 10000 && clave != 1){
      clave = 1;
      lcdAction = 0;
      Col=8;
      lcd.clear();
      lcd.setCursor(3,1);
      lcd.write("TIEMPO SUPERADO");
      delay(1200);
      lcd.clear();
    }
    if((millis() - tiempoRFID) > 1000){
      serNum0 = 0;
      serNum1 = 0;
      serNum2 = 0;
      serNum3 = 0;
      serNum4 = 0;    
    }
    if(lcdAction == 0){
      getFingerprintIDez();
      delay(10);  
      read_RFID();
      rfid.halt();
    }
  }else{
    lcd.setCursor(4,0);
    lcd.print("SIN CONEXION");    
  }

}
////////////////////////////********************************************FUNCIONES ************************\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void Waiting(){  
  for(int i=0; i<4; i++){
      Msj[i] = "";
    }
  lcdAction = 2;
  conextion = 0;
  timeB = millis();
  lcd.clear();
  lcd.setCursor(3,1);
  lcd.print("PROCESANDO...");
  while((millis() - timeB) < 8000 && lcdAction == 2){
     if(Serial.available()) {
        serialEvent();
        conextion = 1;
     }
  }
  if(conextion == 0){
    lcd.clear();
    lcd.setCursor(3,1);
    lcd.write("TIEMPO SUPERADO");
    lcd.setCursor(4,2);
    lcd.write("SIN CONEXION");
    delay(1200);
    lcd.clear();
  }
  lcdAction = 0;
  lcd.clear();
}

void pass_Master(){  
  pass_master = 1;
  Col = 8;
  lcd.clear();
  timeA = millis();
  while(pass_master == 1 && (millis() - timeA) < 10000){    
    lcd.setCursor(1,0);
    lcd.write("MENU ADMINISTRADOR");
    lcd.setCursor(4,1);
    lcd.write("CLAVE MAESTRA");
    lcd.setCursor(0,3);
    lcd.write("[A]ACEPTAR [B]BORRAR");
    key = customKeypad.getKey() ; 
    if (key != NO_KEY) {
      timeA = millis();  
      if(key == 'B' && clave > 1 ){ 
        password[clave] = ' ';
        Col--;
        clave--;
        lcd.setCursor(Col, 2);
        lcd.write(" ");          
      }else if((key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') && clave <= 4){          
        password[clave] = key;
        lcd.setCursor(Col, 2);       
        lcd.write("*");
        Col++;
        clave++;
      }else if(key == 'A' && clave == 5){
        pass_master = 0;   
        clave = 1;
        Col=8;
        lcd.clear(); 
        Serial.print("ADMIN-");
        Serial.print(modulo);        
        Serial.print('-');    
        Serial.print(codigo);
        Serial.print('-');    
        Serial.print(password[1]);
        Serial.print(password[2]);
        Serial.print(password[3]);
        Serial.print(password[4]);
        Serial.print("*");
        Waiting();              
      }
    }    
  } 
  lcd.clear();
  if(pass_master == 1){
    pass_master=0;
    lcd.setCursor(3,1);
    lcd.write("TIEMPO SUPERADO");
    delay(1200);
    lcd.clear();
    lcdAction = 0;
  }
}
void menu_Administrador(){
  menuadm=1;
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("MENU ADMINISTRADOR");
  lcd.setCursor(0,2);
  lcd.print("[1]-AGREGAR USUARIO");
  lcd.setCursor(0,3);
  lcd.print("[2]-     [0]-VOLVER");
  timeA = millis();
  while(menuadm!=0 && (millis() - timeA) < 10000){
    if(Serial.available()){
      serialEvent();
      break;
    }
    key = customKeypad.getKey() ; 
    if (key != NO_KEY) {  
      timeA = millis();
      if(key == '0'){
        lcd.clear(); 
        menuadm=0;
        break;
      }else if(key == '1'){
        lcd.clear();
        verificar_usuario();
      }
    } 
  }
  lcdAction = 0;
  
  menubio=0;
  menuadm=0;
  lcd.clear();  
}
void verificar_usuario(){  
  dni_user = 1;
  Col = 5;
  clave = 1;
  lcd.clear();
  timeA = millis();
  while(dni_user == 1 && (millis() - timeA) < 10000){    
    lcd.setCursor(2,0);
    lcd.write("INGRESE EL SERIAL");
    lcd.setCursor(4,1);
    lcd.write("DE 9 DIGITOS");
    lcd.setCursor(0,3);
    lcd.write("[A]ACEPTAR [B]BORRAR");
    key = customKeypad.getKey() ; 
    if (key != NO_KEY) {
      timeA = millis();  
      if(key == 'B' && clave > 1 ){ 
        dni[clave] = ' ';
        Col--;
        clave--;
        lcd.setCursor(Col, 2);
        lcd.write(" ");          
      }else if((key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') && clave <= 9){          
        dni[clave] = key;
        lcd.setCursor(Col, 2);       
        lcd.write(key);
        Col++;
        clave++;
      }else if(key == 'A' && clave == 10){
        clave = 1;
        Col=5;
        lcd.clear(); 
        Serial.print("ADDUSER-");
        Serial.print(modulo);        
        Serial.print("-"); 
        for(int p=1;p<=9;p++){
           Serial.print(dni[p]);
        }
        Serial.print("*");
        Waiting();              
      }
    }    
  } 
  lcd.clear();
  if(dni_user == 1){
    dni_user=0;
    lcd.setCursor(3,1);
    lcd.write("TIEMPO SUPERADO");
    delay(1200);
    lcd.clear();
    lcdAction = 0;
  }
}
// sin usosssssssssssssssssssssss
void agregar_Acceso(String idUser, String id_bio){
  menuadm=1;
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("AGREGAR ACCESO");
    lcd.setCursor(0,1);
    lcd.print("[1]-BIOMETRIA");
    lcd.setCursor(0,2);
    lcd.print("[2]-RFID ");
  lcd.setCursor(0,3);
  lcd.print("         [0]-VOLVER");
  delay(2000);
  timeA = millis();
  while(menuadm!=0 && (millis() - timeA) < 10000){
    if(Serial.available()){
      serialEvent();
      break;
    }
    key = customKeypad.getKey() ; 
    if (key != NO_KEY) {  
      timeA = millis();
      if(key == '0'){
        lcd.clear(); 
        menuadm=0;
        menu_Administrador();
      }else if(key == '2' ){
        lcd.clear();
        dni_user = 0;
        RFID_ENROLL(idUser);
        menurfid=0;
        menu_Administrador();
      }else if(key == '1' ){
        dni_user = 0;
        lcd.clear();
        fp_detec = 0;
        timeB = millis();
        while (!getFingerprintEnroll(id_bio.toInt(), idUser) && (millis() - timeB) < 15000);
        delay(10);
        menu_Administrador();
      }
    } 
  }
  menuadm=0;
  lcd.clear();  
  
}

