#include <Keypad.h> //open library for keypad
#include <SoftwareSerial.h> //open library for gsm and bluetooth
#include <LiquidCrystal.h> //open library for LCD

SoftwareSerial gsm(52, 16); //set pins for gsm, 52 for rx and 16 for tx
SoftwareSerial bluetooth(50, 18); //set pins for gsm, 50 for rx and 18 for tx

char IncData; //declare a char variable for receive message
int y = 0; //declare an int variable for number pressed for keypad

int condition=0; //declare a interger variable for condition
char password[5]="1234"; //declare a char array for the password with max characters of 5
int loc=6; //declare an interger variable named loc
int pass=0; //declare an interger variable named pass
int correct=0;// declare an interger variable named correct
int mode; //declare an interger variable name mode
char bt; //declare a char variable named bt

char RcvdMsg[60] = ""; //declare char array variable with maximum characters of 60
int RcvdCheck = 0; //declare an interger with initial value of 0
int RcvdConf = 0; //declare an interger with initial value of 0
int index = 0; //declare an interger with initial value of 0
int RcvdEnd = 0; //declare an interger with initial value of 0
char MsgTxt[10]; //declare a char array variable with maximum characters of 10
int MsgLength = 0; //declare an interger with initial value of 0

int a;
int x;
int i;
char key;

int stat1;
int stat2;

int Gsm1_Init(int value1);
int Gsm2_Init(int value2);

void RecBT();
void Gsm_Status();
void RecSMS();
void ClearSMS();
void Config();
void ChangeMode();

const byte ROWS = 2; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'A','B','C','D'}
};
byte rowPins[ROWS] = {12, 13}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {

      stat1=0; //to indicate the current status of device 1, the default status is 0 which means off
      stat2=0; //to indicate the current status of device 2, the default status is 0 which means off
      a=0;
      mode=0; //to indicate the current mode of system, 1 for gsm, 2 for bluetooth
            
      pinMode(A0,OUTPUT); //to set pin A0 as an output
      pinMode(A1,OUTPUT); //to set pin A1 as an output
      pinMode(A2,OUTPUT); //to set pin A2 as an output
      pinMode(A3,OUTPUT); //to set pin A3 as an output
      
      lcd.begin(16, 2); //to define how many rows and columns for LCD
      lcd.clear(); //to clear the display of LCD
      
      lcd.setCursor(3,0); //set column at column 4, set row at row 1
      lcd.print("WELCOME TO"); //display "WELCOME TO" on LCD

      lcd.setCursor(5,1); //set column at column 4, set row at row 2
      lcd.print("SHCS v2.0"); //display "GBSHCS v2.0" on LCD
      delay(3000); //delay for 3 seconds
      lcd.clear(); //to clear the display of LCD

      lcd.setCursor(0,0); //set column at column 1, set row at row 1
      lcd.print("CHECKING"); //display "CHECKING" on LCD
      lcd.setCursor(0,1); //set column to column 1, set row to row 2
      lcd.print("CONNECTIONS..."); //display "CONNECTIONS..." on LCD
      delay(5000); //delay for 3 seconds
      lcd.clear(); //to clear the display of LCD

      lcd.setCursor(0,0); //set column to column 1, set row to row 1
      lcd.print("CONNECTIONS OKAY"); //display "CONNECTIONS OKAY" on lcd
      delay(2000); //delay for 2 seconds
      lcd.clear(); //to clear the display of LCD
      
      lcd.setCursor(2,0); //set column to column 3, set row to row 1
      lcd.print("CHOOSE YOUR"); //display "CHOOSE YOUR" on LCD
      lcd.setCursor(5,1); //set column to column 6, set row to row 2
      lcd.print("MODE:"); //display "MODE:" on LCD
      delay(3000); //delay for 3 seconds
      lcd.clear(); //to clear the display of LCD

      do{ //start a loop

        key = customKeypad.getKey(); //variable key equal to key pressed on the keypad

        lcd.setCursor(0,0); //set column to column 1, set row to row 1
        lcd.print("4: GSM"); //display "A:GSM" on LCD
        lcd.setCursor(0,1); //set column to column 1, set row to row 2
        lcd.print("5: BLUETOOTH"); //display "B:BLUETOOTH" on LCD

        switch(key){

          case 'A':
          lcd.clear();
          lcd.setCursor(5,0);
          lcd.print("GSM");
          lcd.setCursor(5,1);
          lcd.print("MODE");
          delay(3000);
          lcd.clear();
          delay(100);
          gsm.begin(9600);
          delay(100);
          Config();
          mode=1;
          a=1;
          break;

          case 'B':
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("BLUETOOTH");
          lcd.setCursor(5,1);
          lcd.print("MODE");
          delay(3000);
          lcd.clear();
          delay(100);
          bluetooth.begin(9600);
          delay(100);
          mode=2;
          a=1;
          break;
          
        }
      }while(a!=1);
      
    a=0;
  
}


