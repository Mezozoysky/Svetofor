#include "LoopMode0.h"
#include "Pins.h"


void LoopMode0::onReset()
{
    mCurrFrame = 0;
    applyFrame();
}

void LoopMode0::onUpdate(unsigned long deltaTime)
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

LoopMode0::LoopMode0()
{}

void LoopMode0::applyFrame()
{
    digitalWrite(LED_RED, mFrames[mCurrFrame].red ? HIGH : LOW);
    digitalWrite(LED_YELLOW, mFrames[mCurrFrame].yellow ? HIGH : LOW);
    digitalWrite(LED_GREEN, mFrames[mCurrFrame].green ? HIGH : LOW);
    mFrameTimeElapsed = 0;
}

Frame LoopMode0::mFrames[3] = {
    Frame(true, false, false, 1UL * 1000000UL),
    Frame(false, true, false, 1UL * 1000000UL),
    Frame(false, false, true, 1UL * 1000000UL)
};

uint8_t LoopMode0::mCurrFrame = 0U;
unsigned long LoopMode0::mFrameTimeElapsed = 0UL;
