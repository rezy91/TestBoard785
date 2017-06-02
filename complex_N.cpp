#include "complex_N.h"

complex_N::complex_N(SHAPE_MATH e_s, float f_rOrMag, float f_iOrP)
{
    shape = e_s;

    if(shape == ALGEB)
    {
        real = f_rOrMag;
        imag = f_iOrP;
    }
    else if(shape == GONIO)
    {
        magni = f_rOrMag;
        phase = f_iOrP;
    }
}

void complex_N::ConvertToGonio(complex_N &m_c)
{
    if(m_c.shape == ALGEB)
    {
        m_c.magni = sqrt(m_c.real * m_c.real + m_c.imag * m_c.imag);

        if(m_c.imag < 0.0f)
        {
            m_c.phase = - acos(m_c.real / m_c.magni);
        }
        else
        {
            m_c.phase = acos(m_c.real / m_c.magni);
        }
        m_c.shape = GONIO;
    }
}

void complex_N::ConvertToAlgeb(complex_N &m_c)
{
    if(m_c.shape == GONIO)
    {
        m_c.real = m_c.magni * cos(m_c.phase);
        m_c.imag = m_c.magni * sin(m_c.phase);

        m_c.shape = ALGEB;
    }
}

float complex_N::GetPhase()
{
    ConvertToGonio(*this);

    return this->phase;
}

float complex_N::GetMagni()
{
    ConvertToGonio(*this);

    return this->magni;
}

float complex_N::GetReal()
{
    ConvertToAlgeb(*this);

    return this->real;
}

float complex_N::GetImag()
{
    ConvertToAlgeb(*this);

    return this->imag;
}

complex_N complex_N::operator+(complex_N m_c)
{
    complex_N result;

    if(m_c.shape == GONIO)
    {
        ConvertToAlgeb(m_c);
    }
    if(this->shape == GONIO)
    {
        ConvertToAlgeb(*this);
    }

    result.real = this->real + m_c.real;
    result.imag = this->imag + m_c.imag;

    return result;
}

complex_N complex_N::operator-(complex_N m_c)
{
    complex_N result;

    if(m_c.shape == GONIO)
    {
        ConvertToAlgeb(m_c);
    }
    if(this->shape == GONIO)
    {
        ConvertToAlgeb(*this);
    }

    result.real = this->real - m_c.real;
    result.imag = this->imag - m_c.imag;

    return result;
}

complex_N complex_N::operator*(complex_N m_c)
{
    complex_N result;

    if(m_c.shape == GONIO)
    {
        ConvertToAlgeb(m_c);
    }
    if(this->shape == GONIO)
    {
        ConvertToAlgeb(*this);
    }

    result.real = this->real * m_c.real - this->imag * m_c.imag;
    result.imag = this->real * m_c.imag + this->imag * m_c.real;

    return result;
}

complex_N complex_N::operator/(complex_N m_c)
{
    complex_N result;

    if(m_c.shape == GONIO)
    {
        ConvertToAlgeb(m_c);
    }
    if(this->shape == GONIO)
    {
        ConvertToAlgeb(*this);
    }

    float div = (m_c.real * m_c.real) + (m_c.imag * m_c.imag);
    result.real = (this->real * m_c.real) + (this->imag * m_c.imag);
    result.real /= div;
    result.imag = (this->imag * m_c.real) - (this->real * m_c.imag);
    result.imag /= div;

    return result;
}
