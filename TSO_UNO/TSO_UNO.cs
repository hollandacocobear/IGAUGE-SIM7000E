// TELKOMSEL IOT PRESSURE DATA LOGGER
// BOARD : ARDUINO UNO
// DEVELOPED BY : MANTIS ID

#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <SoftwareSerial.h>
SoftwareSerial SIM7000(2, 3); // RX | TX

//internet configuration
String hp = "+628119811995";
//String APN = "Telkomsel";
//String USER = "wap";
//String PWD = "wap123";

//sensor configuration
String ID = "0030";
byte interval = 5;
byte burst = 2;

File file;
String kalimat;

#define powerSensor 4
#define key 5
#define LED1 8
#define LED2 7
#define SSpin           10
#define pressurePin A0

char karakter;
char sdcard[50];
byte i, indeks, indeks1, indeks2;
byte tahun, bulan, hari, jam, menit, detik;
String hasil, json;
unsigned int kode;
unsigned long start;
float offset, tekanan;

void setup() {
  Serial.begin(9600);
  SIM7000.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(powerSensor, OUTPUT);
  pinMode(key, OUTPUT);

  Serial.println("WATER PRESSURE SENSOR TSO 2019");
  digitalWrite(LED1, HIGH);
  delay(1000);
  digitalWrite(LED2, HIGH);
  delay(1000);
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  delay(1000);

  //SD INIT
  /*
    Serial.println("cek micro SD");
    pinMode(SSpin, OUTPUT);
    digitalWrite(SSpin, HIGH);
    delay(100);
    if (!SD.begin(SSpin)) { //SD ERROR
    Serial.println(F("micro SD error!!!"));
    Serial.flush();
    digitalWrite(LED2, 1);
    while (1);
    }

    Serial.println(F("MICRO SD CARD INIT OK!"));
    Serial.flush();
    for (indeks = 0; indeks < 2; indeks++) {
    indikatorOK();
    }
    delay(1000);
  */

  //GET CONFIG TXT
  // Serial.println(F("CONFIGURATION FILE"));
  // configs();
  // delay(1000);

  //init GSM
  sim();
  delay(1000);
  // waktu();
  // delay(2000);

  //cek waktu
  /*
    waktu();
    Serial.println(F("ujicoba pengiriman data ke server sebanyak 3 kali"));
    tekanan = 99.99;
    for (i = 1; i < 4; i++) {
    Serial.print(F("Uji="));
    Serial.println(i);
    sendServer();
    }
  */
  //ambil waktu dan set waktu untuk alarm
  // waktu();
  // setTime(jam, menit, detik, hari, bulan, tahun); // set time berdasarkan waktu dari GSM

  // create timers, to trigger relative to when they're created
  //Alarm.timerRepeat(interval * 60, ambildata);         // timer setiap interval dalam satuan detik
  ambildata();
}

void loop() {
  Alarm.delay(0);
}

void ambildata() {
  Serial.println("saatnya ambil data");
  digitalWrite(powerSensor, 1);
  delay(100);
  waktu();

  start = 0;
  tekanan = 0;
  for (indeks = 0; indeks < burst; indeks++) {
    int pressADC = analogRead(pressurePin);
    start += pressADC;
  }
  float pressvolt = float(float (start / burst) / 1024.00 * 5.00);
  tekanan = (3.00 * pressvolt - 1.5) + offset;
  digitalWrite(powerSensor, 0);

  //cek data tekanan di serial monitor
  Serial.print(F("Tekanan="));
  Serial.print(tekanan);
  Serial.println(F(" bar\n"));

  //kirim data
  sendServer();
}

void indikatorOK() {
  digitalWrite(LED1, 1);
  delay(500);
  digitalWrite(LED1, 0);
  delay(500);
}

