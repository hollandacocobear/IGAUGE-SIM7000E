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
