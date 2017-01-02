uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");Serial.print("*");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");Serial.print("*");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");Serial.print("*");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");Serial.print("*");
      return p;
    default:
      Serial.println("Unknown error");Serial.print("*");
      return p;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");Serial.print("*");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");Serial.print("*");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");Serial.print("*");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");Serial.print("*");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");Serial.print("*");
      return p;
    default:
      Serial.println("Unknown error");Serial.print("*");
      return p;
  }  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    Serial.print("*");
    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("USUARIO");
    lcd.setCursor(2,2);
    lcd.print("NO IDENTIFICADO");
    delay(1000);
    lcd.clear();
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    Serial.print("*");
    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("USUARIO");
    lcd.setCursor(2,2);
    lcd.print("NO IDENTIFICADO");
    delay(1000);
    lcd.clear();
    return p;
  } else {
    Serial.println("Unknown error");
    Serial.print("*");
    return p;
  }     
  // found a match!
}
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;  
  // found a match!
  Serial.print("BIOMETRIA-USER-"); 
  Serial.print(finger.fingerID);
  Serial.print("-");
  Serial.print(modulo);
  Serial.print("*");
  Waiting();
  lcd.clear();  
  return finger.fingerID; 
}
/////////////////////*****************************Enroll Finger Print**************************\\\\\\\\\\\\\\\\\\\\\\\\\\
uint8_t getFingerprintEnroll(int id, String idUser) {
  int p = -1;
  lcd.setCursor(0,0);
  lcd.print("SITUE SU DEDO INDICE");
  lcd.setCursor(7,1);
  lcd.print("DERECHO");
  lcd.setCursor(4,2);
  lcd.print("EN EL LECTOR");
  int Finger = 0;
  timeA = millis();
  while (p != FINGERPRINT_OK && (millis() - timeA) < 12000) {
    p = finger.getImage();
    switch (p) {
      
      case FINGERPRINT_OK:
        //Serial.println("Image taken");
        Finger = 1;
        lcd.clear();
        lcd.setCursor(3,1);
        lcd.print("TOMANDO IMAGEN");
        break;
      case FINGERPRINT_NOFINGER:
        //Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        //Serial.println("Communication error");
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("ERROR COMUNICACION");
        break;
      case FINGERPRINT_IMAGEFAIL:
        //Serial.println("Imaging error");
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("ERROR DE IMAGEN");
        break;
      default:
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("ERROR DESCONOCIDO");
        //Serial.println("Unknown error");
        break;
    }
  }
  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("IMAGEN CONVERTIDA");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(3,1);
      lcd.print("IMAGEN BORROSA");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("ERROR COMUNICACION");
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PODRIA NO ENCONTRAR CARACTERISTICA DE UN DEDO");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PODRIA NO ENCONTRAR");
      lcd.setCursor(0,1);
      lcd.print("CARACTERISTICA DE UN");
      lcd.setCursor(8,2);
      lcd.print("DEDO");
      return p;
    default:
      //Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("ERROR DESCONOCIDO");
      return p;
  }  
  //Serial.println("Remove finger");
  if(Finger == 1){
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("RETIRE SU DEDO");
    delay(2000);
  }
  p = 0;
  while (p != FINGERPRINT_NOFINGER && Finger == 1) {
    p = finger.getImage();
  }
  p = -1;
  if(Finger == 1){
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("VUELVA A COLOCAR");
    lcd.setCursor(3,2);
    lcd.print("EL MISMO DEDO");
  }
  //Serial.println("Place same finger again");
  timeA = millis();
  while (p != FINGERPRINT_OK && Finger == 1 && (millis() - timeA) < 12000) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(3,1);
      lcd.print("TOMANDO IMAGEN");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("ERROR COMUNICACION");delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("ERROR DE IMAGEN");delay(1000);
      break;
    default:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("ERROR DESCONOCIDO");delay(1000);
      //Serial.println("Unknown error");
      break;
    }
  }
  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
     case FINGERPRINT_OK:
      //Serial.println("Image converted");
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("IMAGEN CONVERTIDA");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(3,1);
      lcd.print("IMAGEN BORROSA");delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("ERROR COMUNICACION");delay(1000);
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PODRIA NO ENCONTRAR");
      lcd.setCursor(0,1);
      lcd.print("CARACTERISTICA DE UN");
      lcd.setCursor(8,2);
      lcd.print("DEDO");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PODRIA NO ENCONTRAR");
      lcd.setCursor(0,1);
      lcd.print("CARACTERISTICA DE UN");
      lcd.setCursor(8,2);
      lcd.print("DEDO");delay(1000);
      return p;
    default:
      //Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("ERROR DESCONOCIDO");delay(1000);
      return p;
  }  
  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Prints matched!");
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("DEDOS COINCIDEN");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("ERROR COMUNICACION");delay(1000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    //Serial.println("Fingerprints did not match");
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("DEDOS NO COINCIDEN");delay(1000);
    return p;
  } else {
    //Serial.println("Unknown error");delay(1000);
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("ERROR DESCONOCIDO");delay(1000);
    return p;
  }     
  //Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    fp_detec = 1;
    Serial.print("BIOMETRIA-ADMIN-");
    Serial.print(id);
    Serial.print("-");
    Serial.print(modulo);
    Serial.print("-");
    Serial.print(idUser);
    Serial.print("*");
    lcd.clear();
    lcd.setCursor(4,1);
    lcd.print("ALMACENANDO"); 
    delay(1000);
    Waiting();
    lcdAction = 2;
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("ERROR COMUNICACION");delay(1000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not store in that location");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("PODRIA NO GUARDARSE");delay(1000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("ERROR EN MEMORIA FLASH");delay(1000);
    return p;
  } else {
    //Serial.println("Unknown error");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("ERROR DESCONOCIDO");delay(1000);
    return p;
  }  
  delay(10);
}
/////////////////////*****************************Delete Finger Print**************************\\\\\\\\\\\\\\\\\\\\\\\\\\\'
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;  
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Deleted!");
    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("BORRANDO");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("PODRIA NO BORRAR");
    //Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("ERROR EN MEMORIA FLASH");
    return p;
  } else {
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("ERROR DESCONOCIDO");
    //Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
  lcd.clear();
} 