void configs() {
  indeks = 0;
  /*
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
  */
  Serial.print(F("Nomor stasiun = "));
  Serial.println(ID);
  Serial.print(F("Interval data = "));
  Serial.print(interval);
  Serial.println(F(" menit"));
  Serial.print(F("Burst Interval = "));
  Serial.print(burst);
  Serial.println(F(" detik"));
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
    if (indeks > 5) {
      break;
    }
    indeks++;
  }
  return indeks;
}

void ceksim() {
  indeks2 = 0;
  kalimat = "";
  hasil = "";
  // Serial.println(F("AT+CREG=1"));
  SIM7000.println(F("AT+CREG=1"));
  bacaserial(200);
  delay(500);

  while (1) {
    // Serial.println(F("AT+COPS?"));
    SIM7000.println(F("AT+COPS?"));
    Serial.flush();
    SIM7000.flush();
    delay(200);
    kalimat = "";
    while (SIM7000.available() > 0) {
      if (SIM7000.find("+COPS:")) {
        while (SIM7000.available() > 0) {
          kalimat += (char)SIM7000.read();
        }
      }
    }

    indeks = kalimat.indexOf('"');
    indeks1 = kalimat.indexOf('"', indeks + 1);
    hasil = kalimat.substring(indeks + 1, indeks1);
    Serial.println(hasil);
    Serial.flush();
    delay(100);

    //option if not register at network
    if (hasil == "")  {
      indeks2++;
      if (indeks2 == 15) {
        Serial.println(F("NO OPERATOR FOUND"));
        while (1) {
          digitalWrite(LED2, 1);
          delay(500);
          digitalWrite(LED2, 0);
          delay(500);
        }
      }
    }
    if (hasil.length() > 3) break;
  }
  Serial.println("operator ditemukan");
  Serial.flush();


  Serial.println(F("AT+CSQ"));
  SIM7000.println(F("AT+CSQ"));
  Serial.flush();
  SIM7000.flush();
  delay(200);
  kalimat = "";
  while (SIM7000.available() > 0) {
    if (SIM7000.find("+CSQ: ")) {
      while (SIM7000.available() > 0) {
        kalimat += (char)SIM7000.read();
      }
    }
  }

  Serial.println(" ");
  Serial.println(kalimat);
  Serial.flush();
  SIM7000.flush();

  //PENGECEKAN IP YANG SUDAH DIMASUKKAN
  SIM7000.println(F("AT+CGDCONT?"));
  bacaserial(3000);
  
  //Network Registration Status. Nilai 1 di bagian akhir menandakan network sudah diregistrasi
  SIM7000.println(F("AT+CGREG?"));
  bacaserial(200);
  
  //Inquiring UE system information. Cek service apakh GSM, LTE, atau NB-IOT
  SIM7000.println(F("AT+CPSI?"));
  bacaserial(200);
  
  //Switch on or off Engineering Mode. Pengecekan BTS yang berhubungan dengan modul dan mode service yang digunakan
  SIM7000.println(F("AT+CENG?"));
  bacaserial(1000);
  Serial.println("");
  Serial.flush();
  SIM7000.flush();
  delay(3000);
}

void sim() { //udah fix
  // Serial.println(F("AT+CSCLK=0"));
  SIM7000.println(F("AT+CSCLK=0"));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();

  //CONNECT AT
  for (indeks = 0; indeks < 6; indeks++) {
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

      if (indeks == 5) {
        Serial.println(F("CONTACT CS!!!"));
        Serial.flush();
        SIM7000.flush();
        while (1) {
          digitalWrite(LED2, 1);
          delay(500);
          digitalWrite(LED2, 0);
          delay(500);
        }
      }
    }
  }
  delay(1000);

  SIM7000.println(F("AT+CPIN?"));
  bacaserial(200);
  SIM7000.println(F("AT+CFUN=1"));
  bacaserial(200);
  SIM7000.println(F("AT+CBAND=\"ALL_MODE\""));
  bacaserial(200);
