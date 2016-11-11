void setup(){
  Serial.begin(9600);
  delay(100);
  Serial.println("Serial started");
}

void loop(){
  String message = "131";
  byte bytes[message.length() + 1];
  message.getBytes(bytes, message.length() + 1);
  Serial.print(message);
  Serial.print(" = ");
  for (int i = 0; i < message.length(); i++){
    if (i > 0){
      Serial.print(" "); 
    }
    Serial.print(bytes[i]);
  }
  Serial.println();
}
