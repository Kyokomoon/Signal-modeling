#include "barrier.h"

double calculation_glass(double f){
    return 2.0 + 0.2 * f;
}
double calculation_IRR_glass(double f){
    return 23.0 + 0.3 * f;
}

double calculation_concrete(double f){
    return 5.0 + 4.0 * f;
}

double calculation_drywall(double f){
    return 4.85 + 0.12 * f;
}
