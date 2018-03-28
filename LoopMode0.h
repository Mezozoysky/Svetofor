#pragma once

#include "LoopModeMixin.h"

struct LoopMode0
: public LoopModeMixin<LoopMode0>
{
    static void onReset();
    static void onUpdate(unsigned long deltaTime);

private:
    LoopMode0();
    static void applyFrame();

private:
    static Frame mFrames[3];
    static uint8_t mCurrFrame;
    static unsigned long mFrameTimeElapsed;
};
