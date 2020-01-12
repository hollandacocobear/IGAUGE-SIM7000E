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
      kata[indeks++] = file.read();
    }
  }
  else  { //error reading
    Serial.println(F("ERROR READING!!!"));
    while (1);
  }
  file.close();

  kalimat = String(kata);
//  Serial.println(kalimat);
  hapus();
  Serial.println(F("GET CONFIG SUCCESS!!!"));
  Serial.flush();

  //ID
  indeks = kalimat.indexOf("=", 1);
  indeks1 = kalimat.indexOf("\r", indeks + 1);
  kalimat.substring(indeks + 2, indeks1).toCharArray(ID,5);
  
  //interval data
  indeks = kalimat.indexOf("=", indeks1 + 1);
  indeks1 = kalimat.indexOf("\r", indeks + 1);
  interval = kalimat.substring(indeks + 1, indeks1).toInt();

  //burst interval
  indeks = kalimat.indexOf("=", indeks1 + 1);
  indeks1 = kalimat.indexOf("\r", indeks + 1);
  burst = kalimat.substring(indeks + 1, indeks1).toInt();
}

void cekfile() {
  while (1) {
    kalimat = "LOG" + String(nomor) + F(".txt");
    kalimat.toCharArray(kata, 13);
    indeks = SD.exists(kata);
    if (indeks == 0) {
      Serial.println(kalimat + F(" doesn't exist"));
      Serial.flush();
      file = SD.open(kalimat, FILE_WRITE);
      file.println(F("Waktu (tahun/bulan/hari jam:menit:detik) , Tekanan (Bar), Kode"));
      file.flush();
      file.close();
      indeks1 = 1;
      break;
    }
    else {
      Serial.println(kalimat + F(" exist"));
      Serial.flush();
      nomor++;
    }
    delay(500);
    if (indeks1 == 1) {
      break;
    }
  }
}

void simpanData(){
  kalimat = "LOG" + String(nomor) + ".txt";
  SdFile::dateTimeCallback(dateTime);

  Serial.print(F("Simpan data ke "));
  Serial.print(kalimat);
  file = SD.open(kalimat, FILE_WRITE);
  file.print("20");
  file.print(tahun);
  file.write("/");
  file.print(bulan);
  file.write("/");
  file.print(hari);
  file.write(" ");
  file.print(int2digits(jam));
  file.write(":");
  file.print(int2digits(menit));
  file.write(":");
  file.println(int2digits(detik));
  file.print(F(","));
  file.print(tekanan);
  file.print(F(","));
  file.println(kode);  file.flush();  
  file.close();
  Serial.println(" selesai");
}

void dateTime(uint16_t* date, uint16_t* time) {
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(year(), month(), day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(hour(), minute(), second());
}
