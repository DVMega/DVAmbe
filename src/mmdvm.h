#include "Arduino.h"

uint8_t MMDVM_ACK_SEND[4] = {0xE0,0x04,0x70,0x00};
uint8_t MMDVM_version[10] = {0xe0,0x0a,0x00,0x01,0x44,0x56,0x4D,0x45,0x47,0x41};
//First byte == Mode
//  Mode:
//      01 DSTAR
//      02 DMR
//      04 Fusion
//second byte == Radio Mode
//  Mode:
//      01 DSTAR
//      02 DMR
//      03 Fusion
//                                         DSTA TS1  TS2  C4FM P25
uint8_t MMDVM_Status[8] = {0x03,0x02,0x00,0x00,0x00,0x03,0x00,0x01};

const uint8_t MMDVM_FRAME_START  = 0xE0U;

const uint8_t MMDVM_GET_VERSION  = 0x00U;
const uint8_t MMDVM_GET_STATUS   = 0x01U;
const uint8_t MMDVM_SET_CONFIG   = 0x02U;
const uint8_t MMDVM_SET_MODE     = 0x03U;
const uint8_t MMDVM_SET_FREQ     = 0x04U;

const uint8_t MMDVM_CAL_DATA     = 0x08U;

const uint8_t MMDVM_SEND_CWID    = 0x0AU;

const uint8_t MMDVM_DSTAR_HEADER = 0x10U;
const uint8_t MMDVM_DSTAR_DATA   = 0x11U;
const uint8_t MMDVM_DSTAR_LOST   = 0x12U;
const uint8_t MMDVM_DSTAR_EOT    = 0x13U;

const uint8_t MMDVM_DMR_DATA1    = 0x18U;
const uint8_t MMDVM_DMR_LOST1    = 0x19U;
const uint8_t MMDVM_DMR_DATA2    = 0x1AU;
const uint8_t MMDVM_DMR_LOST2    = 0x1BU;
const uint8_t MMDVM_DMR_SHORTLC  = 0x1CU;
const uint8_t MMDVM_DMR_START    = 0x1DU;
const uint8_t MMDVM_DMR_ABORT    = 0x1EU;

const uint8_t MMDVM_YSF_DATA     = 0x20U;
const uint8_t MMDVM_YSF_LOST     = 0x21U;

const uint8_t MMDVM_P25_HDR      = 0x30U;
const uint8_t MMDVM_P25_LDU      = 0x31U;
const uint8_t MMDVM_P25_LOST     = 0x32U;

const uint8_t MMDVM_ACK          = 0x70U;
const uint8_t MMDVM_NAK          = 0x7FU;

const uint8_t MMDVM_DEBUG1       = 0xF1U;
const uint8_t MMDVM_DEBUG2       = 0xF2U;
const uint8_t MMDVM_DEBUG3       = 0xF3U;
const uint8_t MMDVM_DEBUG4       = 0xF4U;
const uint8_t MMDVM_DEBUG5       = 0xF5U;
