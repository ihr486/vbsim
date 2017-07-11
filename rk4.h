#pragma once

typedef struct state_tag
{
    REAL v, i;
} state_t;

typedef struct slope_tag
{
    REAL dv, di;
} slope_t;

static slope_t RK4sub(state_t s0, const REAL m[6])
{
    slope_t d;
    d.dv = m[0] * s0.v + m[1] * s0.i + m[2];
    d.di = m[3] * s0.v + m[4] * s0.i + m[5];
    return d;
}

static state_t RK4predict(state_t s0, slope_t d, REAL h)
{
    state_t s1;
    s1.v = s0.v + h * d.dv;
    s1.i = s0.i + h * d.di;
    return s1;
}

static state_t RK4(state_t s0, const REAL m[6], REAL h)
{
    slope_t k[4];
    k[0] = RK4sub(s0, m);
    k[1] = RK4sub(RK4predict(s0, k[0], 0.5 * h), m);
    k[2] = RK4sub(RK4predict(s0, k[1], 0.5 * h), m);
    k[3] = RK4sub(RK4predict(s0, k[2], h), m);

    slope_t km;
    km.dv = (k[0].dv + 2.0 * k[1].dv + 2.0 * k[2].dv + k[3].dv) / 6.0;
    km.di = (k[0].di + 2.0 * k[1].di + 2.0 * k[2].di + k[3].di) / 6.0;

    return RK4predict(s0, km, h);
}

