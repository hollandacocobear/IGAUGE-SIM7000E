// TELKOMSEL IOT PRESSURE DATA LOGGER
// BOARD : ARDUINO UNO
// DEVELOPED BY : MANTIS ID
// REVISED : 2020-1-13

#include <TimeLib.h>
#include <TimeAlarms.h>
#include <SoftwareSerial.h>
#include "variable.h"

//JIKA MENGGUNAKAN ARDUINO UNO, BARIS BERIKUT DI UNCOMMENT
SoftwareSerial SIM7000(2, 3); // RX | TX

//JIKA MENGGUNAKAN MEGA2560 MAKA MENGGUNAKAN PERINTAH INI
//HardwareSerial & SIM7000 = Serial1;

void inisialisasi() {
  Serial.begin(9600);
  Serial.println(F("WATER PRESSURE SENSOR TSO 2019"));

  Serial.println(F("Inisialisasi Serial GSM"));
  SIM7000.begin(9600);
  delay(1000);

  Serial.println(F("Atur LED OK dan LED ERROR"));
  pinMode(ledOK, OUTPUT);
  pinMode(ledERROR, OUTPUT);
  pinMode(powerSensor, OUTPUT);
  pinMode(key, OUTPUT);
  delay(1000);

  Serial.println(F("Nyalakan LED"));
  digitalWrite(ledOK, HIGH);
  delay(1000);
  digitalWrite(ledERROR, HIGH);
  delay(1000);
  digitalWrite(ledOK, 0);
  digitalWrite(ledERROR, 0);
  delay(1000);
}

void setup() {
  inisialisasi();
/*
  //INIT SD CARD
  pinMode(SSpin, OUTPUT);
  digitalWrite(SSpin, HIGH);
  delay(100);
  sdInit();
  delay(1000);
  //GET CONFIG TXT
  configs();

  //munculkan parameter di serial monitor
  Serial.print(F("Stasiun = "));
  Serial.println(ID);
  Serial.print(F("Interval data = "));
  Serial.print(interval);
  Serial.println(F(" menit"));
  Serial.print(F("Burst Interval = "));
  Serial.print(burst);
  Serial.println(F(" detik"));
  Serial.flush();
  delay(1000);

  //CEK FILE PENYIMPANAN UNTUK DATA
  //  cekfile();

*/
  //INIT GSM
  Serial.println(F("Inisialisasi GSM"));
  delay(1000);
  initGSM();
  delay(3000);

  //SINKRONISASI WAKTU DENGAN NETWORK
  Serial.println(F("SINKRONISASI WAKTU DENGAN NETWORK"));
  NTPtime();

  Serial.println(F("\r\nSet waktu pengambilan data sesuai dengan interval"));
  waktu();
  setTime(jam, menit, detik, hari, bulan, tahun); // set time berdasarkan waktu dari GSM
  dataJSON();
  // delay(2000);
/*
  Serial.println(F("Cek GPRS sebanyak 3 kali"));
  hapus();
  for (i = 1; i < 4; i++) {
    Serial.print(F("CEK "));
    Serial.println(i);
    indeks = GPRScommunication();
    if (indeks == 0) {
      Serial.println(F("GPRS ERROR"));
      delay(1000);
      if (i == 3) {
        Serial.println(F("Kirim SMS bahwa GPRS Error"));
        strcat(json,"Stasiun ");
        strcat(json,ID);
        strcat(json," GPRS Error");
        kirimSMS();
        delay(5000);
        break;
      }
      continue;
    }
    else {
      Serial.println(F("Komunikasi GPRS OK. Kita Lanjut ke step berikutnya"));
      break;
    }
  }

  //ambil waktu dan set waktu untuk menunggu
  waktu();
  //  waktuTunggu();

  // create timers, to trigger relative to when they're created
  Alarm.timerRepeat(interval * 60, ambildata);         // timer setiap interval dalam satuan detik
  ambildata();*/
}

void loop() {
  Alarm.delay(0);
}

void ambildata() {
  Serial.println("\r\n SAATNYA AMBIL DATA");
  waktu();
  digitalWrite(powerSensor, 1);
  delay(100);

  sensor();
  digitalWrite(powerSensor, 0);

  //cek data tekanan di serial monitor
  Serial.print(F("Tekanan="));
  Serial.print(tekanan);
  Serial.println(F(" bar\n"));

  //coba pengiriman data ker server sebanyak 3 kali
  byte indeksGPRS = 0;
  for (i = 1; i < 2; i++) {
    Serial.print(F("Pengiriman ke-"));
    Serial.println(i);
    indeksGPRS = 0;//GPRScommunication();
    if (indeksGPRS == 0) {
      Serial.println(F("GPRS ERROR"));
      digitalWrite(ledOK, 0);
      digitalWrite(ledERROR, 1);
      delay(1000);
      dataJSON();
      continue;
    }
    else {
      digitalWrite(ledERROR, 0);
      Serial.println(F("KIRIM DATA KE SERVER"));
      sendServer();    //kirim data
    }
    if (kode == 200) break;
  }

  //jika data dikirim 3 kali dan tidak masuk ke server maka kirim sms
  Serial.println(F("keluar dari pengiriman data"));
  Serial.println(kode);
  if (kode != 200) {
    Serial.println(F("ERROR kirim. kasih tau via SMS"));
    kirimSMS();
    delay(5000);
  }

}

void indikatorOK() {
  digitalWrite(ledOK, 1);
  delay(500);
  digitalWrite(ledOK, 0);
  delay(500);
}

void hapus() {
  for ( i = 0; i < sizeof(kata); i++) { //clear variable
    kata[i] = (char)0;
  }
for ( i = 0; i < sizeof(json); i++) { //clear variable
    json[i] = (char)0;
  }
}
