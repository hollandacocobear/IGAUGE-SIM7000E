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

void dataJSON() {
  //http://www.mantisid.id/project/osh_2019/api/product/osh_data_c.php?{"Data":"003,20190315213555,2456,02356,02201,01233,05,15"}
  // {"Data":"id unit,yyyymmddhhmmss,tekanan(5 digit),kelembaban(5 digit),suhu(5 digit),volt (5 digit), burst (2 digit), interval (2 digit)"}
  hapus();
  strcat(json, "{\"Data\":\"");

  //ID
  strcat(json, ID);
  strcat(json, ",");
  //  Serial.println(data);

  //tanggal
  strcat(json, tanggal);
  strcat(json, ",");
  //  Serial.println(data);
  kalimat = String(tekanan, 2);
  strcat(json, kalimat.c_str());
  strcat(json, ",-9999,-9999,-9999,");
//  Serial.println(json);

  strcat(json,String(burst).c_str());
  strcat(json,",");
  strcat(json,String(interval).c_str());
  strcat(json,"\"}");
  Serial.println(json);
  Serial.flush();
}
