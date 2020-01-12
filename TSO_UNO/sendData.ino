boolean GPRScommunication() {
  // ATTACH GPRS SERVICE
  SIM7000.println(F("AT+CGATT=1"));
  bacaserial(200);

  //DEAKTIVASI GPRSPDP CONTEXT DAN TUTUP BEARER
  SIM7000.println(F("AT+CIPSHUT;+SAPBR=0,1"));
  bacaserial(500);

  //CONNECTION TYPE = GPRS ; ATUR APN, USER, DAN PASSWORD
  SIM7000.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\";+SAPBR=3,1,\"APN\",\"Telkomsel\";+SAPBR=3,1,\"USER\",\"\";+SAPBR=3,1,\"PWD\",\"\""));
  bacaserial(500);

  //OPEN BEARER
  SIM7000.println(F("AT+SAPBR=1,1"));
  bacaserial(5000);
  Serial.flush();
  SIM7000.flush();
  delay(100);

  //PARSING STRING DARI OPEN BEARER UNTUK CEK OK ATAU ERROR
  indeks1 = kalimat.indexOf('\r', 1);
  json = kalimat.substring(indeks1 + 1, kalimat.length());

  if (json == "ERROR" || json=="") {
    return 0;
  }


  //mulai task and set APN, user name, password
  SIM7000.println(F("AT+CSTT=\"Telkomsel\""));
  bacaserial(5000);

  SIM7000.println(F("AT+CIICR"));
  bacaserial(3000);
  //PARSING STRING DARI OPEN BEARER UNTUK CEK OK ATAU ERROR
  indeks1 = kalimat.indexOf('\r', 1);
  json = kalimat.substring(indeks1 + 1, kalimat.length());
  if (json == "ERROR") {
    return 0;
  }

  SIM7000.println(F("AT+CIFSR"));
  bacaserial(200);

  return 1;
}

void sendServer() {
  json="";
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
  
  //http://www.mantisid.id/api/product/osh_data_c.php?= {"Data":"003,20190315213555,2456,02356,02201,01233,05,15"}
  // {"Data":"id unit,yyyymmddhhmmss,tekanan(5 digit),kelembaban(5 digit),suhu(5 digit),volt (5 digit), burst (2 digit), interval (2 digit)"}
  json="";
  json  = "{\"Data\":\"";
  json.concat(String(ID));
  json.concat(String(",20"));
  json.concat(String(tahun));
  if (bulan < 10) {
    json += "0" + String(bulan);
  }
  if (bulan >= 10) {
    json += String(bulan);
  }
  if (hari < 10) {
    json += "0" + String(hari);
  }
  if (hari >= 10) {
    json += String(hari);
  }
  if (jam < 10) {
    json += "0" + String(jam);
  }
  if (jam >= 10) {
    json.concat(String(jam));
  }
  if (menit < 10) {
    json += "0" + String(menit);
  }
  if (menit >= 10) {
    json.concat(String(menit));
  }
  if (detik < 10) {
    json += "0" + String(detik);
  }
  if (detik >= 10) {
    json.concat(String(detik));
  }

  json.concat(",");
  json.concat(String(tekanan,2));
  json.concat(",-9999,-9999,-9999,");
  json.concat(String(burst));
  json.concat(",");
  json.concat(String(interval));
  json.concat("\"}");

  Serial.println(json);
  Serial.flush();
  delay(1000);
  //SET HTTP DATA FOR SENDING TO SERVER
  kalimat = "";
  kalimat.concat("AT+HTTPDATA=");
  kalimat.concat(String(json.length() + 1));
  kalimat.concat(",15000");
  Serial.println(kalimat);
  Serial.flush();
  delay(2000);
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
  indeks1 = '0';
  indeks2 = '0';
  kalimat = "";
  //CHECK KODE HTTPACTION
  while ((mulai + 30000) > millis()) {
    while (SIM7000.available() > 0) {
      karakter = SIM7000.read();
      kalimat += karakter;
      indeks1 = kalimat.indexOf(":");
      indeks2 = kalimat.length();
      if (indeks2 - indeks1 > 8) {
        Serial.println(F("MOVING OUT"));
        break;
      }
    }
    if (indeks2 - indeks1 > 8) {
      Serial.println(F("OK"));
      break;
    }
  }
  Serial.flush();
  SIM7000.flush();

  Serial.println(F("Hasil HTTPACTION:"));
  Serial.println(kalimat);

  indeks1 = '0';
  indeks1 = kalimat.indexOf(',');
  indeks2 = kalimat.indexOf(',', indeks1 + 1);
  kode = kalimat.substring(indeks1 + 1, indeks2).toInt();

  Serial.print(F("kode="));
  Serial.println(kode);
  Serial.flush();

  SIM7000.println("AT+HTTPREAD");
  delay(10000);
  bacaserial(200);

  indeks1 = '0';
  indeks2 = '0';
  SIM7000.println(F("AT+HTTPTERM"));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();

  // Serial.println(F("AT+SAPBR=0,1"));
  SIM7000.println(F("AT+SAPBR=0,1"));
  bacaserial(200);
}

void kirimSMS(String input) {
  SIM7000.println(F("AT+CMGF=1"));
  bacaserial(200);
  kalimat = "AT+CMGS=\"" + String(hp) + ("\"");
  SIM7000.println(kalimat);
  bacaserial(500);
  delay(500);
  kalimat = input;
  if(input.charAt(0)=='{')  kalimat+= "\r\n TIDAK TERKIRIM";
  SIM7000.println(kalimat);
  delay(100);
  SIM7000.println((char)26);
  bacaserial(1000);
}
