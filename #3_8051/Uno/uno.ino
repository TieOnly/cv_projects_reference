void setup()
{
  Serial.begin(2400);
}

void loop()
{
  uint8_t c = '\0';
  if(Serial.available())
  {
      c = Serial.read();
      Serial.write(c);
  }
}
