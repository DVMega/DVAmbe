#include "Arduino.h"

void PrintHex8(uint8_t *data, uint8_t length);
void SendDMRToHost(uint8_t *data, uint8_t length, uint8_t frametype);
void SendToHost(uint8_t *data, uint8_t length, uint8_t type);
void SendToAmbe(uint8_t *data, uint8_t length);
