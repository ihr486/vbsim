#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define REAL double
#define INT int

#include "rk4.h"

typedef struct params_tag
{
    REAL E;
    REAL L;
    REAL Rs;
    REAL C;
    REAL Rp;
    REAL Vf;
    REAL Ic;
    REAL Rc;
    REAL F;
    INT N;
    INT Ns;
} params_t;

static void print_params(const params_t *params)
{
    printf("Circuit parameters:\n");
    printf("    Supply voltage (E) = %lf [V]\n", params->E);
    printf("    Booster inductance (L) = %lf [uH]\n", params->L * 1E+6);
    printf("    Buffer capacitance (C) = %lf [uF]\n", params->C * 1E+6);
    printf("    Booster ESR (Rs) = %lf [ohm]\n", params->Rs);
    printf("    Resistive load (Rp) = %lf [ohm]\n", params->Rp);
    printf("    Diode voltage drop (Vf) = %lf [V]\n", params->Vf);
    printf("    Constant load (Ic) = %lf [mA]\n", params->Ic * 1E+3);
    printf("    FET channel resistance (Rc) = %lf [ohm]\n", params->Rc);
    printf("Control parameters:\n");
    printf("    Drive frequency (F) = %lf [kHz]\n", params->F * 1E-3);
    printf("Simulation parameters:\n");
    printf("    PWM steps (N) = %d\n", params->N);
    printf("    RK4 steps per PWM step (Ns) = %d\n", params->Ns);
}

static INT pid_control(const params_t *params, state_t state)
{
    INT ad = state.v * 
}

#define DIODE_EPS (1E-6)

static void simulate(const params_t *params, FILE *of)
{
    REAL Mon[6], Moff[6], Moff1[6];

    Mon[0] = -1.0 / (params->C * params->Rp);
    Mon[1] = 1.0 / params->C;
    Mon[2] = -params->Ic / params->C;

    Mon[3] = -1.0 / params->L;
    Mon[4] = -params->Rs / params->L;
    Mon[5] = (params->E - params->Vf) / params->L;

    Moff[0] = Mon[0];
    Moff[1] = 0;
    Moff[2] = Mon[2];

    Moff[3] = 0;
    Moff[4] = -(params->Rs + params->Rc) / params->L;
    Moff[5] = params->E / params->L;

    memcpy(Moff1, Moff, sizeof(Moff));

    Moff1[4] = 0;
    Moff1[5] = 0;

    state_t s;

    s.v = params->E - params->Vf - params->Ic * params->Rs;
    s.i = s.v / params->Rp + params->Ic;

    REAL T = 1.0 / (params->F * params->Ns);
    INT D = 50;

    for (INT i0 = 0; i0 < params->N; i0++)
    {
        INT i1;
        for (i1 = 0; i1 < D; i1++)
        {
            s = RK4(s, Mon, T);
        }
        for (; i1 < params->Ns; i1++)
        {
            s = RK4(s, fabs(s.i) < DIODE_EPS ? Moff1 : Moff, T);
        }
        fprintf(of, "%lf %lf\n", i0 / params->F, s.v);
    }
}

int main(int argc, const char *argv[])
{
    FILE *gp = popen("gnuplot -persist", "w");

    params_t params = {
        5.0, 33E-6, 0.18, 47E-6, 10E+3, 0.4, 0.1E-3, 10E-3, 20E+3, 200, 1024
    };

    print_params(&params);

    fprintf(gp, "plot '-' with lines\n");

    simulate(&params, gp);

    fprintf(gp, "e\n");

    return 0;
}
