#pragma once

#include "Arduino.h"

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
