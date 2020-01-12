void sensor() {
  unsigned long nilai = 0;
  float pressVolt = 0;
  tekanan = 0;
  
  for (indeks = 0; indeks < burst; indeks++) {
    int pressADC = analogRead(pressurePin);
    nilai += pressADC;
  }

  pressVolt = float(float (nilai / burst) / 1024.00 * 5.00);
  tekanan = (3.00 * pressVolt - 1.5) - offset; // satuan bar
}

void dataJSON(){
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
}
