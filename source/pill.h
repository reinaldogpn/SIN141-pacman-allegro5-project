#ifndef PILL_H
#define PILL_H

class Pill
{
public:
    Pill(void);
    ~Pill(void);
    float getPX(void);
    float getPY(void);
    float getPW(void);
    float getPH(void);
private:
    float PX;
    float PY;
    float PW;
    float PH;
};

#endif // PILL_H
