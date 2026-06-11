#ifndef TSETLIN_H
#define TSETLIN_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #ifdef _WIN32
// #include <windows.h>
// #elif !defined(__AVR__)
// #include <sys/unistd.h>
// #endif

#include "clause.h"

#ifdef __cplusplus
extern "C" {
#endif

void tsetlin_step(Tsetlin* model, char* X_img, int y_target, int T, float s);

int tsetlin_evaluate(Tsetlin* model, char* input, int *out_votes);

#ifdef __cplusplus
}
#endif

#endif // TSETLIN_H