//  SIM7000.println(F("AT+CBC"));
//  bacaserial(200);
  SIM7000.println(F("AT+CNMP=51"));
  bacaserial(200);
  SIM7000.println(F("AT+CMNB=3"));
  bacaserial(200);

  //PDP CONTEXT BERUPA IP DIATUR SESUAI PROVIDER
  SIM7000.println(F("AT+CGDCONT=1,\"IP\",\"nb1internet\""));
  bacaserial(200);
  SIM7000.println(F("AT+CGDCONT=13,\"IP\",\"nb1internet\""));
  bacaserial(200);

  //CEK SIM
  ceksim();
}

void bacaserial(int wait) {
  kalimat = "";
  start = millis();
  while ((start + wait) > millis()) {
    while (SIM7000.available() > 0) {
      karakter=SIM7000.read();
      kalimat += karakter;
    }
  }
  SIM7000.flush();
  Serial.println(kalimat);
  Serial.flush();
}

void waktu() {
  kalimat = "";
  SIM7000.flush();
  Serial.println("");
  SIM7000.println(F("AT"));
  bacaserial(200);

  SIM7000.println(F("AT+CCLK?"));
  SIM7000.flush();
  bacaserial(200);

  //parse string
  indeks2 = 0;

  //tahun
  indeks1 = kalimat.indexOf("\"", indeks2 + 1);
  indeks2 = kalimat.indexOf("/", indeks1 + 1);
  tahun = kalimat.substring(indeks1 + 1, indeks2).toInt();
  Serial.print(F("tahun="));
  Serial.println(tahun);

  //bulan
  indeks1 = kalimat.indexOf("/", indeks2 + 1);
  bulan = kalimat.substring(indeks2 + 1, indeks1).toInt();
  Serial.print(F("bulan="));
  Serial.println(bulan);

  //hari
  indeks2 = kalimat.indexOf(",", indeks1 + 1);
  hari = kalimat.substring(indeks1 + 1, indeks2).toInt();
  Serial.print(F("hari="));
  Serial.println(hari);

  //jam
  indeks1 = kalimat.indexOf(":", indeks2 + 1);
  jam = kalimat.substring(indeks2 + 1, indeks1).toInt();
  Serial.print(F("jam="));
  Serial.println(jam);

  //menit
  indeks2 = kalimat.indexOf(":", indeks1 + 1);
  menit = kalimat.substring(indeks1 + 1, indeks2).toInt();
  Serial.print(F("menit="));
  Serial.println(menit);

  //detik
  indeks1 = kalimat.indexOf("+", indeks2 + 1);
  detik = kalimat.substring(indeks2 + 1, indeks1).toInt();
  Serial.print(F("detik="));
  Serial.println(detik);
}

String int2digits(int number) {
  String y = "";
  if (number >= 0 && number < 10) {
    y += "0";
  }
  y += String(number);
  return y;
}

