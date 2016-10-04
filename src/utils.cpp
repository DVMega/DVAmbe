/*
   DV-AMBE version 00.01A

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA  02110-1301, USA.

   Authors: 
    Guus van Dooren, PE1PLM via E-mail : info@auria.nl
    Rudy Hardeman, PD0ZRY pd0zry at pd0zry.nl

*/

#include "utils.h"

void PrintHex8(uint8_t *data, uint8_t length)
{
    for (int i=0; i<length; i++) {
        if (data[i]<17)
        {
            Serial2.print("0");
        }
        Serial2.print(data[i],HEX);
        Serial2.print(" ");
    }
    Serial2.println();
}

void SendDMRToHost(uint8_t *data, uint8_t length, uint8_t frametype)
{
    Serial.write(0xE0);
    Serial.write(0x25);
    Serial.write(0x1A);
    Serial.write(frametype);
    for (int i=0; i<length; i++) {
        Serial.write(data[i]);
    }
}

void SendToHost(uint8_t *data, uint8_t length, uint8_t type)
{
    Serial.write(0xE0);
    Serial.write(length+3);
    Serial.write(type);
    for (int i=0; i<length; i++) {
        Serial.write(data[i]);
    }
}

void SendToAmbe(uint8_t *data, uint8_t length)
{
    for (int i=0; i<length; i++) {
        Serial3.write(data[i]);
    }
}
