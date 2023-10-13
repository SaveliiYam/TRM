## Терморегулятор на базе микроконтроллера ESP32.
Имеет возможность управлять регулятором при помощи кнопок с выводом информации на lcd экран по интерфейсу I2C. Также использует веб-интерфейс.

## Функции
* Возможность по желанию подключиться к wifi сразу при включении. Во время работы подключение невозможно.
* Регуляция происходит при помощи ПИД регулятора.
* ПИД регулятор можно откалибровать. Для этого необходимо ввести условно максимальную температуру и запустить процесс.
* Мощность подается очень короткими импульсами (255 миллисекунд максимум)
* Можно записать в память до 7 программ по 6 шагов в каждой
* Диапазон времени ограничен двумя часами (120 минут).
* В настройках можно выбрать режим отображения времени в секундах или минутах.
* Также можно использовать режим задержки времени (программа не начнет отсчитывать время пока не достигнет нужного значения уставки)
* К ацп можно подключить термопару типа K. При неправильных показаниях можно ввести калибровочное значение.
* Данные настройки можно применять во время выполнения рабочей программы.
* Все настройки автоматически сохраняются в память устройства и восстанавливаются при запуске.
* Веб интерфейс позволяет вводить точно такие же настройки.

## Сборка 
Необходим PlatformIO. С помощью него можно загрузить прошивку. Разрабатывался под ESP32-WROOM-32. А значит программу можно использовать с любым dev-kit на базе этого МК.

## Схемы

![Электрическая схема](/images/Schematic.png)


## Используемые библиотеки
* Thinger.io. Для реализации веб интерфейса. Ключи находятся в файле arduino_secrets.h.
* GyverPID. Для реализации ПИД регулирования. 
* WiFiManager. Для удоббного подключения к WiFi.
* Остальные библиотеки были написаны самостоятельно и находятся в папке src.
* LiquidCrystal_I2C. Отвечает за работу с LCD диспеем по интрефейсу I2C.

## Пошаговый ход программы
* МК опрашивает пользователя, желает ли он подключиться к WiFi.
* В случае положительного ответа МК раздает WiFi точку, к которой подключается пользователь и вводит параметры домашней точки.
* При успешном подключении на экране высвечивается уведомление об успешном подключении. В противном случае уведомление будет отрицательным и устройство будет перезагружено.
* В случае успешного подключения можно использовать веб интерфейс Thinger для управления всеми процессами.
* Контроллер ожидает действий от пользователя (вход в настройки, старт программы).
* При использовании веб интерфейса, пользователь может использовать привычное управление кнопками, однако программу невозможно будет начать, если веб интерфейс активен.


## Описание классов и структур
Для всех классов и структур кроме ```TRM``` и ```LCD``` приведен список используемых методов и полей.
### struct temperaturePausesStruct
```cpp
    struct temperaturePausesStruct{                             
    uint8_t setpointTemperature[6];
    uint32_t time[6];
    };
```
Данная структура отвечает за одну температурную программу. В ходе выполнения программы данная структура может записываться в память или читаться из нее. К видно, температурная программа состоит из двух массивов по 6 элементов в каждой. setpoint определяет температуру уставки, а time определяет время поддержания температуры уставки.

### struct parametrs
```cpp
struct parametrs
{
    parametrs(const bool &timeSet, const bool &timeDelay, const byte &powerMax, const byte &powerMin, const float &calibr, const byte &predel)
        : _timeSet(timeSet), _timeDelay(timeDelay), _powerMax(powerMax), _powerMin(powerMin), _calibr(calibr), _predel(predel) {}
    parametrs() {}
    bool _timeSet;
    bool _timeDelay;
    byte _powerMax = 255, _powerMin = 0, _predel = 101;
    float _calibr = 0;
};
```
Данная структура используется для получения и записи настроек времени, мощности и калибровочных значений.

###struct Koefficients 
```cpp
struct Koefficients
{
    float P = 1;
    float I = 1;
    float D = 1;
};

```
Структура хранящая ПИД коэффициенты.

Структуры ```parametrs``` и ```Koefficients``` при создании иницализируются стандартными значениями для возможного сброса настроек.

