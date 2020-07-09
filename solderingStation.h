/*
 * solderingStation.h
 *
 */

#ifndef SOLDERING_STATION_SOLDERINGSTATION_H_
#define SOLDERING_STATION_SOLDERINGSTATION_H_

#include <stdint.h>

typedef struct
{
    void (*const Init)();
    void (*const Start)();
}SolderingStation_t;

extern const SolderingStation_t SolderingStation;

#endif /* SOLDERING_STATION_SOLDERINGSTATION_H_ */
