#ifndef BARRIER_H
#define BARRIER_H
#include <QGraphicsScene>

const int Glass_P = 1;
const int Glass_IRR = 2;
const int Beton = 3;
const int Drywall = 4;
double calculation_glass(double f);
double calculation_IRR_glass(double f);
double calculation_concrete(double f);
double calculation_drywall(double f);
#endif // BARRIER_H
