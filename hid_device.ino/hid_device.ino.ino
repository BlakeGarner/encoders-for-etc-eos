
void setup() {
  Serial.begin(9600);
  Serial.println(F("Blake's RawHID Test"));
  for (int i = 0; i < 7; i++) {
    pinMode(i, OUTPUT);
  }
}

// RawHID packets are always 64 bytes
byte buffer[64];
elapsedMillis msUntilNextSend;
unsigned int packetCount = 0;

void loop() {
  int n;
  n = RawHID.recv(buffer, 0); // 0 timeout = do not wait.
  if (n > 0) {
    // The computer sent a message. Display the bits of the first byte on pins 0 to 7.
    // Ignore the other 63 bytes.
    Serial.print(F("Received packet, first byte: "));
    Serial.println((int) buffer[0]);
    for(int i = 0; i < 8; i++) {
      int b = buffer[0] & (1 << i);
      digitalWrite(i, b);
    }
  }

  // Every 2 seconds, send a packet to the computer.
  if (msUntilNextSend > 2000) {
    msUntilNextSend = msUntilNextSend - 2000;
      // First 2 bytes are a signature.
      buffer[0] = 0xAB;
      buffer[1] = 0xCD;
      // Next 24 bytes are analog measurements.
      for(int i = 0; i < 12; i++) {
        int val = analogRead(i);
        buffer[i * 2 + 2] = highByte(val);
        buffer[i * 2 + 3] = lowByte(val);
      }
      // Fill the rest with zeros.
      for (int i = 26; i < 64; i++) {
        buffer[i] = 0;
      }
      // And put a count of packets sent at the end.
      buffer[62] = highByte(packetCount);
      buffer[63] = lowByte(packetCount);
      // Actually send the packet.
      n = RawHID.send(buffer, 100);
      if (n > 0) {
        Serial.print(F("Transmit packet "));
        Serial.println(packetCount);
        packetCount = packetCount + 1;
      }
      else {
        Serial.println(F("Unable to transmit packet"));
    }
  }
}
