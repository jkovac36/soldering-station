
#include "measurement.h"
#include "spi.h"

#include <stdint.h>

static void Init()
{
    SPI.Init();
}

static int16_t ReadTemp()
{
    uint8_t buffer[2];
    uint16_t receivedData = 0;
    SpiMsg_t msg =
    {
        buffer,
        0,
        buffer,
        2
    };

    // According to MAX6675 specification (https://datasheets.maximintegrated.com/en/ds/MAX6675.pdf)
    // There's two byte output from the device with following format:
    //  _______ ____________________________ _____________ _______ _______
    // | Dummy | Temperature reading 12-bit |  TC input   | DevID | State |
    // |  15   | 14  | ...        ... |  3  |      2      |   1   |   0   |
    // |   0   | MSB | ...        ... | LSB | 1-not conn. |   0   |   -   |

    SPI.ReadBurst(&msg);
    if(buffer[1] & 0x04) return -1; // if bit 2 is high, thermocouple is disconnected

    receivedData = (buffer[0] << 8) + buffer[1];
    receivedData = (receivedData & 0x7FF8) >> 3;

    if((receivedData % 4) > 1) return (int16_t)((receivedData / 4) + 1);
    else return (int16_t)(receivedData / 4);
}

const Measurement_t Measurement =
{
    Init,
    ReadTemp
};
