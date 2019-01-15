#include <Keyboard.h>
#include <SPI.h>

/*
 * Keyboard Manager for NovoPoker
 * Author: Ayex
 * Date: 11-01-2018
 * 
 */

//Lamps
#define LAMP_1    0;
#define LAMP_2    0;
#define LAMP_3    0;
#define LAMP_4    0;
#define LAMP_5    0;
#define LAMP_6    0;
#define LAMP_7    0;
#define LAMP_8    0;

#define MACHINE_CURRST    0 // Current state of machine


//Keys PINS
#define OPERATOR_KEY        2
#define SUPERVISOR_KEY      3
#define DOOR_SWITCH         4
#define REMOTE_OUT          5
#define ENTER_KEY           6
//#define AVAILABLE_1       7
//#define AVAILABLE_1       8
//#define AVAILABLE_1       9
#define RED_BET             10
#define BLACK               14
#define DEAL_TAKE           15
#define HOLD1               16
#define HOLD2               18
#define HOLD3               19
#define HOLD4               20
#define HOLD5               21


#define DOOR_SW_ASSW        true
#define SUPERVISOR_KEY_ASSW true
#define OPERATOR_KEY_ASSW   true


#define COIN1               50
#define COIN2               51
#define COIN3               53
#define COIN4               54

#define SENTPIN_TOKYB       false //Si es false manda los pin por serial y no como key press a la botonera

#define ArrayLength         12


#define EXIT                0xB1

#define DELAY               100
#define SERIAL_BR           9600


int keypadPins[ArrayLength]          = {OPERATOR_KEY,SUPERVISOR_KEY,DOOR_SWITCH,REMOTE_OUT, RED_BET, BLACK, DEAL_TAKE, HOLD1,HOLD2, HOLD3, HOLD4, HOLD5};
int keypadPinsEnabled [ArrayLength];
int keypadStatus;          
  
bool doorSwitchStatus     = false;    //Contain status of Door's Switch true=OPEN ; false = CLOSED
bool operatorKeyStatus    = false;    //Contain status of Operator's Switch true=Activated;false=not activated;
bool supervisorKeyStatus  = false;  
bool keyboardLock         = false;    //Contains status for keyboard locked or not locked true=LOCKED ; false=UNLOCKED

int timeout;
void setup()
{
  Serial1.begin(SERIAL_BR);
  
  for (int i = 0; i < ArrayLength; i++)
  {
      keypadPinsEnabled[i] = true;
      pinMode(keypadPins[i], INPUT);  
      digitalWrite(keypadPins[i], HIGH);
  }
}


/***************** Primary Loop *************************/

void loop()
{
  if(Serial1.available() > 0)
  {
      char serialData = Serial1.read();
      switch(serialData)
      {
        case 'D':
          if (doorSwitchStatus)
            Serial1.write('N');
          else
            Serial1.write('D');
          delay(DELAY);
          break;
         case 'K':
           keyboardLock = true;
           break;
         case 'k':
            keyboardLock = false;
           break;
         default:
           break;
      }

      
      Keyboard.press(serialData);
      delay(DELAY);
      Keyboard.release(serialData);
  }
   
  getKeypadStatus();
  
}


//#######################################################
//######                  SERIAL                   ######
//######                                           ######
//#######################################################
void getSerialData()
{

  if(Serial1.available() > 0)
  {
      char serialData = Serial1.read();
      Keyboard.press(serialData);
      delay(DELAY);
      Keyboard.releaseAll();
  }
}




//#######################################################
//######                  KEYBOARD                 ######
//######                                           ######
//#######################################################
void getKeypadStatus()
{
  
  
  //Check door switch if pressed /then keyboard enabled else keyboard disabled Pin Close = low
  if(digitalRead(DOOR_SWITCH)==0)
  {   
     if(doorSwitchStatus)
     { 
        Keyboard.press('O');
        delay(DELAY);
        Keyboard.press('O');
        delay(DELAY);
        Keyboard.release('O');
        Serial1.write('D');
        delay(DELAY);
     }
     doorSwitchStatus = false; 
  }else{
    if(!doorSwitchStatus)
    {
      Serial1.write('N');
      //delay(DELAY);
      Keyboard.press('O');
      delay(DELAY);
    }
    doorSwitchStatus = true;
  }
  
  /***** Operator Key *****/
  char a = '0';   //0
  char so = 'Q';  //Q
  char sc = 'q';  //q
  if(digitalRead(OPERATOR_KEY)==0)
  {  
      if(!operatorKeyStatus)
     {  
        Keyboard.press(a);
        delay(DELAY);
        Serial1.write(so);
     }
     operatorKeyStatus = true;
  }else{
      if(operatorKeyStatus){
         Keyboard.release(a);
         delay(DELAY);
         Keyboard.press(a);
         delay(DELAY);
         Serial1.write(sc);
         Keyboard.release(a);
         //delay(DELAY);
      }
      operatorKeyStatus = false;
  }

  a = '0';   
  so = 'P';  
  sc = 'p';  
  /************* SUPERVISOR COLLECT HP *************/
  
  if(digitalRead(SUPERVISOR_KEY)==0)
  {  
     if(!supervisorKeyStatus)
     {  
        Keyboard.press(a);
        delay(DELAY);
        Serial1.write(so);
     }
     supervisorKeyStatus = true;
  }else{
      if(supervisorKeyStatus){
         Keyboard.release(a);
         delay(DELAY);
         Keyboard.press(a);
         delay(DELAY);
         Serial1.write(sc);
         Keyboard.release(a);
      }
      supervisorKeyStatus = false;
  }

  if(keyboardLock) return;
   
  for (int i = 0; i < 12; i++)
  {
      if (digitalRead(keypadPins[i]) == 0)
      {
        if (keypadPinsEnabled[i] == true)
        {
          sendKeyPress(keypadPins[i]);
        }
      }
  }
}



