#include "LoopMode0.h"


void LoopMode0::onReset()
{
}

void LoopMode0::onUpdate(unsigned long deltaTime)
{
}

LoopMode0::LoopMode0()
{}

template<>
Frame LoopModeMixin<LoopMode0, 3U>::mFrames[3U] = {
    Frame(true, false, false, 1UL * 1000000UL),
    Frame(false, true, false, 1UL * 1000000UL),
    Frame(false, false, true, 1UL * 1000000UL)
};
