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

 Special tnx to:
 G4KLX for MMDVMHost
 Nonoo for parts of dmr.cpp and helping getting the code to contruct a good Full LC header
 PD0MZ for parts of dmr.cpp
 

 */

#include "main.h"

void setup(void)
{
    //Init display
    nexInit();

    // set all the pin modes and initial states
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);

    pinMode(A9, OUTPUT);
    pinMode(A8, INPUT);
    pinMode(PTT, INPUT);
    pinMode(A11, OUTPUT);
    pinMode(A12, OUTPUT);

    Serial.begin(115200);                  // set serial interface to host
    Serial.flush();
    Serial2.begin(115200);                  // set serial interface to host
    Serial2.flush();
    Serial3.begin(115200);                  // set serial interface to host
    Serial3.flush();

    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);

    digitalWrite(A11, HIGH);
    digitalWrite(A9, LOW);
    delay(200);
    digitalWrite(A9, HIGH);
    delay(200);

    Serial3.write(0x61);
    Serial3.write(0x00);
    Serial3.write(0x01);
    Serial3.write(0x00);
    Serial3.write(0x33);

    delay(50);

    Serial3.write(0x61);
    Serial3.write(0x00);
    Serial3.write(0x0D);
    Serial3.write(0x00);
    Serial3.write(0x0A);

    Serial3.write(0x04);   // DMR
    Serial3.write(0x31);
    Serial3.write(0x07);
    Serial3.write(0x54);
    Serial3.write(0x24);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x00);
    Serial3.write(0x6F);
    Serial3.write(0x48);

    delay(500);

    uint32_t dst_id = 2043044;
    uint32_t src_id = 2043088;

    guiDstID.setValue(dst_id);
    guiSrcID.setValue(src_id);

    full_lc_header.fields.dst_id[0] = (uint8_t)(dst_id >> 16);
    full_lc_header.fields.dst_id[1] = (uint8_t)(dst_id >> 8);
    full_lc_header.fields.dst_id[2] = (uint8_t)(dst_id >> 0);

    full_lc_header.fields.src_id[0] = (uint8_t)(src_id >> 16);
    full_lc_header.fields.src_id[1] = (uint8_t)(src_id >> 8);
    full_lc_header.fields.src_id[2] = (uint8_t)(src_id >> 0);

    //http://www.etsi.org/deliver/etsi_ts/102300_102399/10236102/01.02.06_60/ts_10236102v010206p.pdf 7.2.1
    full_lc_header.fields.service_options = 0x00;

    //http://www.etsi.org/deliver/etsi_ts/102300_102399/10236101/02.02.01_60/ts_10236101v020201p.pdf 9.3.5
    full_lc_header.fields.fid = 0x00;

    //http://www.etsi.org/deliver/etsi_ts/102300_102399/10236102/01.02.06_60/ts_10236102v010206p.pdf B.1
    //0x00 Group call
    //0x01 Private call
    uint32_t flcoState;
    guiFLCO.getValue(&flcoState);
    if (flcoState == 0)
        full_lc_header.fields.flco = 0x00; 
    else
        full_lc_header.fields.flco = 0x03; 

}

void sendFullLC()
{
    uint8_t sessionFullLC[33];
    full_lc_encode(&full_lc_header, sessionFullLC, DATA_TYPE_VOICE_LC);
    slot_type_encode(0x01, DATA_TYPE_VOICE_LC, sessionFullLC);
    dmr_sync_pattern_encode(sessionFullLC,DMR_SYNC_PATTERN_MS_SOURCED_DATA);

    SendDMRToHost(sessionFullLC, 33, 0x41);
}

void sendTerminator()
{
    uint8_t sessionTerminator[33];
    full_lc_encode(&full_lc_header, sessionTerminator, DATA_TYPE_TERMINATOR_WITH_LC);
    slot_type_encode(0x01, DATA_TYPE_TERMINATOR_WITH_LC, sessionTerminator);
    dmr_sync_pattern_encode(sessionTerminator,DMR_SYNC_PATTERN_MS_SOURCED_DATA);

    SendDMRToHost(sessionTerminator, 33, 0x42);
}

