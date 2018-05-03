#define SEED  5743

#include "mplib.h"

int initroad(int *road, int n, float density, int seed);
void updatebcs(int *road, int n);
void mpupdatebcs(int *road, int n, int procid, int nproc);
int updateroad(int *newroad, int *oldroad, int n);
