
extern "C" void printbegin(int baudrate) {
  Serial.begin(baudrate);
}


extern "C" void println(void) {
  Serial.println();
}

extern "C" void printstr(char* str) {
  Serial.print(str);
}

extern "C" void printint(uint32_t num) {
  Serial.print(num, HEX);
}

extern "C" void printframe(uint32_t id, uint8_t data_len, uint8_t * data) {
  if (id > 0x7ff) {
    Serial.print("Extended ID: 0x");
    if (id < 0x10000000) Serial.print("0");
    if (id < 0x1000000) Serial.print("00");
    if (id < 0x100000) Serial.print("000");
    if (id < 0x10000) Serial.print("0000");
    Serial.print(id, HEX);
  } else {
    Serial.print("Standard ID: 0x");
    if (id < 0x100) Serial.print("0");
    if (id < 0x10) Serial.print("00");
    Serial.print(id, HEX);
    Serial.print("     ");
  }
  Serial.print(" DLC: ");
  Serial.print(data_len);
  Serial.print(" Data: ");
  for(int i=0;i<data_len;i++) {
    Serial.print("0x");
    Serial.print(data[i], HEX);
    if (i != (data_len-1)) Serial.print(" ");
  }
  Serial.println();
}