void getGUISettings()
{
    uint32_t dst_id=0,src_id=0;

    guiDstID.getValue(&dst_id);
    guiSrcID.getValue(&src_id);
    guiDstID.getValue(&dst_id);
    guiSrcID.getValue(&src_id);

    full_lc_header.fields.dst_id[0] = (uint32_t)(dst_id >> 16);
    full_lc_header.fields.dst_id[1] = (uint32_t)(dst_id >> 8);
    full_lc_header.fields.dst_id[2] = (uint32_t)(dst_id >> 0);

    full_lc_header.fields.src_id[0] = (uint32_t)(src_id >> 16);
    full_lc_header.fields.src_id[1] = (uint32_t)(src_id >> 8);
    full_lc_header.fields.src_id[2] = (uint32_t)(src_id >> 0);

    //Read group/private call
    uint32_t flcoState=0;
    guiFLCO.getValue(&flcoState);
    if (flcoState == 0)
        full_lc_header.fields.flco = 0x00; 
    else
        full_lc_header.fields.flco = 0x03; 
}

void handlePTTStatus()
{
    if(digitalRead(PTT) == LOW)
    {
        if (TRX_STATUS==0)
        {
            getGUISettings();
            sendFullLC();
            slotState = 0;
        }
        TRX_STATUS = 1;
    }
    else
    {
        if (TRX_STATUS==1)
        {
            sendTerminator();
            slotState = 0; 
        }
        TRX_STATUS = 0;
    }
}

//======================================= Main loop =====================================================

void loop()
{

    handlePTTStatus();

    //Host buffer
    while (Serial.available() > 0)
    {

        serial_buffer[serial_buffer_input_pointer] = Serial.read();
        serial_buffer_input_pointer +=1;
        if (serial_buffer_input_pointer > ser_in_buf_len) serial_buffer_input_pointer = ser_in_buf_len;

        if (serial_buffer[0] != 0xE0) 
        {
            serial_buffer_input_pointer = 0;
        }
        if (serial_buffer[0] == 0xE0 && (serial_buffer_input_pointer == serial_buffer[1]))
        {
            Answer_Host();
            serial_buffer_input_pointer = 0;
            break;
        }
    }


    //AMBE Buffer
    while (Serial3.available() > 0)
    {
        //Read AMBE data from AMBE to serial3 buffer
        serial3_buffer[serial3_buffer_input_pointer] = Serial3.read();
        serial3_buffer_input_pointer +=1;
        if (serial3_buffer_input_pointer > ser3_in_buf_len) serial3_buffer_input_pointer = ser3_in_buf_len;

        if (serial3_buffer[0] != 0x61) 
        {
            serial3_buffer_input_pointer = 0;
            break;
        } 

        if (serial3_buffer_input_pointer > 14)
        {
            serial3_buffer_input_pointer = 0;
            serial3_buffer[0] = 0x00;
            if ((serial3_buffer[1] == 0x00) && (serial3_buffer[2] == 0x0B) && (serial3_buffer[3] == 0x01) && (serial3_buffer[4] == 0x01) && (serial3_buffer[5] == 0x48) )
            {
                //Send ambe buffer to ambe
                if(TRX_STATUS == 0)
                {
                    if (toAmbe->numElements(toAmbe) > 0)
                    {
                        uint8_t voiceFrame[9];
                        uint8_t ambeStartVoiceFrame[6] = {0x61,0x00,0x0B,0x01,0x01,0x48};
                        toAmbe->pull(toAmbe,&voiceFrame);
                        SendToAmbe(ambeStartVoiceFrame, 6);
                        SendToAmbe(voiceFrame, 9);
                    }
                }
                else
                {
                    fromAmbe->add(fromAmbe, (const char*)serial3_buffer + 6);
                    if (fromAmbe->isFull(fromAmbe)){
                        Send_Buffer_To_Host();
                    }
                }
                break;

            } // End Valid AMBE packet received
        }
    } // End AMBE packet received

}