void sendKeyPress(int key)
{
      switch (key)
      { 
          case ENTER_KEY:
            if(SENTPIN_TOKYB)
            {
              Keyboard.press('\n');
            }else{
              //sendSerialData('\n');
              Serial1.write('\n');
            }
            break;
          case OPERATOR_KEY:
            
            /*
            if (SENTPIN_TOKYB) {Keyboard.press('9');
            
             }else{
              //sendSerialData('9');
              Serial1.write('9');
            }
            */
            break;
          case SUPERVISOR_KEY:
            if (SENTPIN_TOKYB) {Keyboard.press('0');
             }else{
              //sendSerialData('0');
              Serial1.write('0');
            }
            break;
          case DOOR_SWITCH:
            //doorSwitchStatus =!doorSwitchStatus;
            //Keyboard.write('O');
            break;
          case REMOTE_OUT:
            if(SENTPIN_TOKYB) {Keyboard.press('4'); 
            }else{
              //sendSerialData('4');
              Serial1.write('4');
            }
            break;
          case RED_BET:
            if(SENTPIN_TOKYB) {Keyboard.press('A');
             }else{
              //sendSerialData('A');
              Serial1.write('A');
            }
            break;
          case BLACK:
            if(SENTPIN_TOKYB)  {Keyboard.press('S');
             }else{
              //sendSerialData('S');
              Serial1.write('S');
            }
            break;
          case DEAL_TAKE:
            if(SENTPIN_TOKYB) { Keyboard.press('2');
             }else{
              //sendSerialData('2');
              Serial1.write('2');
            }
            break;
          case HOLD1:
            if(SENTPIN_TOKYB) { Keyboard.press('Z');
             }else{
              //sendSerialData('Z');
              Serial1.write('Z');
            }
            break;
          case HOLD2:
            if(SENTPIN_TOKYB) { Keyboard.press('X');
             }else{
              //sendSerialData('X');
              Serial1.write('X');
            }
            break;
          case HOLD3:
            if(SENTPIN_TOKYB) { Keyboard.press('C');
             }else{
              //sendSerialData('C');
              Serial1.write('C');
            }
            break;
          case HOLD4:
            if(SENTPIN_TOKYB) { Keyboard.press('V');
             }else{
              //sendSerialData('V');
              Serial1.write('V');
            }
            break;
          case HOLD5:
            if(SENTPIN_TOKYB) { Keyboard.press('B');
             }else{
              //sendSerialData('B');
              Serial1.write('B');
            }
            break;
          case COIN1:
            if(SENTPIN_TOKYB) { Keyboard.press('5');
             }else{
              //sendSerialData('5');
              Serial1.write('5');
            }
            break;
          case COIN2:
            if(SENTPIN_TOKYB) { Keyboard.press('6');
             }else{
              //sendSerialData('6');
              Serial1.write('6');
            }
            break;
          case COIN3:
            if(SENTPIN_TOKYB) { Keyboard.press('7');
             }else{
              //sendSerialData('7');
              Serial1.write('7');
            }
            break;
          case COIN4:
            if(SENTPIN_TOKYB) { Keyboard.press('8');
             }else{
              //sendSerialData('8');
              Serial1.write('8');
            }
            break;
           case EXIT:
            if(SENTPIN_TOKYB) { Keyboard.press(EXIT);
             }else{
              //sendSerialData('EXIT');
              Serial1.write('EXIT');
            }
            break;
     }
     delay(DELAY);
     //Keyboard.releaseAll();
}
