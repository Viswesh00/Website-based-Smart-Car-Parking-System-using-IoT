#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define codeLength 7

LiquidCrystal lcd(5, 4, 3, 2, A4, A5);
Servo ServoMotor;

byte keycount=0;
char Code[codeLength];
char PassW[codeLength]="238743";
//You can change the password

int position = 0;
int lcdflag = 0;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[ROWS] = { A0, A1, 11, 10 };
byte colPins[COLS] = { 9, 8, 7, 6 };
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int LCDCol = 0;
int LCDRow = 0;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(LCDCol, LCDRow);
  ServoMotor.attach(A3);
  ServoMotor.write(0);
 // LockedPosition(true);
  lcd.print("Enter PIN");
}

void loop(){
  char key = keypad.getKey();
  if(key){
    
    lcd.setCursor (LCDCol, 1); 
    lcd.print(key);
    ++LCDCol;
   if(LCDCol==6) {
     delay(1000);
     LCDCol=0;
     lcd.clear();
     lcd.print("Enter PIN");
     lcd.setCursor(LCDCol, 1);
   }
    
    Code[keycount]=key;
    Serial.print(Code[keycount]);
    keycount++;
  }
  if(keycount==codeLength-1){
    Serial.println(" ");
    
    if(!strcmp(Code,PassW)){
      Serial.println("Correct");
      lcd.clear();
      lcd.print("Welcome");
      ServoMotor.write(80); //unlock
      delay(4000);
      lcd.clear();
      lcd.print("Enter PIN");
      ServoMotor.write(0); //lock

    }
       else{
         Serial.println("Incorrect");
         lcd.clear();
         lcd.print("Invalid PIN");
         delay(3000);
         lcd.clear();
         lcd.print("Enter PIN");
       }
    
    deleteCount();
  }

}
  
void deleteCount(){
  while(keycount !=0){
    Code[keycount--]=0;
  }
  return;
}