/*
 * Create dmr frame and send to MMDVMHost
 */
void Send_Buffer_To_Host()
{
    uint8_t dmrFrame[33];
    uint8_t voiceFrame[9];
    uint8_t i;
    //Read voiceframe 1
    fromAmbe->pull(fromAmbe,&voiceFrame);
    for (i=0;i<9;i++)
    {
        dmrFrame[i] = voiceFrame[i];
    }

    //Read voiceframe 2
    fromAmbe->pull(fromAmbe,&voiceFrame);
    dmrFrame[9] = voiceFrame[0];
    dmrFrame[10] = voiceFrame[1];
    dmrFrame[11] = voiceFrame[2];
    dmrFrame[12] = voiceFrame[3];
    dmrFrame[13] = (voiceFrame[4] & 0xF0) | 0x00;
    dmrFrame[19] = (voiceFrame[4] & 0x0F) | 0x00;
    dmrFrame[20] = voiceFrame[5];
    dmrFrame[21] = voiceFrame[6];
    dmrFrame[22] = voiceFrame[7];
    dmrFrame[23] = voiceFrame[8];

    //Read voiceframe 3
    fromAmbe->pull(fromAmbe,&voiceFrame);
    for (i=0;i<9;i++)
    {
        dmrFrame[i+24] = voiceFrame[i];
    }

    //Send SYNC word
    if(slotState == 0)
    {
        dmr_sync_pattern_encode(dmrFrame,DMR_SYNC_PATTERN_MS_SOURCED_VOICE);
    }
    else
    {
        dmrFrame[14] = 0x00;
        dmrFrame[15] = 0x00;
        dmrFrame[16] = 0x00;
        dmrFrame[17] = 0x00;
        dmrFrame[18] = 0x00;
    }

    SendDMRToHost(dmrFrame, 33, (slotState == 0 ? 0x20 : slotState));
    if (slotState == 6)
    {
        slotState = 0;
    }
    else
        slotState++;
}

/*
 * Reply to MMDVMHost sending bytes
 */
void Answer_Host()
{
    switch(serial_buffer[2])
    {
        case MMDVM_GET_VERSION:
            SendToHost(MMDVM_version, 10, MMDVM_GET_VERSION);
            break;
        case MMDVM_GET_STATUS:
            MMDVM_Status[2] = (uint8_t)TRX_STATUS;
            MMDVM_Status[5] = (5 - (toAmbe->numElements(toAmbe)/3));
            SendToHost(MMDVM_Status, 8, MMDVM_GET_STATUS);
            break;
        case MMDVM_SET_MODE:
        case MMDVM_SET_FREQ:
        case MMDVM_SET_CONFIG:
            MMDVM_ACK_SEND[3] = (uint8_t)serial_buffer[2]; 
            SendToHost(MMDVM_ACK_SEND, 4, MMDVM_ACK);
            break;
    }

    if (serial_buffer[0] == 0xE0 && serial_buffer[1] == 0x25 && serial_buffer[2] == 0x1A)
    {
        voiceFrame_t voiceFrame;

        //Read voiceframe 1
        toAmbe->add(toAmbe,(const char*)serial_buffer + 4);

        //Read voiceframe 2
        voiceFrame.bytes[0] = serial_buffer[13];
        voiceFrame.bytes[1] = serial_buffer[14];
        voiceFrame.bytes[2] = serial_buffer[15];
        voiceFrame.bytes[3] = serial_buffer[16];
        voiceFrame.bytes[4] = (serial_buffer[17] & 0xF0) | (serial_buffer[23] & 0x0F);
        voiceFrame.bytes[5] = serial_buffer[24];
        voiceFrame.bytes[6] = serial_buffer[25];
        voiceFrame.bytes[7] = serial_buffer[26];
        voiceFrame.bytes[8] = serial_buffer[27];
        toAmbe->add(toAmbe,&voiceFrame);

        //Read voiceframe 3 (start on byte 28)
        toAmbe->add(toAmbe,(const char*)serial_buffer+28);
    }
}
