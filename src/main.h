#include "Arduino.h"
#include <RingBuf.h>
#include <Nextion.h>
#include "utils.h"
#include "dmr.h"
#include "mmdvm.h"
#define PTT A10

#define to_ambe_buf_len 850     // Size of radio tx ring buffer
//#define FSK_ring_buffer_lengte 300     // Size of radio rx ring buffer
#define ser_in_buf_len 55        // Size of serial ring buffer
#define ser_out_buf_len 55       // Size of serial out buffer
#define ser3_in_buf_len 55
#define to_host_buf_len 850

//Init display stuff
NexDSButton guiFLCO  = NexDSButton(0, 1, "flco");
NexNumber   guiSrcID = NexNumber(0, 2, "srcid");
NexNumber   guiDstID = NexNumber(0, 3, "dstid");

//Init ambe ring buffer
RingBuf *fromAmbe = RingBuf_new(9, 3);
RingBuf *toAmbe = RingBuf_new(9, 15);

uint8_t sessionEMB[33];

uint8_t slotState; //TX slot state
volatile byte  to_host_buffer[to_host_buf_len + 1];
volatile int  to_host_buffer_in_counter = 1;

volatile byte to_ambe_buffer[to_ambe_buf_len + 1];
volatile int  to_ambe_buffer_in_counter = 1;
volatile int  to_ambe_buffer_out_counter = 1;
volatile int  to_ambe_pack_avail_counter = 0;

volatile byte serial_buffer[ser_in_buf_len + 1];                 // Input buffer for serial communication
volatile byte serial3_buffer[ser3_in_buf_len + 1];

volatile int serial_buffer_input_pointer = 0;
volatile int serial_buffer_output_pointer = 0;
volatile int serial3_buffer_input_pointer = 0;
volatile int serial3_buffer_output_pointer = 0;

volatile byte serial_out[ser_out_buf_len + 1];                   // Output buffer for serial coomunication

volatile int serial_length;

unsigned int crc;

volatile int AMBE_To_TS_Counter = 0;
volatile int Loop_Counter = 0;
volatile int TRX_STATUS = 0;
volatile byte SYNC_SLOT = 1;

volatile byte Temp_Byte;
volatile byte Fill_Buffer = 0;

full_lc_t full_lc_header;

void LOAD_SYNC();
void Send_Buffer_To_Host();
void Send_AMBE_Data_To_Buffer();
void Send_Buffer_Data_To_AMBE();
void Answer_Host();