void loop(){

      if(stat1==1 && stat2==1 ){
      lcd.setCursor(0,0);
      lcd.print("D1=ON");
      lcd.setCursor(0,1);
      lcd.print("D2=ON");
      }
      else if(stat1==0 && stat2==0){
      lcd.setCursor(0,0);
      lcd.print("D1=OFF");
      lcd.setCursor(0,1);
      lcd.print("D2=OFF");
      }
      else if(stat1==1 && stat2==0){
      lcd.setCursor(0,0);
      lcd.print("D1=ON");
      lcd.setCursor(0,1);
      lcd.print("D2=OFF");
      }
      else if(stat1==0 && stat2==1){
      lcd.setCursor(0,0);
      lcd.print("D1=OFF");
      lcd.setCursor(0,1);
      lcd.print("D2=ON");
      }  

if(mode==1){

  RecSMS();
  
}else if(mode==2){

  RecBT();
}

      key = customKeypad.getKey();

      if(key){
        Serial.println(key);
      }
      
      switch (key){

      case '1':

      digitalWrite(A0, HIGH);
      digitalWrite(A2, HIGH);
      lcd.clear();
      stat1=1;
      break;

      case '2':

      digitalWrite(A0, LOW);
      digitalWrite(A2, LOW);
      lcd.clear();
      stat1=0;
      break;

      case '3':

      digitalWrite(A1, HIGH);
      digitalWrite(A3, HIGH);
      lcd.clear();
      stat2=1;
      break;
 
      case '4':

      digitalWrite(A1, LOW);
      digitalWrite(A3, LOW);
      lcd.clear();
      stat2=0;
      break;

      case 'C':
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ENTER PASSWORD");

      do{

        key = customKeypad.getKey();
        
        if(key>0){
        lcd.setCursor(loc, 1);
        lcd.print("*");
        loc++;
        y++;
     }
  
  if(key == password[x]){
    x++;
  }

  if(x==4 && y==4){

    delay(1000);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("CORRECT");
    lcd.setCursor(2,1);
    lcd.print("PASSWORD");
    delay(2000);
    ChangeMode();
    x=0;
    y=0;
    loc=6;
    condition=1;
  }else if(y==4 && x!=4){

    delay(1000);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("INCORRECT");
    lcd.setCursor(2,1);
    lcd.print("PASSWORD");
    delay(2000);
    lcd.clear();
    loop();
    x=0;
    loc=6;
    y=0;
    condition=1;
  }
        
      }while(condition!=1);
      
      condition=0;
      break;

      case 'D':

      if(mode==1){
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("SYSTEM IS IN");
        lcd.setCursor(4,1);
        lcd.print("GSM MODE");
        delay(3000);
        lcd.clear();
      }
 
        else if(mode==2){
          
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("SYSTEM IS IN");
        lcd.setCursor(4,1);
        lcd.print("BT MODE");
        delay(3000);
        lcd.clear();
        
         }
         break;
         
       }
      
  }

void ChangeMode(){

lcd.clear();
lcd.setCursor(2,0);
lcd.print("CHOOSE YOUR");
lcd.setCursor(6,1);
lcd.print("MODE:");
delay(3000);
lcd.clear();

  do{

    key = customKeypad.getKey();

    lcd.setCursor(0,0);
    lcd.print("4: GSM");
    lcd.setCursor(0,1);
    lcd.print("5: BLUETOOTH");

    switch(key){

      case 'A':

      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("GSM");
      lcd.setCursor(5,1);
      lcd.print("MODE");
      delay(3000);
      lcd.clear();
      delay(100);    
      bluetooth.end();
      delay(100);
      gsm.begin(9600);
      delay(100);
      Config();
      mode=1;
      a=1;
      break;

      case 'B':

      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("BLUETOOTH");
      lcd.setCursor(5,1);
      lcd.print("MODE");
      delay(3000);
      lcd.clear();

      delay(100);
      gsm.end();
      delay(100);
      bluetooth.begin(9600);
      delay(100);
      mode=2;
      a=1;
      break;

      }
 
  }while(a!=1);

  a=0;
    
}

void RecBT(){

  if (bluetooth.available()>0){
    bt= bluetooth.read();
  }

  if(bt=='a'){
    digitalWrite(A0, HIGH);
    digitalWrite(A2, HIGH);
    stat1=1;
    lcd.clear();
  }else if(bt=='b'){
    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    stat1=0;
    lcd.clear();
  }else if(bt=='c'){
    digitalWrite(A1, HIGH);
    digitalWrite(A3, HIGH);
    stat2=1;
    lcd.clear();
  }else if(bt=='d'){
    digitalWrite(A1, LOW);
    digitalWrite(A3, LOW);
    stat2=0;
    lcd.clear();
  }

  bt=0;
    
}


