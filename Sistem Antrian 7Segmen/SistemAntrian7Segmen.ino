/* Sebuah Sistem Antrian berbasis 7 segmen dan IC4511
 * 
 * 
 */

/* PIN TOMBOL TELLER */
#define t1 0 //pin tombol teller 1
#define t2 1 //pin tombol teller 2
#define t3 2 //pin tombol teller 3

/* PIN SELECTOR */
#define ss 5 //pin selector satuan
#define sp 6 //pin selector puluhan
#define sr 7 //pin selector ratusan
#define st 8 //pin selector teller

/* PIN BIT-BIT ANGKA */
#define b0 10 //pin bit ke-0
#define b1 11 //pin bit ke-1
#define b2 12 //pin bit ke-2
#define b3 13 //pin bit ke-3

int antrian[3] = {0, 0, 0}; // antrian pada teller berdasarkan index
/*
   Index 0 -> t1
   Index 1 -> t2
   Index 2 -> t3
*/

bool terklik; //latching tombol agar tidak naik terus menerus

void setup() {

  for (byte t = t1; t <= t3; t++) { //mendefinisikan semua pin tombol adalah INPUT_PULLUP
    pinMode(t, INPUT_PULLUP);
  }

  for (byte s = ss; s <= st; s++) { //mendefinisikan semua pin selector adalah OUTPUT
    pinMode(s, OUTPUT);
  }

  for (byte b = b0; b <= b3; b++) { //mendefinisikan semua pin bit angka adalah OUTPUT
    pinMode(b, OUTPUT);
  }

}

void loop() {
  /* KETIKA TOMBOL TELLER DITEKAN */
  for (byte t = t1; t <= t3; t++) { //UNTUK SETIAP TELLER
    if (!digitalRead(t) and !terklik) {
      setAntrianPadaTeller(t, antrian[t] + 1); //naikkan nomor antrian sebesar 1
      terklik = true;
      break;
    }
  }

  if (digitalRead(t1) and digitalRead(t2) and digitalRead(t3)) { //lepas latching jika semua tombol tidak ditekan
    terklik = false;
  }

delay(1);
}

void setBitAngka(byte bit3, byte bit2, byte bit1, byte bit0) { //FUNGSI INDEPENDEN
  //Mengatur nilai antrian dalam format biner
  digitalWrite(b0, bit0);
  digitalWrite(b1, bit1);
  digitalWrite(b2, bit2);
  digitalWrite(b3, bit3);
}

void setAngka(byte angka) { //FUNGSI DEPENDEN: setBitAngka
  //Mengatur nilai antrian dalam format desimal
  if (angka == 1) {
    setBitAngka(0, 0, 0, 1);
  }
  else if (angka == 2) {
    setBitAngka(0, 0, 1, 0);
  }
  else if (angka == 3) {
    setBitAngka(0, 0, 1, 1);
  }
  else if (angka == 4) {
    setBitAngka(0, 1, 0, 0);
  }
  else if (angka == 5) {
    setBitAngka(0, 1, 0, 1);
  }
  else if (angka == 6) {
    setBitAngka(0, 1, 1, 0);
  }
  else if (angka == 7) {
    setBitAngka(0, 1, 1, 1);
  }
  else if (angka == 8) {
    setBitAngka(1, 0, 0, 0);
  }
  else if (angka == 9) {
    setBitAngka(1, 0, 0, 1);
  }
  else {
    setBitAngka(0, 0, 0, 0);
  }
}

void setSelector(String s) { //FUNGSI INDEPENDEN
  //Mengatur selector digit angka yang mau aktifkan ke IC sehingga dapat menerima input bit angka.
  if (s == "satuan") {
    digitalWrite(ss, LOW);
    digitalWrite(sp, HIGH);
    digitalWrite(sr, HIGH);
    digitalWrite(st, HIGH);
  }
  else if (s == "puluhan") {
    digitalWrite(ss, HIGH);
    digitalWrite(sp, LOW);
    digitalWrite(sr, HIGH);
    digitalWrite(st, HIGH);
  }
  else if (s == "ratusan") {
    digitalWrite(ss, HIGH);
    digitalWrite(sp, HIGH);
    digitalWrite(sr, LOW);
    digitalWrite(st, HIGH);
  }
  else if (s == "teller") {
    digitalWrite(ss, HIGH);
    digitalWrite(sp, HIGH);
    digitalWrite(sr, HIGH);
    digitalWrite(st, LOW);
  }
  else {
    digitalWrite(ss, HIGH);
    digitalWrite(sp, HIGH);
    digitalWrite(sr, HIGH);
    digitalWrite(st, HIGH);
  }
}

void setDigit(String s, byte a) { //FUNGSI DEPENDEN: setSelector, setAngka
  //mengatur digit angka sekaligus memasukkan nilai pada digit tersebut
  setSelector(s); //membuka selektor tertentu
  setAngka(a);
  setSelector("tutup"); //menutup semua selektor
}

void setBilangan(int antri) { //FUNGSI DEPENDEN: setDigit
  //Memecah bilangan menjadi digit satu persatu kemudian mengatur digit angka sekaligus memasukkan nilai digit satu persatu
  int ratusan = antri / 100;
  int puluhan = (antri - (ratusan * 100)) / 10;
  int satuan = (antri - (ratusan * 100)) - (puluhan * 10);
  setDigit("satuan", satuan);
  setDigit("puluhan", puluhan);
  setDigit("ratusan", ratusan);
}

void setAntrianPadaTeller(byte noTeller, int antri) { //FUNGSI DEPENDEN: setDigit, setBilangan
  //Mengatur nilai antrian pada teller
  if (antri > 999) { //JIKA NOMOR ANTRIAN MELAMPAUI BATAS
    antri = 1;
  }

  antrian[noTeller] = antri; //simpan nomor antrian di RAM
  setDigit("teller", noTeller + 1); //digit pada teller tidak mungkin 0 karena noTeller selalu ditambah 1.
  setBilangan(antri);

}
