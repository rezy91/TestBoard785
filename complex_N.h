#ifndef complex_N_H
#define complex_N_H

#include <cmath>

class complex_N
{
public:
    typedef enum
    {
        ALGEB,
        GONIO,
    } SHAPE_MATH;

    complex_N(SHAPE_MATH e_s = ALGEB, float f_rOrMag = 0, float f_iOrP = 0);

    void ConvertToGonio(complex_N &m_c);
    void ConvertToAlgeb(complex_N &m_c);

    float GetPhase(void);
    float GetMagni(void);
    float GetReal(void);
    float GetImag(void);

    complex_N operator+(complex_N m_c);
    complex_N operator-(complex_N m_c);
    complex_N operator*(complex_N m_c);
    complex_N operator/(complex_N m_c);

private:
    float real;
    float imag;

    float magni;
    float phase;

    int shape;

};

#endif // complex_N_H
