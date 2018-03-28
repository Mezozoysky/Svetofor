#pragma once

#include "Arduino.h"
#include "Pins.h"

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

template<typename DerivedSModeT, uint8_t framesNumberV>
struct LoopModeMixin
{
    /* using BaseType LoopModeMixin<DerivedSModeT>; */
    /* using ActualType DerivedSModeT; */

    inline static void reset()
    {
        mCurrFrame = 0;
        applyFrame();

        DerivedSModeT::onReset();
        /* ActualType::onReset(); */
    }

    inline static void update(unsigned long deltaTime)
    {
        if (mFrameTimeElapsed >= mFrames[mCurrFrame].frameTime)
        {
            mCurrFrame += 1;
            if (mCurrFrame >= framesNumberV)
            {
                mCurrFrame = 0;
            }
            applyFrame();
            return;
        }

        mFrameTimeElapsed += deltaTime;

        DerivedSModeT::onUpdate(deltaTime);
        /* ActualType::onUpdate(deltaTime); */
    }

protected:
    LoopModeMixin()
    {}

private:
    static void applyFrame()
    {
        digitalWrite(LED_RED, mFrames[mCurrFrame].red ? HIGH : LOW);
        digitalWrite(LED_YELLOW, mFrames[mCurrFrame].yellow ? HIGH : LOW);
        digitalWrite(LED_GREEN, mFrames[mCurrFrame].green ? HIGH : LOW);
        mFrameTimeElapsed = 0;
    }

protected:
    static Frame mFrames[framesNumberV];
    static uint8_t mCurrFrame;
    static unsigned long mFrameTimeElapsed;
};

template<typename DerivedSModeT, uint8_t framesNumberV>
uint8_t LoopModeMixin<DerivedSModeT, framesNumberV>::mCurrFrame = 0U;

template<typename DerivedSModeT, uint8_t framesNumberV>
unsigned long LoopModeMixin<DerivedSModeT, framesNumberV>::mFrameTimeElapsed = 0UL;
