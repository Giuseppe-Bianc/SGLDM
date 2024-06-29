//
// Created by gbian on 28/06/2024.
//

#pragma once

#include "../cast/casts.hpp"
#include "timeFactors.hpp"

class Timestep {
public:
    explicit Timestep(float time = 0.0F) : m_Time(C_LD(time)) {}
    explicit Timestep(double time = 0.0) : m_Time(C_LD(time)) {}
    explicit Timestep(long double time = 0.0L) : m_Time(time) {}

    explicit operator float() const { return C_F(m_Time); }
    explicit operator double() const { return C_D(m_Time); }
    explicit operator long double() const { return m_Time; }

    long double GetSeconds() const noexcept { return m_Time; }
    long double GetMilliseconds() const noexcept { return m_Time * 1000.0L; }

private:
    long double m_Time;
};