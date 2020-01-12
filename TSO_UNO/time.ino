void NTPtime() {
  // SINKRONISASI WAKTU LOKAL DENGAN WAKTU DARI NETWORK TIME PROVIDER
  SIM7000.println(F("AT+CNTP"));
  SIM7000.flush();
  bacaserial(500);
  Serial.flush();
  SIM7000.flush();
  delay(500);
}

void waktu() {
  //bersihkan variabel
  hapus();
  tahun = '0'; bulan = '0'; hari = '0'; jam = '0'; menit = '0'; detik = '0';

  //AMBIL WAKTU DARI PROVIDER
  //  Serial.println("ambil waktu");
  Serial.flush();
  SIM7000.flush();
  SIM7000.println(F("AT+CCLK?"));
  bacaserial(500);
  delay(300);

  //debug
  //  Serial.println(F("hasil dari cclk="));
  //  Serial.println(kalimat);

  //PISAHKAN DATA WAKTU
  //tahun
  strcat(json,kalimat.c_str());
  char *p = strstr(json,"\"");
//  Serial.println(p);
//  Serial.println(int(p-json));
  kata[0]=json[int(p-json)+1];
  kata[1]=json[int(p-json)+2];
  tahun=atoi(kata);
//  Serial.println(tahun);
  
  //bulan
  kata[0]=json[int(p-json)+4];
  kata[1]=json[int(p-json)+5];
  bulan = atoi(kata);
//  Serial.println(bulan);

  //hari
  kata[0]=json[int(p-json)+7];
  kata[1]=json[int(p-json)+8];
  hari = atoi(kata);
//  Serial.println(hari);

  //jam
  kata[0]=json[int(p-json)+10];
  kata[1]=json[int(p-json)+11];
  jam = atoi(kata);
//  Serial.println(jam);

  //menit
  kata[0]=json[int(p-json)+13];
  kata[1]=json[int(p-json)+14];
  menit = atoi(kata);
//  Serial.println(menit);

  //detik
  kata[0]=json[int(p-json)+16];
  kata[1]=json[int(p-json)+17];
  detik = atoi(kata);
//  Serial.println(detik);
  
  setTime(jam, menit, detik, hari, bulan, tahun);

  //Keluarkan tanggal di serial monitor
  hapus();
  Serial.print(F("YYYY/MM/DD HH:MM:SS="));
  strcat(tanggal, "20");
  strcat(tanggal, itoa(tahun,json,10));
  strcat(tanggal, "/");
  strcat(tanggal, int2digits(bulan).c_str());
  strcat(tanggal, "/");
  strcat(tanggal, int2digits(hari).c_str());
  strcat(tanggal, " ");
  strcat(tanggal, int2digits(jam).c_str());
  strcat(tanggal, ":");
  strcat(tanggal, int2digits(menit).c_str());
  strcat(tanggal, ":");
  strcat(tanggal, int2digits(detik).c_str());
  Serial.println(tanggal);
}

void waktuTunggu() {
  indeks = minute();
  Serial.print("Tidur hingga ");
  for (byte i = 0; i < 60 / interval; i++) {
    if (indeks >= 60 - interval) {
      Serial.print(60 - indeks);
      menit = 60 - indeks;
      break;
    }
    if (indeks >= i * interval && indeks < (i + 1) * interval) {
      Serial.print((i + 1) * interval - indeks);
      menit = (i + 1) * interval - indeks;
      break;
    }
  }
  Serial.println(" menit >> ");

  while (minute() < indeks + menit) {
    Serial.print(int2digits(hour()));
    Serial.write(":");
    Serial.print(int2digits(minute()));
    Serial.write(":");
    Serial.println(int2digits(second()));
    indikatorOK();
  }
}

String int2digits(int number) {
  String y = "";
  if (number >= 0 && number < 10) {
    y += "0";
  }
  y += String(number);
  return y;
}

