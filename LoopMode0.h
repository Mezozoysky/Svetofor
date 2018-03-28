#pragma once

#include "LoopModeMixin.h"

struct LoopMode0
: public LoopModeMixin<LoopMode0, 3U>
{
    static void onReset();
    static void onUpdate(unsigned long deltaTime);

private:
    LoopMode0();
};
