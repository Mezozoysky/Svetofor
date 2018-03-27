/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

#include "OneButton.h"
#include "timer-api.h"

#define LED_RED 7
#define LED_YELLOW 4
#define LED_GREEN 2

template<typename DerivedSModeT>
struct LoopModeMixin
{
    /* using BaseType LoopModeMixin<DerivedSModeT>; */
    /* using ActualType DerivedSModeT; */

    inline static void reset()
    {
        DerivedSModeT::onReset();
        /* ActualType::onReset(); */
    }

    inline static void update(unsigned long deltaTime)
    {
        DerivedSModeT::onUpdate(deltaTime);
        /* ActualType::onUpdate(deltaTime); */
    }

protected:
    LoopModeMixin()
    {}
};

struct Frame
{
    bool red;
    bool yellow;
    bool green;
    unsigned long frameTime;

    Frame()
    : red(false)
    , yellow(false)
    , green(false)
    , frameTime(0UL)
    {}

    Frame( bool aRed, bool aYellow, bool aGreen, unsigned long aFrameTime )
    : red(aRed)
    , yellow(aYellow)
    , green(aGreen)
    , frameTime(aFrameTime)
    {}
};

struct LoopMode0
: public LoopModeMixin<LoopMode0>
{
    static void onReset()
    {
        mCurrFrame = 0;
        applyFrame();
    }

    static void onUpdate(unsigned long deltaTime)
    {
        if (mFrameTimeElapsed >= mFrames[mCurrFrame].frameTime)
        {
            mCurrFrame += 1;
            if ( mCurrFrame >= 3 )
            {
                mCurrFrame = 0;
            }
            applyFrame();
            return;
        }

        mFrameTimeElapsed += deltaTime;
    }

private:
    LoopMode0()
    {}

    static void applyFrame()
    {
        digitalWrite(LED_RED, mFrames[mCurrFrame].red ? HIGH : LOW);
        digitalWrite(LED_YELLOW, mFrames[mCurrFrame].yellow ? HIGH : LOW);
        digitalWrite(LED_GREEN, mFrames[mCurrFrame].green ? HIGH : LOW);
        mFrameTimeElapsed = 0;
    }

private:
    static Frame mFrames[3];
    static uint8_t mCurrFrame;
    static unsigned long mFrameTimeElapsed;
};

Frame LoopMode0::mFrames[3] = {
    Frame(true, false, false, 1UL * 1000000UL),
    Frame(false, true, false, 1UL * 1000000UL),
    Frame(false, false, true, 1UL * 1000000UL)
};

uint8_t LoopMode0::mCurrFrame = 0U;
unsigned long LoopMode0::mFrameTimeElapsed = 0UL;


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

OneButton btn0(A0, true);
OneButton btn1(A1, true);

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

    btn0.attachClick(click0);
    btn1.attachClick(click1);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    delay(700);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    delay(100);
}

void loop()
{
    btn0.tick();
    btn1.tick();

    delay(5);
}

void click0()
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

void click1()
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

void timer_handle_interrupts(int timer)
{
    static unsigned long prevTime = 0;

    unsigned long currTime = micros();
    unsigned long deltaTime = currTime - prevTime;
    prevTime = currTime;


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
        break;

    case MODE_LM0:
        if (!modeIsSet)
        {
            LoopMode0::reset();
            modeIsSet = true;
        }
        LoopMode0::update(deltaTime);
        break;

    default:
        Serial.print("Unknown mode: ");
        Serial.println(currMode);
        break;
    }
}
