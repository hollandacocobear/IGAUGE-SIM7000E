void bacaserial(int wait) {
  kalimat = "";
  mulai = millis();
  while ((mulai + wait) > millis()) {
    while (SIM7000.available() > 0) {
      karakter = SIM7000.read();
      kalimat += karakter;
    }
  }
  SIM7000.flush();
  Serial.println(kalimat);
  Serial.flush();
}

byte ConnectAT(String cmd, int d) {
  indeks = 0;
  while (1) {
    SIM7000.println(cmd);
    while (SIM7000.available()) {
      if (SIM7000.find("OK"))
        indeks = 8;
    }
    delay(d);
    if (indeks > 4) {
      break;
    }
    indeks++;
  }
  return indeks;
}

void initGSM() { 
  //ATUR MODUL GSM UNTUK TIDAK SLEEP
  SIM7000.println(F("AT+CSCLK=0"));
  bacaserial(200);
  
  //CONNECT AT
  for (i = 0; i < 6; i++) {
    indeks2 = ConnectAT(F("AT"), 100);
    if (indeks2 == 8) {
      Serial.println(F("GSM MODULE OK!!"));
      Serial.flush();
      SIM7000.flush();
      break;
    }
    if (indeks2 < 8) {
      Serial.println(F("GSM MODULE ERROR"));
      Serial.flush();
      SIM7000.flush();

      if (i==5) {
        Serial.println(F("CONTACT CS!!!"));
        Serial.flush();
        SIM7000.flush();
        while (1) {
          digitalWrite(ledERROR, 1);
          delay(500);
          digitalWrite(ledERROR, 0);
          delay(500);
        }
      }
    }
  }


  //CEK KARTU SIAP ATAU TIDAK. "+CPIN: READY" YANG DIINGINKAN
  SIM7000.println(F("AT+CPIN?"));
  bacaserial(200);

  //SET PHONE FUNCTIONALITY MENJADI FULL FUNCTIONALITY
  SIM7000.println(F("AT+CFUN=1"));
  bacaserial(200);

  //ATUR BAND KE SELURUH MODE
  SIM7000.println(F("AT+CBAND=\"ALL_MODE\""));
  bacaserial(200);

  //CEK VOLTASE BATERAI
  //  SIM7000.println(F("AT+CBC"));
  //  bacaserial(200);

  //PEMILIHAN MODE YANG DIINGINKAN KE 13 (GSM ONLY) ATAU 51 (GSM AND LTE)
  SIM7000.println(F("AT+CNMP=13"));
  bacaserial(200);

  //PEMILIHAN MODE CAT-M ATAU NB-IOT [1.CAT-M] [2.NB-Iot] [3.CAT-M & NB-IoT]
  SIM7000.println(F("AT+CMNB=3"));
  bacaserial(200);

  //DEFINE PDP CONTEXT
  kalimat="AT+CGDCONT=1,\"IP\",\"" + IP+ "\"";
  SIM7000.println(kalimat);
  bacaserial(200);
  kalimat="AT+CGDCONT=13,\"IP\",\"" + IP+ "\"";
  SIM7000.println(kalimat);
  bacaserial(200);

  //REGISTRASI KARTU KE NETWORK PROVIDER
  SIM7000.println(F("AT+CREG=1"));
  bacaserial(200);

  //CEK OPERATOR PROVIDER
  while (1) {
    kalimat = "";
    //PERINTAH MENCARI NAMA OPERATOR PROVIDER
    SIM7000.println(F("AT+COPS?"));
    Serial.flush();
    SIM7000.flush();
    delay(200);
    while (SIM7000.available() > 0) {
      if (SIM7000.find("+COPS:")) {
        while (SIM7000.available() > 0) {
          kalimat += (char)SIM7000.read();
        }
      }
    }

    // MENCARI INDEKS KARAKTER DARI NAMA PROVIDER YANG DITEMUKAN
    indeks = kalimat.indexOf('"');
    indeks1 = kalimat.indexOf('"', indeks + 1);
    kalimat = kalimat.substring(indeks + 1, indeks1);
    Serial.println(kalimat);
    Serial.flush();
    delay(100);

    //option if not register at network
    if (kalimat == "")  {
      indeks2++;
      if (indeks2 == 15) {
        Serial.println(F("OPERATOR PROVIDER TIDAK DITEMUKAN"));
        while (1) {
          digitalWrite(ledERROR, 1);
          delay(500);
          digitalWrite(ledERROR, 0);
          delay(500);
        }
      }
    }
    if (kalimat.length() > 3) break;
    delay(1000);
  }
  Serial.println(F("OPERATOR PROVIDER DITEMUKAN"));
  Serial.flush();

  //CEK KEKUATAN SINYAL
  SIM7000.println(F("AT+CSQ"));
  bacaserial(200);

  //CEK PDP CONTEXT APAKAH SUDAH SESUAI
  SIM7000.println(F("AT+CGDCONT?"));
  bacaserial(200);

  //CEK STATUS REGISTRASI NETWORK GPRS
  SIM7000.println(F("AT+CGREG?"));
  bacaserial(200);

  //CEK INFORMASI MENGENAI JARINGAN GSM ATAU NB IOT DARI PROVIDER YANG DITERIMA MODUL GSM
  SIM7000.println(F("AT+CPSI?"));
  bacaserial(200);
  SIM7000.println(F("AT+CENG?"));
  bacaserial(200);
}
