boolean GPRScommunication() {
  // ATTACH GPRS SERVICE
  SIM7000.println(F("AT+CGATT=1"));
  bacaserial(200);

  //DEAKTIVASI GPRSPDP CONTEXT DAN TUTUP BEARER
  SIM7000.println(F("AT+CIPSHUT;+SAPBR=0,1"));
  bacaserial(1000);

  //CONNECTION TYPE = GPRS ; ATUR APN, USER, DAN PASSWORD
  delay(200);
  SIM7000.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\";+SAPBR=3,1,\"APN\",\"Telkomsel\";+SAPBR=3,1,\"USER\",\"\";+SAPBR=3,1,\"PWD\",\"\""));
  bacaserial(500);

  //OPEN BEARER
  kalimat = "";
  SIM7000.println(F("AT+SAPBR=1,1"));
  cekSerial(15000);
  delay(100);
  //debug
  //  Serial.print(F("Karakter dari sapbr=1 : "));
  //  Serial.println(karakter);

  //PARSING STRING DARI OPEN BEARER UNTUK CEK OK ATAU ERROR
  if (karakter != 'O') {
    return 0;
  }
  kalimat = "";

  //mulai task and set APN, user name, password
  SIM7000.println(F("AT+CSTT=\"Telkomsel\""));
  bacaserial(3000);

  SIM7000.println(F("AT+CIICR"));
  cekSerial(5000);
  //PARSING STRING DARI OPEN BEARER UNTUK CEK OK ATAU ERROR
  if (karakter != 'O') {
    return 0;
  }
  
  SIM7000.println(F("AT+CIFSR"));
  bacaserial(200);

  return 1;
}

void sendServer() {
  hapus();
  //TERMINATE HTTP SERVICE
  SIM7000.println(F("AT+HTTPTERM"));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();
  delay(100);

  SIM7000.println(F("AT+HTTPINIT"));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();
  delay(100);

  //SET HTTP PARAMETERS VALUE
  SIM7000.println(F("AT+HTTPPARA=\"CID\",1"));
  SIM7000.flush();
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();
  delay(100);

  // send data measurement to server
  //SET HTTP URL
  // kalimat = String();
  Serial.println(F("AT+HTTPPARA=\"URL\",\"http://www.mantisid.id/project/osh_2019/api/product/osh_data_c.php\""));
  SIM7000.println(F("AT+HTTPPARA=\"URL\",\"http://www.mantisid.id/project/osh_2019/api/product/osh_data_c.php\""));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();
  delay(300);

  // HTTP parameter - Content (content depend on related <HTTPParamTag>.Used to set the “Content-Type” field in HTTP)
  SIM7000.println(F("AT+HTTPPARA=CONTENT,application/json"));
  bacaserial(300);
  delay(200);

  dataJSON();

  delay(1000);
  //SET HTTP DATA FOR SENDING TO SERVER
  kalimat = "";
  kalimat.concat("AT+HTTPDATA=");
  kalimat.concat(String(json).length() + 1);
  kalimat.concat(",15000");

  //debug
  //  Serial.println(kalimat);
  //  Serial.flush();
  //  delay(2000);
  SIM7000.println(kalimat);
  SIM7000.flush();
  while (SIM7000.available() > 0) {
    while (SIM7000.find("DOWNLOAD") == false) {
    }
  }
  bacaserial(500);

  //SEND DATA
  Serial.println(F("KIRIM DATANYA"));
  Serial.println(json);
  Serial.flush();
  SIM7000.println(json);
  bacaserial(1000);
  Serial.flush();
  SIM7000.flush();

  //HTTP METHOD ACTION
  mulai = millis();
  Serial.println(F("AT+HTTPACTION=1"));
  SIM7000.println(F("AT+HTTPACTION=1"));
  Serial.flush();
  SIM7000.flush();
  while (SIM7000.available() > 0) {
    while (SIM7000.find("OK") == false) {
      // if (SIM7000.find("ERROR")) {
      // goto serve;
      // }
    }
  }
  Serial.flush();
  SIM7000.flush();
  indeks = '0';
  indeks1 = '0';
  kalimat = "";
  //CHECK KODE HTTPACTION
  while ((mulai + 30000) > millis()) {
    while (SIM7000.available() > 0) {
      karakter = SIM7000.read();
      kalimat += karakter;
      indeks = kalimat.indexOf(":");
      indeks1 = kalimat.length();
      if (indeks1 - indeks > 8) {
        Serial.println(F("MOVING OUT"));
        break;
      }
    }
    if (indeks1 - indeks > 8) {
      Serial.println(F("OK"));
      break;
    }
  }
  Serial.flush();
  SIM7000.flush();

  Serial.println(F("Hasil HTTPACTION:"));
  Serial.println(kalimat);
  Serial.flush();

  indeks = '0';
  indeks = kalimat.indexOf(',');
  indeks1 = kalimat.indexOf(',', indeks1 + 1);
  kode = kalimat.substring(indeks + 1, indeks1).toInt();

  Serial.print(F("kode="));
  Serial.println(kode);
  Serial.flush();

  SIM7000.println("AT+HTTPREAD");
  delay(10000);
  bacaserial(200);

  SIM7000.println(F("AT+HTTPTERM"));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();

  // Serial.println(F("AT+SAPBR=0,1"));
  SIM7000.println(F("AT+SAPBR=0,1"));
  bacaserial(200);
}

void kirimSMS() {
  SIM7000.println(F("AT+CMGF=1"));
  bacaserial(500);
  Serial.println(json);
  kalimat = "AT+CMGS=\"" + String(hp) + ("\"");
  SIM7000.println(kalimat);
  bacaserial(500);

  kalimat = "";
  if (kode > 0)  kalimat = "http://mantisid.id/project/osh_2019/api/product/osh_data_c.php?";
  kalimat.concat(json);
  Serial.println(kalimat);
  Serial.flush();
  SIM7000.print(kalimat);
  SIM7000.write(26);
  SIM7000.flush();
  while (1) {
    while (SIM7000.available() > 0) {
      karakter = SIM7000.read();
      Serial.write(karakter);
      if (karakter == '+')    break;
    }
    if (karakter == '+')    break;
  }
  bacaserial(200);
  Serial.println("kirim SMS selesai");
}