### class TRM
* Основной класс всей программы, использует интерфейсы ниже приведенных классов для вывода и приема информации. Отвечает за правильное сохранение всех настроек и температурных программ. 
* Предоставляет метод ввода настроек с помощью кнопок или веб-интерфейса.
* Отвечает на команды пользователя по вводу температурной программы, посылает команды для всех используемых классов.
* В основном ```loop``` цикле ожидает ввода каких-либо команд и возвращает выходное значение для нагреваетля.
### class LCD
* Отвечает за вывод всей информации на экран.
* Использует библиотеку LiquidCrystal_I2C. 
* Хранит в себе весь текст, выводящийся на экран.
* Т.к. был выбран lcd экран с возможностью вывода кириллицы, то все надписи были закодированы в 16-ном виде.
* Вывод нужного текста осуществляется через вызов нужного метода класса с передачей ему нужных методов. 
### class TemperatureProgramm
```cpp
class TemperatureProgramm{
private:
    temperaturePausesStruct _pausesStruct;

public:
    TemperatureProgramm(const temperaturePausesStruct& pauses);//При создании объекта этого класса будет считана переданная температурная программа
    TemperatureProgramm(const byte& adress);//При создании объекта этого класса будет считана температурная программа из переданной ячейки
    ~TemperatureProgramm(){}
    Pair<byte, uint32_t> readPause(const byte& pause) const;//Получение одной паузы
    void writePauses(const byte& adress, const temperaturePausesStruct& structWrite);//Запись программы с дальнейшим её сохранением в ячейку
    temperaturePausesStruct getPauseAll(); //Получение последней используемой программы
    void savePauses(const int& adress); //Запись последней используемой программы 
};
```
Данный класс служит для записи и получения температурной программы прмо из ячейки и дальнейшей передаче её вызывающему методу. Данный класс использует класс коллектор, передавая ему либо номер ячейки для получения, либо температурную программу для записи.
### class TemperaturePauseCollector
```cpp
class TemperaturePauseCollector
{
private:
    byte _numberProgramms[7] = {0, 1, 2, 3, 4, 5, 6};
    byte _adress[7] = {0, 35, 70, 105, 140, 175, 210}; // ячейки в которые будут записываться паузы
public:
    TemperaturePauseCollector() {}
    void writePauses(const byte &numberProgramm, const temperaturePausesStruct &pause);//запись программы в ячейку
    Pair<byte, uint32_t> readPause(const byte &numberProgramm, const byte &numberPause); //получили одну паузу из программы
    temperaturePausesStruct getPause(const byte &numberProgramm); //получает программу из ячейки
};
```
Данный класс служит для записи и получения температурных программ. Хранит порядковые номера программ последовательно. Каждой программе соответствует определенная ячейка памяти. Получение программы происходит по передаче порядкового номера программы. Запись происходит по передаче порядкового номера программы и самой программе. Для записи и получения использует класс TemperatureProgramm. 

### class ProgrammRunner
```cpp
class ProgrammRunner
{
private:
    temperaturePausesStruct programm_;
    bool timeSet = false;
    bool timeDelay = false;
    bool startTime = false;
    bool can_to_start = false;
    bool is_stopped = false;
    uint32_t timer = millis();
    uint32_t ticker = 0;
    byte numberPause = 0;

public:
    ProgrammRunner(const temperaturePausesStruct &programm, const bool &time, const bool &delay);
    ProgrammRunner(const bool &time, const bool &delay);
    ProgrammRunner();
    void putTimeSet(const bool &time);
    void putTimeDelay(const bool &delay);
    void putTimeSettings(const bool &time, const bool &delay); //Ввод настроек всех времени одним методом
    void putProgramm(const temperaturePausesStruct &programm); // Ввод температуры нагрева
    bool is_programm_run() const; // если программа закончена, то false
    void programm_stop(); // Выполняет остановку программы
    void startPause(); // Выполняет запуск программы нагрева 
    Third<byte, uint32_t, byte> runningProgramm(const float &temperature); 
    //Метод, который активно ведет отсчет времени температурной программы и возвращает порядковый номер программы, оставшееся время выполняемой паузы и температуру уставки программы. 
    //Для реализации правильной работы задержки нагрева необходимо передавать температуру 
};
```
Данный класс служит для начала старта или остановки выполнения программы нагрева. Использует настройки времени (при активированной настройки задержки времени не начинает остчет времени пока не будет достигнута температура уставки). Инициализацию класса можно производить без параметров или передав ей параметры времени и температурную программу, которая будет выполняться.

### class PIDRegulator
```cpp
class PIDRegulator
{
private:
    Koefficients koefficients;
    GyverPID regulator;
    PIDtuner tuner;
    bool tune = false;
    Pair<byte, byte> parametrsForLCD{0, 0};

public:
    PIDRegulator();

    // Надо каждый раз вводить лимиты
    void setLimits(const byte &minValue, const byte &maxValue); // Ограничение выходной мощности в процентах
    // Получить значение
    int getValuePID(const int &temperatureNow);
    // Внести температуру уставки
    void putTemperature(const int &setTemperature);
    // Надо вызывать перед каждым тюном
    void tuneInitialization(const float &temperatureNeed);
    // тюн
    Third<bool, byte, byte> tunePID(const int &temperatureNow);
    Pair<byte, byte> GetPIDValueTune(const int &temperatureNow);
    // вставить коэффициенты в регулятор
    void enterPIDKoefficients(const Koefficients &koefficients);
    //
    void loadKoefficients();
    void saveKoefficients();
    void baseKoefficients();
    bool getTuneInfo() const;
    Pair<byte, byte> GetParametrsLCD() const;// Получение значений для передачи их на дисплей
};
```
Данный класс использует ПИД регулятор, хранит в себе ПИД коэффициенты. Позоволяет производить корректировку коэффициентов и регулировку максимальной и минимальной вызодной мощности в процентах. Для расчетов и корректировки использует библиотеку ПИд регулятора.