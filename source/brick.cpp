#include "brick.h"

using namespace std;

Brick::Brick(void)
{
    BX = 721; BY = 772; // 813
    BW = BH = 40;
//    cout << "brick: construct padrao" << endl;
}

Brick::~Brick(void)
{
//    cout << "brick: destr" << endl;
}

float Brick::getBX(void)
{
    return BX;
}

float Brick::getBY(void)
{
    return BY;
}

float Brick::getBW(void)
{
    return BW;
}

float Brick::getBH(void)
{
    return BH;
}
