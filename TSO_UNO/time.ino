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
  tahun = '0'; bulan = '0'; hari = '0'; jam = '0'; menit = '0'; detik = '0';

  //AMBIL WAKTU DARI PROVIDER
//  Serial.println("ambil waktu");
  SIM7000.println(F("AT+CCLK?"));
  SIM7000.flush();
  bacaserial(500);
  delay(300);

  //PISAHKAN DATA WAKTU
  //tahun
  indeks2 = kalimat.indexOf("/", indeks1 + 1);
//  Serial.println(indeks2);
  tahun = kalimat.substring(indeks2-2, indeks2).toInt();
//  Serial.println(tahun);

  //bulan
  indeks1 = kalimat.indexOf("/", indeks2 + 1);
  bulan = kalimat.substring(indeks2 + 1, indeks1).toInt();

  //hari
  indeks2 = kalimat.indexOf(",", indeks1 + 1);
  hari = kalimat.substring(indeks1 + 1, indeks2).toInt();

  //jam
  indeks1 = kalimat.indexOf(":", indeks2 + 1);
  jam = kalimat.substring(indeks2 + 1, indeks1).toInt();

  //menit
  indeks2 = kalimat.indexOf(":", indeks1 + 1);
  menit = kalimat.substring(indeks1 + 1, indeks2).toInt();

  //detik
  indeks1 = kalimat.indexOf("+", indeks2 + 1);
  detik = kalimat.substring(indeks2 + 1, indeks1).toInt();

  //Keluarkan tanggal di serial monitor
  Serial.print(F("YYYY/MM/DD HH:MM:SS="));
  Serial.print("20");
  Serial.print(tahun);
  Serial.write("/");
  Serial.print(bulan);
  Serial.write("/");
  Serial.print(hari);
  Serial.write(" ");
  Serial.print(int2digits(jam));
  Serial.write(":");
  Serial.print(int2digits(menit));
  Serial.write(":");
  Serial.println(int2digits(detik));
  setTime(jam, menit, detik, hari, bulan, tahun);
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

