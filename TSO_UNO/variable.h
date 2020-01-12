//KONFIGURASI ID DAN INTERVAL
String ID = "0030";
byte interval = 5;
byte burst = 2;
float offset= 0.00;

//PARAMETER PEMBAGI WAKTU
byte pembagiWaktu = 60 / interval;

//KONFIGURASI PIN
#define powerSensor 4 // pin untuk menyalakan sensor
#define key 5         // pin power key
#define ledOK 8       // pin LED untuk tanda OK
#define ledERROR 7    // pin LED untuk tanda ERROR
#define SSpin 10      // pin Slave Select
#define pressurePin A0  // Put the data cable to pin A0

//KONFIGURASI NO HP
char hp[14] = "+6285958553254"; //"+628119811995";

//KONFIGURASI IP. Pilih salah satu
String IP = "Telkomsel"; // untuk GSM
//String IP = "nb1internet"; // untuk NB-IoT

//KONFIGURASI INTERNET
//String APN = "Telkomsel";
//String USER = "wap";
//String PWD = "wap123";

char karakter;
char sdcard[40];
byte i, indeks, indeks1, indeks2;
String json, kalimat;
unsigned int kode;
unsigned long mulai;
float tekanan;
byte tahun, bulan, hari, jam, menit, detik;
