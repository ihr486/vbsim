#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define REAL double
#define INT int

typedef struct params_tag
{
    REAL E;
    REAL L;
    REAL Rs;
    REAL C;
    REAL Rp;
    REAL Vf;
    REAL Ic;
    REAL F;
    INT N;
} params_t;

static REAL square(REAL x)
{
    return x * x;
}

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
    printf("Control parameters:\n");
    printf("    Drive frequency (F) = %lf [kHz]\n", params->F * 1E+3);
}

static bool check_oscillation(const params_t *params)
{
    REAL D = square(params->L - params->C * params->Rs * params->Rp) - 4.0 * params->L * params->C * params->Rp;
    
    return D > 0;
}

static bool check_params(const params_t *params)
{
    if (!check_oscillation(params))
    {
        fprintf(stderr, "The circuit oscillates.\n");
        return false;
    }
    return true;
}

static void simulate(const params_t *params, FILE *of)
{

}

int main(int argc, const char *argv[])
{
    params_t params = {
        5.0, 33E-6, 0.18, 47E-6, 5E+3, 0.4, 0.1E-3, 20E+6, 10000
    };

    print_params(&params);
    if (!check_params(&params)) return -1;

    simulate(&params, stdout);

    return 0;
}
