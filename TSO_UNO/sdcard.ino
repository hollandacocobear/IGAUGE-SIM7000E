#include <SPI.h>
#include <SD.h>

File file;

//SD INIT
void sdInit() {
  if (!SD.begin(SSpin)) { //SD ERROR
    Serial.println(F("MICRO SD CARD INIT ERROR!"));
    Serial.flush();
    digitalWrite(ledERROR, 1);
    while (1);
  }
  else {
    Serial.println(F("MICRO SD CARD INIT OK!"));
    Serial.flush();
    for (indeks = 0; indeks < 2; indeks++) {
      indikatorOK();
    }
  }
}

void configs() {
  indeks = 0;

  file = SD.open(F("config.txt"));
  if (file) {
    while (file.available()) {
      sdcard[indeks++] = file.read();
    }
  }
  else  { //error reading
    Serial.println(F("ERROR READING!!!"));
    while (1);
  }
  file.close();

  kalimat = String(sdcard);
  for ( indeks = 0; indeks < sizeof(sdcard); indeks++) { //clear variable
    sdcard[indeks] = (char)0;
  }

  Serial.println(kalimat);
  Serial.flush();
  Serial.println(F("GET CONFIG SUCCESS!!!"));
  Serial.flush();

  indeks1 = 0;

  //ID
  indeks = kalimat.indexOf("=", indeks1 + 1);
  indeks1 = kalimat.indexOf("\r", indeks + 1);
  ID = kalimat.substring(indeks + 2, indeks1);

  //interval data
  indeks = kalimat.indexOf("=", indeks1 + 1);
  indeks1 = kalimat.indexOf("\r", indeks + 1);
  interval = kalimat.substring(indeks + 1, indeks1).toInt();

  //burst interval
  indeks = kalimat.indexOf("=", indeks1 + 1);
  indeks1 = kalimat.indexOf("\r", indeks + 1);
  burst = kalimat.substring(indeks + 1, indeks1).toInt();

  kalimat = '0';
}
