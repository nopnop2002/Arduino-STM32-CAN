
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
  Serial.print("ID: 0x");
  Serial.print(id, HEX);
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
