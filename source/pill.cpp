#include "pill.h"

using namespace std;

Pill::Pill(void)
{
    PX = 410; PY = 208;
    PW = 6; PH = 6;
//    cout << "pill: construct padrao" << endl;
}

Pill::~Pill(void)
{
    //dtor
//    cout << "pill: destr" << endl;
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
