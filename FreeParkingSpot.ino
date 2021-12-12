String myAPIkey = "WVK1H12C611RLRHM";  
#include <SoftwareSerial.h>
SoftwareSerial ESP8266(3, 2); // Rx, Tx

int humidity = 10;
int temp_f = 100;  
long writingTimer = 17; 
long startTime = 0;
long waitTime = 0;

boolean relay1_st = false; 
boolean relay2_st = false; 
unsigned char check_connection=0;
unsigned char times_check=0;
boolean error;

void setup()
{
  Serial.begin(9600); 
  ESP8266.begin(9600); 

  startTime = millis(); 
  ESP8266.println("AT+RST");
  delay(2000);
  Serial.println("Connecting to Wifi");
   while(check_connection==0)
  {
    Serial.print(".");
  ESP8266.print("AT+CWJAP=\"lokvis_2.4\",\"act12345\"\r\n");
  //  ESP8266.print("AT+CWJAP=\"Wifi Network's name\",\"PassWord\"\r\n");
  ESP8266.setTimeout(5000);
 if(ESP8266.find("WIFI CONNECTED\r\n")==1)
 {
 Serial.println("WIFI CONNECTED");
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
   Serial.println("Trying to Reconnect..");
  }
  }
}

void loop()
{
  waitTime = millis()-startTime;   
  if (waitTime > (writingTimer*1000)) 
  {
    writeThingSpeak();
    startTime = millis();   
  }
}

void writeThingSpeak(void)
{
  startThingSpeakCmd();
  // preparacao da string GET
  String getStr = "GET /update?api_key=";
  getStr += myAPIkey;
  getStr +="&field1=";
  getStr += String(temp_f);
  getStr +="&field2=";
  getStr += String(humidity);
  getStr += "\r\n\r\n";
  GetThingspeakcmd(getStr); 
}

void startThingSpeakCmd(void)
{
  ESP8266.flush();
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com IP address
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.print("Start Commands: ");
  Serial.println(cmd);

  if(ESP8266.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}
String GetThingspeakcmd(String getStr)
{
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ESP8266.println(cmd);
  Serial.println(cmd);

  if(ESP8266.find(">"))
  {
    ESP8266.print(getStr);
    Serial.println(getStr);
    delay(500);
    String messageBody = "";
    while (ESP8266.available()) 
    {
      String line = ESP8266.readStringUntil('\n');
      if (line.length() == 1) 
      { 
        messageBody = ESP8266.readStringUntil('\n');
      }
    }
    Serial.print("MessageBody received: ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    ESP8266.println("AT+CIPCLOSE");     
    Serial.println("AT+CIPCLOSE"); 
  } 
}

Tinkercad simulation code:

#include <LiquidCrystal.h>
int cm1 = 0;
int cm2 = 0 ;
int cm3 = 0;
int total = 0;
String ssid     = "Simulator Wifi";  // SSID to connect to 

String password = ""; // Our virtual wifi has no password (so dont do your banking stuff on this network) 

String host     = "api.thingspeak.com"; // Open Weather Map API 
const int httpPort   = 80; 
String uri     = "/update?api_key=IAQWOZQFNFRZQ9EH&field1="; 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int setupESP8266(void) { 
  // Start our ESP8266 Serial Communication 
  Serial.begin(115200);   // Serial connection over USB to computer 
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266 
  delay(10);        // Wait a little for the ESP to respond 
  if (!Serial.find("OK")) return 1;
  // Connect to 123D Circuits Simulator Wifi 
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) return 2; 
// Open TCP connection to the host: 
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort); 
  delay(50);        // Wait a little for the ESP to respond 
  if (!Serial.find("OK")) return 3;
  return 0; 
}  
void anydata(int count) { 
  //int temp = map(analogRead(A0),20,358,-40,125); 
  // Construct our HTTP call 
  String httpPacket = "GET " + uri + String(count) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n"; 
  int length = httpPacket.length(); 
  // Send our message length 
  Serial.print("AT+CIPSEND="); 
  Serial.println(length); 
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1; 
  // Send our http request 
  Serial.print(httpPacket); 
  delay(10); // Wait a little for the ESP to respond 
  if (!Serial.find("SEND OK\r\n")) return; 
} 
int inches = 0;
int cm = 0;
long readUltrasonicDistance(int triggerPin, int echoPin)
{
 pinMode(triggerPin, OUTPUT); // Clear the trigger
 digitalWrite(triggerPin, LOW);
 delayMicroseconds(2);
 // Sets the trigger pin to HIGH state for 10 microseconds
 digitalWrite(triggerPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerPin, LOW);
 pinMode(echoPin, INPUT);
 // Reads the echo pin, and returns the sound wave travel time in microseconds
 return pulseIn(echoPin, HIGH);
}
// initialize the library with the numbers of the interface pins
void setup() {
 // set up the LCD's number of columns and rows:
 lcd.begin(16, 2);
 Serial.begin(9600);
 setupESP8266(); 
}

void loop() {
 int count = 0;
 // set the cursor to column 0, line 1
 // (note: line 1 is the second row, since counting begins with 0):
 lcd.setCursor(10, 1);
 cm1 = 0.01723 * readUltrasonicDistance(7, 7);
 cm2 = 0.01723 * readUltrasonicDistance(6, 6);
 cm3 = 0.01723 * readUltrasonicDistance(8, 8);
 if (cm1 > 150)
 {
 count++;
 }
 if(cm2 > 150)
 {
 count++;
 }
 if(cm3 > 150)
 {
 count++;
 }
 anydata(count);
 lcd.setCursor(0,0);
 lcd.print(count);
 lcd.print(" free ");
 lcd.print("slots");
 lcd.setCursor(0,1);
 lcd.print("Slot ");
 if (cm3 > 150)
 lcd.print("1 ");
 if (cm2 > 150)
 lcd.print("2 ");
 if (cm1 > 150)
 lcd.print("3 ");
 lcd.print("free");
 if (cm3<150 & cm2<150 & cm1<150)
 lcd.print(" = 0");
 delay(500);
 lcd.clear();

}