void sendServer() {
  // ATTACH GPRS SERVICE
  Serial.println(F("kirim data"));
  SIM7000.println(F("AT+CGATT=1"));
  Serial.flush();
  SIM7000.flush();
  bacaserial(200);
  delay(300);

  // coba pengiriman data selama 3 kali
  //for (indeks = 0; indeks < 3; indeks++) {
  //kirim data ke server
  SIM7000.println(F("AT+CIPSHUT"));
  bacaserial(500);
  Serial.flush();
  SIM7000.flush();

  SIM7000.println(F("AT+SAPBR=0,1"));
  bacaserial(500);
  Serial.flush();
  SIM7000.flush();

  //CONNECTION TYPE
  SIM7000.println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();
  delay(100);

  //APN ID
  // hasil = String();
  // Serial.println(F("AT+SAPBR=3,1,\"APN\",\"Telkomsel\""));
  SIM7000.println(F("AT+SAPBR=3,1,\"APN\",\"Telkomsel\""));
  bacaserial(200);
  // Serial.flush();
  // SIM7000.flush();

  //APN USER
  // hasil = String();
  // Serial.println(F("AT+SAPBR=3,1,\"USER\",\"\""));
  SIM7000.println(F("AT+SAPBR=3,1,\"USER\",\"\""));
  bacaserial(200);
  // Serial.flush();
  // SIM7000.flush();

  //APN PASSWORD
  // hasil = String();
  // Serial.println(F("AT+SAPBR=3,1,\"PWD\",\"\""));
  SIM7000.println(F("AT+SAPBR=3,1,\"PWD\",\"\""));
  bacaserial(200);
  // Serial.flush();
  // SIM7000.flush();

  //OPEN BEARER
  SIM7000.println(F("AT+SAPBR=1,1"));
  bacaserial(5000);
  Serial.flush();
  SIM7000.flush();
  delay(100);

  //Start task and set APN, user name, password
  hasil = String("AT+CSTT=\"Telkomsel\",\"\",\"\"");
  // Serial.println(hasil);
  SIM7000.println(hasil);
  bacaserial(5000);

  SIM7000.println(F("AT+CIICR"));
  bacaserial(3000);
  
  SIM7000.println(F("AT+CIFSR"));
  bacaserial(200);

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
  bacaserial(200);

  //http://www.mantisid.id/api/product/osh_data_c.php?= {"Data":"003,20190315213555,2456,02356,02201,01233,05,15"}
  // {"Data":"id unit,yyyymmddhhmmss,tekanan(5 digit),kelembaban(5 digit),suhu(5 digit),volt (5 digit), burst (2 digit), interval (2 digit)"}
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
  json.concat(String(int(tekanan * 100)));
  json.concat(",-9999,-9999,-9999,");
  json.concat(String(burst));
  json.concat(",");
  json.concat(String(interval));
  json.concat("\"}");

  Serial.println(json);
  Serial.flush();
  // delay(1000);
  
  //SET HTTP DATA FOR SENDING TO SERVER
  hasil = "";
  hasil.concat("AT+HTTPDATA=");
  hasil.concat(String(json.length() + 1));
  hasil.concat(",15000");
  // Serial.println(hasil);
  // Serial.flush();
  // delay(2000);
  SIM7000.println(hasil);
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
  start = millis();
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
  hasil = "";
  //CHECK KODE HTTPACTION
  while ((start + 30000) > millis()) {
    while (SIM7000.available() > 0) {
     karakter=SIM7000.read();
	 Serial.print(karakter);
      hasil += karakter;
      indeks1 = hasil.indexOf(":");
      indeks2 = hasil.length();
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

  SIM7000.println("AT+HTTPREAD");
  delay(10000);
  bacaserial(200);

  indeks1 = '0';
  indeks2 = '0';
  indeks1 = kalimat.indexOf(':');
  indeks2 = kalimat.indexOf('{', indeks1 + 1);
  kode = kalimat.substring(indeks1 + 1, indeks2).toInt();
  
  SIM7000.println(F("AT+HTTPTERM"));
  bacaserial(200);
  Serial.flush();
  SIM7000.flush();

  SIM7000.println(F("AT+SAPBR=0,1"));
  bacaserial(200);


  Serial.print(F("kode="));
  Serial.println(kode);
  Serial.flush();
  //if (kode == 200) break;
  //  }
  //jika data dikirim 3 kali dan tidak masuk ke server maka kirim sms
  if (kode != 200) {
    Serial.println(F("ERROR kirim. kasih tau via SMS"));
    kirimSMS();
    delay(3000);
  }
}

void kirimSMS() {
  SIM7000.println(F("AT+CMGF=1"));
  bacaserial(200);
  hasil = "";
  hasil.concat(String("AT+CMGS=\""));
  hasil.concat(hp);
  hasil.concat("\"");
  Serial.println(hasil);
  delay(1000);
  SIM7000.println(hasil);
  bacaserial(500);
  delay(500);
  hasil="";
  hasil.concat(json);
  hasil.concat(" TIDAK TERKIRIM dengan kode ");
  hasil.concat(kode);
  Serial.println(hasil);
  SIM7000.println(hasil);
  delay(100);
  SIM7000.println(26);
  bacaserial(1000);
}


//CEK

