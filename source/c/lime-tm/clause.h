
#ifndef CLAUSE_H
#define CLAUSE_H

// #ifdef _WIN32
// #include <windows.h>
// #elif !defined(__AVR__)
// #include <sys/unistd.h>
// #endif

#define TSETLIN_USING_STATIC_MODEL

#if defined(ARDUINO)
  /* Arduino */
  #include <Arduino.h> 
  #define TSETLIN_USING_STATIC_MODEL
#endif

#if defined(TSETLIN_USING_PROTOBUF)
  #include <tsetlin.pb-c.h>
  #define TSETLIN_MODEL_TRAINABLE
#elif defined(TSETLIN_USING_STATIC_MODEL)
  #include "tsetlin_model.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

uint8_t clause_evaluate(const ClauseCompressed* clause, uint8_t* input, uint32_t n_state, uint32_t n_feature, ModelType type);

#ifdef __cplusplus
}
#endif

#endif // CLAUSE_H
