#include "TRM.h"

TRM::TRM(const byte& dwnBtn, const byte& upBtn, const byte& setBtn, const byte& strtBtn,
        const byte& nmbBtn, const byte& mtrBtn, const byte& sckPin, const byte& csPin, const byte& soPin)
        : _upButton(upBtn), _downButton(dwnBtn), _numberButton(nmbBtn)
{
    //downButton.ini(dwnBtn);
    //upButton.ini(upBtn);
    settingsButton.ini(setBtn);
    startStopButton.ini(strtBtn);
    //numberButton.ini(nmbBtn);
    motorButton.ini(mtrBtn);
    termoCouple.ini(sckPin, csPin, soPin);
}

float TRM::getTemperature()
{ 
    return termoCouple.readCelsius();
}
/*
void TRM::chooseNumberPause()
{

    if (numberButton.click())
    {
        numberPause++;
    }
    if (numberPause == 4)
    {
        numberPause = 1;
    }
}
*/


byte TRM::getNumberPause() const { return numberPause; }


//Кнопки up down создаются через динамическую память
void TRM::enterTemperaturePauses(){
    temperaturePausesStruct pauseEnter = collector.getPause(numberPause);
    MyButton *upButton = new MyButton(_upButton);
    MyButton *downButton = new MyButton(_downButton);
    for(byte i = 0; i < 6; i++){
        byte temp1 = pauseEnter.setpointTemperature[i];
        uint16_t time1 = pauseEnter.time[i];
        while(!startStopButton.click()){ //ввод температуры
            if(downButton->click()){temp1--;}            
            if(upButton->click()){temp1++;}
            temp1 = constrain(temp1, 0, 255);
        }
        while(!startStopButton.click()){ //ввод времени
            if(downButton->click()){time1--;}
            if(upButton->click()){time1++;}
        }
        pauseEnter.setpointTemperature[i] = temp1;
        pauseEnter.time[i] = time1;
    }
    collector.writePauses(numberPause, pauseEnter);
    delete downButton, upButton; 
}

void TRM::settings(){ //настройки 1-паузы, 2-вкл?выкл wifi, 3-параметры по мощности, 4-диапазон времени(сек/мин), 5- отложенное время
    if(settingsButton.pressed()){
        MyButton *upButton = new MyButton(_upButton);
        MyButton *downButton = new MyButton(_downButton);
        byte i = 0;
        while(1){
            if(settingsButton.pressed()) break;
            if(upButton->click()){i++; i = constrain(i, 0, 4);}// Перемещение кнопками up down
            if(downButton->click()){i--; i = constrain(i, 0, 4);}
            if(startStopButton.click()){
                switch (i)
                {
                case 0: //Настройки паузы (ok)
                    MyButton *numberButton = new MyButton(_numberButton);
                    while(1){//сначала выбираем номер паузы, затем вводим паузу
                        if(settingsButton.pressed()){break;}
                        if(numberButton->click()){numberPause++; if(numberPause==3){numberPause = 0;}}
                        if(startStopButton.click()){enterTemperaturePauses();}
                    }
                    delete numberButton;
                    break;
                case 1: // вкл/выкл wifi
                    while(1){

                    }
                    break;
                case 2: // параметры по мощности (ok)
                    byte j = 0;
                    while(1){
                        if(settingsButton.pressed()){break;}
                        if(upButton->click()){j++; j = constrain(j, 0, 1);}
                        if(downButton->click()){j--; j = constrain(j, 0, 1);}
                        if(startStopButton.click()){
                            while(1){
                                if(settingsButton.click()){break;}
                                if(upButton->click() && j == 0){powerMax++; powerMax = constrain(powerMax, 0, 100);}
                                if(upButton->click() && j == 1){powerMin++; powerMax = constrain(powerMax, 0, 100);}
                                if(downButton->click() && j == 0){powerMax--; powerMax = constrain(powerMax, 0, 100);}
                                if(downButton->click() && j == 1){powerMin--; powerMax = constrain(powerMax, 0, 100);}
                            }
                            //надо сохранить все данные в EEprom
                        }
                    }
                    break;
                case 3: // выбор времени sec/min (ok!)
                    while(1){
                        if(settingsButton.pressed()){break;}
                        if(upButton->click()){
                            if(timeSet){timeSet = false;}
                            else{timeSet = true;}
                        }
                        if(downButton->click()){
                            if(!timeSet){timeSet = true;}
                            else{timeSet = false;}
                        }
                    }
                    //надо сохранить в EEprom
                    break;
                case 4: //отложенное время
                    while(1){
                        if(settingsButton.pressed()){break;}
                        if(upButton->click()){
                            if(timeDelay){timeDelay = false;}
                            else{timeDelay = true;}
                        }
                        if(downButton->click()){
                            if(!timeDelay){timeDelay = true;}
                            else{timeDelay = false;}
                        }
                    }
                default:
                    break;
                }
            }
        }
        delete upButton, downButton;
    }
}

void TRM::main_programm(){
    settings();

}

void TRM::WiFiConnect(){ //Подключение к WiFi в режиме сначала раздачи, а затем приема
  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;
  //wifiManager.resetSettings(); //перезапись имени wifi каждый запуск
  bool res; //храним переменную для подключения
  res = wifiManager.autoConnect("PVK"); //подключение телефона к точке
  if(!res){
    ESP.restart();
  }
  else{wifiOn = true;}
}