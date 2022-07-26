#include "pill.h"

Pill::Pill(void)
{
    PX = 410;
    PY = 208;
    PW = 6;
    PH = 6;
}

Pill::~Pill(void)
{
    //dtor
}

float Pill::getPX(void)
{
    return PX;
}

float Pill::getPY(void)
{
    return PY;
}

float Pill::getPW(void)
{
    return PW;
}

float Pill::getPH(void)
{
    return PH;
}
