#include "OneButton.h"
#include "timer-api.h"

#include "LoopMode0.h"


enum SvetoforModes
{
    MODE_OFF = 0,
    MODE_RED_ON = 1,
    MODE_YELLOW_ON = 2,
    MODE_GREEN_ON = 3,
    MODE_ALL_ON = 4,
    MODE_LM0 = 5,
    MODES_COUNT
};

unsigned currMode = MODE_OFF;
bool modeIsSet = false;
bool isBeeping = false;
uint8_t beepsMax = 10U;
uint8_t beepsDone = 0U;


OneButton btnForward(A0, true);
OneButton btnBackward(A1, true);

void setup()
{
    Serial.begin(115200);
//  while(!Serial) {;} //Wait for Leonardo
    if (Serial)
    {
        Serial.println("Svetofor starts");
        Serial.print("Mode: ");
        Serial.println(currMode, DEC);
    }

    timer_init_ISR_100Hz(TIMER_DEFAULT);

    btnForward.attachClick(onClickForward);
    btnBackward.attachClick(onClickBackward);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    pinMode(SPEAKER, OUTPUT);

    startBeeping();
}

void loop()
{
    btnForward.tick();
    btnBackward.tick();

    delay(5);
}

void onClickForward()
{
    if (currMode < MODES_COUNT - 1)
    {
        currMode += 1;
    }
    else
    {
        currMode = MODE_OFF;
    }
    modeIsSet = false;

    if (Serial)
    {
        Serial.print("Mode: ");
        Serial.println(currMode, DEC);
    }
}

void onClickBackward()
{
    if (currMode > MODE_OFF)
    {
        currMode -= 1;
    }
    else
    {
        currMode = MODES_COUNT - 1;
    }
    modeIsSet = false;

    if (Serial)
    {
        Serial.print("Mode: ");
        Serial.println(currMode, DEC);
    }
}

inline void startBeeping()
{
    isBeeping = true;
    beepsDone = 0;
}

void timer_handle_interrupts(int timer)
{
    static unsigned long prevTime = 0;

    unsigned long currTime = micros();
    unsigned long deltaTime = currTime - prevTime;
    prevTime = currTime;

    if (isBeeping)
    {
        if (beepsDone == beepsMax)
        {
            isBeeping = false;
            digitalWrite(SPEAKER, LOW);
        }
        else
        {
            beepsDone += 1;
            digitalWrite(SPEAKER, !digitalRead(SPEAKER));
        }
    }

    switch (currMode) {
    case MODE_OFF:
        if (modeIsSet)
        {
            return;
        }
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_GREEN, LOW);
        modeIsSet = true;
        startBeeping();
        break;

    case MODE_RED_ON:
        if (modeIsSet)
        {
            return;
        }
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_GREEN, LOW);
        modeIsSet = true;
        startBeeping();
        break;

    case MODE_YELLOW_ON:
        if (modeIsSet)
        {
            return;
        }
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_YELLOW, HIGH);
        digitalWrite(LED_GREEN, LOW);
        modeIsSet = true;
        startBeeping();
        break;

    case MODE_GREEN_ON:
        if (modeIsSet)
        {
            return;
        }
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_GREEN, HIGH);
        modeIsSet = true;
        startBeeping();
        break;

    case MODE_ALL_ON:
        if (modeIsSet)
        {
            return;
        }
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_YELLOW, HIGH);
        digitalWrite(LED_GREEN, HIGH);
        modeIsSet = true;
        startBeeping();
        break;

    case MODE_LM0:
        if (!modeIsSet)
        {
            LoopMode0::reset();
            modeIsSet = true;
            startBeeping();
        }
        LoopMode0::update(deltaTime);
        break;

    default:
        if (Serial)
        {
            Serial.print("Unknown mode: ");
            Serial.println(currMode);
        }
        break;
    }
}
