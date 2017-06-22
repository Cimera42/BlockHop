#ifndef ANGLE_H_INCLUDED
#define ANGLE_H_INCLUDED

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include "logger.h"

#define M_2PI 6.28318530

class Angle
{
    bool bound;
    bool clamp;
    double lower;
    double upper;

    double radianAngle;

    double correctAngle(double angle)
    {
        if(clamp)
        {
            angle = std::max(angle, lower);
            angle = std::min(angle, upper);
        }

        if(bound)
        {
            while(angle > upper)
                angle -= std::abs(upper - lower);
            while(angle < lower)
                angle += std::abs(upper - lower);
        }

        return angle;
    }

    public:
        Angle()
        {
            radianAngle = 0;

            clamp = false;
            bound = true;
            lower = 0;
            upper = M_2PI;
        }

        Angle(bool doClamp, double low, double up)
        {
            radianAngle = 0;
            lower = low;
            upper = up;

            bound = !doClamp;
            clamp = doClamp;
        }

        double deg()
        {
            return radianAngle * (M_PI/180);
        }

        template <typename T>
        void operator=(T other)
        {
            radianAngle = correctAngle(other);
        }

        template <typename T>
        Angle operator+(T other)
        {
            Angle angle;
            angle.radianAngle = correctAngle(radianAngle + other);
            return angle;
        }
        template <typename T>
        Angle operator+=(T other)
        {
            radianAngle = correctAngle(radianAngle + other);
            return *this;
        }

        template <typename T>
        Angle operator-(T other)
        {
            Angle angle;
            angle.radianAngle = correctAngle(radianAngle - other);
            return angle;
        }
        template <typename T>
        Angle operator-=(T other)
        {
            radianAngle = correctAngle(radianAngle - other);
            return *this;
        }

        template <typename T>
        Angle operator*(T other)
        {
            Angle angle;
            angle.radianAngle = correctAngle(radianAngle * other);
            return angle;
        }
        template <typename T>
        Angle operator*=(T other)
        {
            radianAngle = correctAngle(radianAngle * other);
            return *this;
        }

        template <typename T>
        Angle operator/(T other)
        {
            Angle angle;
            angle.radianAngle = radianAngle / other;
            return angle;
        }
        template <typename T>
        Angle operator/=(T other)
        {
            radianAngle = radianAngle / other;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &output, const Angle &angle)
        {
            output << angle.radianAngle;
            return output;
        }

        template <typename T>
        operator T()
        {
            return radianAngle;
        }
};

#endif // ANGLE_H_INCLUDED
