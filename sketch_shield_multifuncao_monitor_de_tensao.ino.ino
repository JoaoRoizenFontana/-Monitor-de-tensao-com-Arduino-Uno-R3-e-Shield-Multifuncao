#include <Wire.h>
#include <string.h> 

#include <TimerOne.h>
#include <MultiFuncShield.h>
#include <Wire.h> 

#define HIGH_VOLTAGE_LIMIT 5.0
#define LOW_VOLTAGE_LIMIT 0.0
#define VOLTAGE_UNIT 0.0049
enum MonitorModeValues
{
MONITORING_STOPPED,
MONITORING_STARTED,
SETTING_HIGH_TH_STARTED,
SETTING_LOW_TH_STARTED
};
enum ThresholdStatusValues
{
 THRESHOLD_NOT_SET,
 THRESHOLD_SET
};
enum ledModeValues
{
LED_ALL_OFF,
LED_1_ON,
LED_2_ON,
LED_3_ON,
LED_4_ON
};

enum MonitorModeValues monitor = MONITORING_STOPPED;
float hi = HIGH_VOLTAGE_LIMIT;
float lo = LOW_VOLTAGE_LIMIT;
int hiConfig = 0;
int loConfig = 0;
char buff[4];

void setup(){
    Timer1.initialize();
    MFS.initialize(&Timer1);
    MFS.writeLeds(LED_ALL, OFF);
    Serial.begin(9600);
    sprintf(buff,"off");
}

void loop()
{
    byte btn = MFS.getButton();
    
    switch(monitor){
        case MONITORING_STOPPED:
        
            MFS.write(buff);
            MFS.blinkDisplay(DIGIT_ALL,OFF);
            if(btn == BUTTON_1_SHORT_RELEASE && hiConfig && loConfig){
                sprintf(buff,"off");
                monitor = MONITORING_STARTED;
                break;
            }
            else if(btn == BUTTON_3_LONG_PRESSED){
                MFS.blinkDisplay(DIGIT_1,ON);
                MFS.blinkDisplay(DIGIT_2,ON);
                monitor = SETTING_LOW_TH_STARTED;
                sprintf(buff,"ht%s",String(hi,1).c_str());
                MFS.write(buff);
                break;
            }
            else if(btn == BUTTON_2_LONG_PRESSED){
                MFS.blinkDisplay(DIGIT_1,ON);
                MFS.blinkDisplay(DIGIT_2,ON);
                monitor = SETTING_HIGH_TH_STARTED;
                sprintf(buff,"lt%s",String(lo,1).c_str());
                MFS.write(buff);
                break;
            }
            
            if(btn == BUTTON_2_PRESSED){
                sprintf(buff,"ht%s",String(hi,1).c_str());
                MFS.write(buff);
            }
            if(btn == BUTTON_3_PRESSED){
                sprintf(buff,"lt%s",String(lo,1).c_str());
                MFS.write(buff);
                
            }
            
            break;
        case SETTING_HIGH_TH_STARTED:
            sprintf(buff,"ht%s",String(hi,1).c_str());
            MFS.write(buff);
            
            if(btn == BUTTON_1_SHORT_RELEASE){
                monitor = MONITORING_STOPPED;
                sprintf(buff,"off");
                hiConfig = 1;
                MFS.writeLeds(LED_1, ON);
                break;
            }
            else if((btn == BUTTON_2_SHORT_RELEASE) ||(btn == BUTTON_2_LONG_PRESSED) ){
                hi+=0.1;
                if(hi<=HIGH_VOLTAGE_LIMIT){
                  sprintf(buff,"ht%s",String(hi,1).c_str());
                  MFS.write(buff);
                }
                else hi-=0.1;
                break;
            }
            else if(btn == BUTTON_3_SHORT_RELEASE ||(btn == BUTTON_3_LONG_PRESSED)){
                hi-=0.1;
                sprintf(buff,"ht%s",String(hi,1).c_str());
                MFS.write(buff);
                break;
            }
            break;
        case SETTING_LOW_TH_STARTED:
            sprintf(buff,"lt%s",String(lo,1).c_str());
            MFS.write(buff);

            if(btn == BUTTON_1_SHORT_RELEASE){
                sprintf(buff,"off");
                monitor = MONITORING_STOPPED;
                loConfig = 1;
                MFS.writeLeds(LED_2, ON);
                break;
            }
            else if(btn == BUTTON_2_PRESSED ||(btn == BUTTON_2_LONG_PRESSED)){
                delay(100);
                lo+=0.1;
                sprintf(buff,"lt%s",String(lo,1).c_str());
                MFS.write(buff);
                break;
            }
            else if((btn == BUTTON_3_PRESSED) ||(btn == BUTTON_3_LONG_PRESSED)){
                lo-=0.1;
                if(lo>=LOW_VOLTAGE_LIMIT){
                sprintf(buff,"lt%s",String(lo,1).c_str());
                MFS.write(buff);
                }
                else lo+=0.1;
                break;
            }
            break;
        case MONITORING_STARTED:
            
            MFS.writeLeds(LED_1, ON);
            delay(100);
            MFS.writeLeds(LED_2, ON);
            delay(100);
            MFS.writeLeds(LED_3, ON);
            delay(100);
            MFS.writeLeds(LED_4, ON);
            delay(100);
            MFS.writeLeds(LED_ALL, OFF);
            
            float volt = analogRead(POT_PIN)*VOLTAGE_UNIT;
            if(volt>hi){
                sprintf(buff,"hi%s",String(volt,1).c_str());
                MFS.write(buff);
                MFS.beep(5,5,4,1,10);
                delay(500);
            }
            else if(volt<lo){
              sprintf(buff,"lo%s",String(volt,1).c_str());
                MFS.write(buff);
                MFS.beep(5,5,4,1,10);
                delay(500);
            }
            
            else{
              sprintf(buff,"on%s",String(volt,1).c_str());
              MFS.write(buff);
            }
            MFS.blinkDisplay(DIGIT_1,ON);
            MFS.blinkDisplay(DIGIT_2,ON);
            if(btn == BUTTON_1_SHORT_RELEASE || btn == BUTTON_1_LONG_RELEASE){
                monitor = MONITORING_STOPPED;
                sprintf(buff,"off");
                hiConfig = 0;
                loConfig = 0;
                break;
            }
            else if(btn == BUTTON_2_LONG_PRESSED){
                MFS.blinkDisplay(DIGIT_ALL,OFF);
                sprintf(buff,"ht%s",String(hi,1).c_str());
                MFS.write(buff);
                break;
            }
            else if(btn == BUTTON_3_LONG_PRESSED){
                MFS.blinkDisplay(DIGIT_ALL,OFF);
                sprintf(buff,"lt%s",String(lo,1).c_str());
                MFS.write(buff);
                break;
            }
            break;
    }
}
