
#ifndef SOLDERING_STATION_MEASUREMENT_H_
#define SOLDERING_STATION_MEASUREMENT_H_

#include <stdint.h>

typedef struct
{
    void        (*const Init)();
    int16_t     (*const ReadTemp)();
}Measurement_t;

extern const Measurement_t Measurement;

#endif /* SOLDERING_STATION_MEASUREMENT_H_ */
