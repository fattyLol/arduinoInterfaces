int LED = 10 ;
int RECIEVER = 52;
int SENDER = 53;
int count;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(SENDER, OUTPUT);
  pinMode(RECIEVER, INPUT);
  Serial.begin(9600); // Debugging only
  Serial.println("Recieving started"); //Prints "Setup" to the serial monitor
}
String alphabet = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int gettingTransaction = 0;
String MyStr;
String msg;

void loop(){
  
  if(gettingTransaction == 3){
    gettingTransaction = 0;
    char msg = recieveMsg();
    Serial.println(msg);
  }
  
  if(Serial.available() > 1){
    
    int num = charToInt();
    serialFlush();
    msg = toBinaryString(num);
    
    int ready = 0;
    int frame;
    
    while(ready != 10){
      Serial.println("From the loop");
      frame = digitalRead(RECIEVER);
      if (frame == 0){
        ready++;
      } else {
        ready = 0;
      }
        delay(200);
    }
    
    sendMsg(msg);
  }
  
  if (digitalRead(RECIEVER) != 0){
    gettingTransaction ++;
  } else {
    gettingTransaction = 0;
  }
  digitalWrite(SENDER, LOW);
  delay(200);
}

//Очистка буфера
void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}
  
int charToInt(){
  int num = alphabet.indexOf(Serial.read());  
  return num;
}

String toBinaryString(int num){
  String binNum = String(num, BIN); // int -> BIN
  while(binNum.length() < 8){
    binNum = "0" + binNum;
  }
  return binNum;
}

void sendMsg(String msg){
  
  for(int i = 0; i < 3; i++){
    digitalWrite(SENDER,HIGH);
    delay(200);
  }
  
  char digits[8];
  msg.toCharArray(digits,9);
  
  for(int i = 0 ; i < 8 ; i++ ){
    if(digits[i] == '0'){
      digitalWrite(SENDER, LOW);
    } else {
      digitalWrite(SENDER, HIGH);
    }
  delay(200);
  }
}

char recieveMsg(){

  char bits[9];
  int portValue;
  String msg = "";
  for(int i = 0; i < 8; i++){
    portValue = digitalRead(RECIEVER);
  
    if (portValue == 0){
     bits[i] = '0';
      msg += "0";
    } else {
      bits[i] = '1';
        msg += "1";
    }
  delay(200);
  }
  
  int index = binaryToDigital(bits);
  
  char a = alphabet[index];
  
  return a;
}

int binaryToDigital(char bits[8]){

  int num = 0;
  int power = 7;
 
  
  for(int i = 0; i < 8 ; i++){
    if (bits[i] == '1'){
      int a = powerForInts(2, power);  
      Serial.print("a = "); 
      Serial.println(a); 
      num += a;
      Serial.print("num = ");
     Serial.println(num);
    }
    power--;
  }

  return num;
}

int powerForInts(int base, int power){
  int num = 1;

  for(int i = 0; i < power; i++){
    num *= base;
  }

  return num;
}
