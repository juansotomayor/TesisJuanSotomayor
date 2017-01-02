void serialEvent() {  
  int valores = 0;
  while (Serial.available()) {// get the new byte:    
    char inChar = (char)Serial.read();     
    if(inChar == '*' ){
      mensaje++;
    }else  if (inChar == '-') {
      stringComplete = true;
      valores = mensaje;
      mensaje = 0;      
      
      break;
    }else{
      if(inChar != ' '){
        Msj[mensaje] += inChar;
      }
    }
  }
  clave == 1;
  if (stringComplete) {
    stringComplete = false; 
    if(Msj[1] == "USERSBIO"){
      for(int a=0; a<=125; a++){
        int coincidencia = 0;        
        for(int b=0; b<=valores; b++){
          if(Msj[b].toInt() == a) {
             coincidencia = 1;
          }
        }
        if(coincidencia == 0){
          Serial.print("UDELETE-");
          Serial.print(a);
          Serial.print("*");
          deleteFingerprint(a);          
        }
      }
      //deleteFingerprint(Msj[1].toInt());
      //lcdAction = 0;
    }
    if(Msj[1] == "OPEN_DOOR"){
      int col1 = Msj[2].length();
      int col2 = Msj[3].length();
      float suma = col1+col2;  
      suma = (20 - suma)/2;
      col1 = (20 - col1)/2;
      col2 = (20 - col2)/2;
      lcd.clear();
      if(suma > 0){
        lcd.setCursor(suma,1);
        lcd.print(Msj[2]);
        lcd.print(" ");
        lcd.print(Msj[3]);
      }else{
        lcd.setCursor(col1,0);
        lcd.print(Msj[2]);
        lcd.setCursor(col2,1);
        lcd.print(Msj[3]);
      }  
      lcd.setCursor(5,3);
      if(Msj[4] == "hombre"){
        lcd.print("BIENVENIDO");
      }else{
        lcd.print("BIENVENIDA");
      }      
      RGB(0, 1, 0); // rojo
      delay(2000);
      lcdAction = 0;
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "FP_DELETE" && FingerPrint == "ON"){      
      deleteFingerprint(Msj[2].toInt());
      lcdAction = 0;
    }else if(Msj[1] == "RFID_OK"){
      Serial.print("RFID-ADMIN-");
      Serial.print(Msj[2]); 
      Serial.print("-");
      Serial.print(Msj[3]); 
      Serial.print("-");
      Serial.print(Msj[4]); 
      Serial.print("*");
      Waiting(); 
    }else if(Msj[1] == "USER_ADDED"){
      Serial.print("user_Added-");
      Serial.print("*");
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.write("USUARIO ADICIONADO");
      RGB(0, 1, 0); // rojo
      delay(1200);
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "USER_EXISTING"){
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.write("USUARIO EXISTENTE");
      RGB(1, 0, 0); // rojo
      delay(1500);
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "USER_DENIED"){
      int col1 = Msj[2].length();
      int col2 = Msj[3].length();
      float suma = col1+col2;  
      suma = (20 - suma)/2;
      col1 = (20 - col1)/2;
      col2 = (20 - col2)/2;
      lcd.clear();
      if(suma > 0){
        lcd.setCursor(suma,1);
        lcd.print(Msj[2]);
        lcd.print(" ");
        lcd.print(Msj[3]);
      }else{
        lcd.setCursor(col1,0);
        lcd.print(Msj[2]);
        lcd.setCursor(col2,1);
        lcd.print(Msj[3]);
      }    
      lcd.setCursor(2,3);
      lcd.write("NO TIENES ACCESO");
      RGB(1, 0, 0); // rojo
      delay(2000);
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "USER_NONEXIST"){
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.write("USUARIO NO EXISTE");
      RGB(1, 0, 0); // rojo
      delay(1500);
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "USER_COMPLETFORM"){
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.write("INGRESE LOS DATOS");
      lcd.setCursor(1,1);
      lcd.write("DEL USUARIO EN LA");
      lcd.setCursor(5,2);
      lcd.write("PLATAFORMA");
      RGB(1, 0, 0); // rojo
      delay(3000);
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "USER_ADDSITE"){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.write("USUARIO YA TIENE");
      lcd.setCursor(0,1);
      lcd.write("LOS ACCESOS, INGRESE");
      lcd.setCursor(0,2);
      lcd.write("A LA PLATAFORMA PARA");
      lcd.setCursor(7,3);
      lcd.write("EDITAR");
      RGB(1, 0, 0); // rojo
      delay(1500);
      verificar_usuario();
    }else if(Msj[1] == "USER_ADD"){
      agregar_Acceso(Msj[2], Msj[3]);
    }else if(Msj[1] == "ADMIN"){
      if(Msj[2] == "OK"){
        menu_Administrador();        
      }else if(Msj[2] == "FAIL"){
        lcd.clear();
        lcd.setCursor(2,1);
        lcd.write("ACCESO DENEGADO");
        RGB(1, 0, 0); // rojo
        delay(1500);
        lcd.clear();
      }
      lcd.clear();
      lcdAction = 0;
    }else if(Msj[1] == "CHECK"){
      connection = 1;
      lcdAction = 0;   
      clave = 1;
      TiempoEnvio = millis();      
    }
    for(int i=0; i<5; i++){
      Msj[i] = "";
    }     
  }
  TiempoEnvio = millis();
  RGB(0, 0, 1); // azul
}
void RGB(int r, int g, int b){
  digitalWrite(ledVerde, g);
  digitalWrite(ledAzul, b);
  digitalWrite(ledRojo, r);
}