int Gsm1_Init(int value1){

switch(MsgTxt[1]){

case '1':
 
 if(stat1==0){

     digitalWrite(A0, HIGH);
     digitalWrite(A2, HIGH);
 
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1 IS TURN ON");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
     value1 = 1;
      }
      else if(stat1==1){
        
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1 IS ALREADY ON");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
     
      }
      break;
      
case '0':

       if(stat1==1){
       
       digitalWrite(A0, LOW);
       digitalWrite(A2, LOW);
       
       gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
       delay(1000);
       gsm.println("D1 IS OFF");// The SMS text you want to send
       delay(100);
       gsm.println((char)26);// ASCII code of CTRL+Z
       delay(1000);
       value1=0;
       }
       else if(stat1==0){
        
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1 IS ALREADY OFF");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
       }
       break;
       
      }
               ClearSMS();
               lcd.clear();
               return value1;
      }

      
int Gsm2_Init(int value2){

switch(MsgTxt[1]){

case '1':

 if(stat2==0){

     digitalWrite(A1, HIGH);
     digitalWrite(A3, HIGH);
     
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D2 IS ON");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
     value2 = 1;
      }
      else if(stat2==1){
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D2 IS ALREADY ON");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
      }
      break;

case '0':

       if(stat2==1){
     digitalWrite(A1, LOW);   
     digitalWrite(A3, LOW);
     
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D2 IS OFF");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
      value2=0;
       }
       else if(stat2==0){
     gsm.println("AT+CMGS=\"0137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D2 IS ALREADY OFF");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
       }
       break;

      }
               ClearSMS();
               lcd.clear();
               return value2;
      }
      
void Gsm_Status(){

if(stat1==1 && stat2==1 ){
  
     gsm.println("AT+CMGS=\"+60137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1=ON & D2=ON");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
      }
      else if(stat1==0 && stat2==0){
        
     gsm.println("AT+CMGS=\"+60137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1=OFF & D2=OFF");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
      }
      else if(stat1==1 && stat2==0){
        
     gsm.println("AT+CMGS=\"+60137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1=ON & D2=OFF");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
      }
      else if(stat1==0 && stat2==1){
        
     gsm.println("AT+CMGS=\"+60137946211\"\r"); // Replace x with mobile number
     delay(1000);
     gsm.println("D1=OFF & D2=ON");// The SMS text you want to send
     delay(100);
     gsm.println((char)26);// ASCII code of CTRL+Z
     delay(1000);
      }
      ClearSMS();
}

void RecSMS(){
  
        if (gsm.available()>0)
        {
          IncData = gsm.read();
         
          if(IncData == '+'){RcvdCheck = 1;}
          if((IncData == 'C') && (RcvdCheck == 1)){RcvdCheck = 2;}
          if((IncData == 'M') && (RcvdCheck == 2)){RcvdCheck = 3;}
          if((IncData == 'T') && (RcvdCheck == 3)){RcvdCheck = 4;}
          if((IncData == ':') && (RcvdCheck == 4)){RcvdCheck = 5;}
          if(RcvdCheck == 5){index = 0;RcvdConf = 1; RcvdCheck = 0;}

          if(RcvdConf == 1)
          { 
              Serial.print(IncData);
              if(IncData == '\n'){
              RcvdEnd++;
              }
              
              RcvdMsg[index] = IncData;
              index++;
              
              if(RcvdEnd == 2){RcvdConf = 0;
              MsgLength = index;
              index = 0;
              }
              if(RcvdConf == 0)
              {
                  for(x = 44; x < MsgLength; x++)
                  {
                      MsgTxt[x-44] = RcvdMsg[x];
                  }
                    if(MsgTxt[0]=='a'){  
                       stat1= Gsm1_Init(stat1);
                       }
                       
                    if(MsgTxt[0]=='b'){
                       stat2= Gsm2_Init(stat2);
                       }
                       
                    if(MsgTxt[0]=='s'){
                       Gsm_Status();
                       }
              }
           }
        }
}

void ClearSMS()
{
    strcpy(RcvdMsg,"");
    RcvdCheck = 0;
    RcvdConf = 0;
    index = 0;
    RcvdEnd = 0;
    strcpy(MsgTxt,"");
    MsgLength = 0;
}    

void Config()
{

  delay(100);
  Serial.begin(9600);
  delay(100);
  gsm.print("ATE0\r");
  delay(100);
  gsm.print("AT+CMGF=1\r");
  delay(100);
  gsm.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  gsm.print("AT+CMGD=1,4");
  delay(100);
}    
