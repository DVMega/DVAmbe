#include "Arduino.h"

typedef bool flag_t;

typedef enum {
  DATA_TYPE_VOICE_PI = 0x00,
  DATA_TYPE_VOICE_LC,
  DATA_TYPE_TERMINATOR_WITH_LC,
  DATA_TYPE_CSBK,
  DATA_TYPE_MBC,
  DATA_TYPE_MBCC,
  DATA_TYPE_DATA,
  DATA_TYPE_RATE12_DATA,
  DATA_TYPE_RATE34_DATA,
  DATA_TYPE_IDLE
} data_type_t;

typedef union __attribute__((packed)) {
  struct {
        uint8_t flco            : 6;
        uint8_t reserved        : 1;
        uint8_t protect         : 1;
        uint8_t fid;
        uint8_t service_options;
        uint8_t dst_id[3];
        uint8_t src_id[3];
        uint8_t crc[3];
      } fields;
  uint8_t bytes[12];
} full_lc_t;

typedef uint8_t color_code_t;
typedef uint8_t emb_lcss_t;

typedef union {
    struct {
                color_code_t     color_code : 4;
                uint8_t          pi         : 1;
                emb_lcss_t       lcss       : 2;
                uint8_t          qr0        : 1;
                uint8_t          qr1        : 8;
            } flags;
    uint8_t bytes[2];
} emb_t;

struct voiceFrame_t {
    uint8_t bytes[9];
};

static const uint8_t CRC_MASK_VOICE_LC[]           = {0x96, 0x96, 0x96};
static const uint8_t CRC_MASK_TERMINATOR_WITH_LC[] = {0x99, 0x99, 0x99};

static const uint8_t dmr_sync_pattern_bs_sourced_voice[] = { 0x07, 0x55, 0xFD, 0x7D, 0xF7, 0x5F, 0x70 };
static const uint8_t dmr_sync_pattern_bs_sourced_data[]  = { 0x0D, 0xFF, 0x57, 0xD7, 0x5D, 0xF5, 0xD0 };
static const uint8_t dmr_sync_pattern_ms_sourced_voice[] = { 0x07, 0xF7, 0xD5, 0xDD, 0x57, 0xDF, 0xD0 };
static const uint8_t dmr_sync_pattern_ms_sourced_data[]  = { 0x0D, 0x5D, 0x7F, 0x77, 0xFD, 0x75, 0x70 };
static const uint8_t dmr_sync_pattern_ms_sourced_rc[]    = { 0x07, 0x7D, 0x55, 0xF7, 0xDF, 0xD7, 0x70 };
static const uint8_t dmr_sync_pattern_direct_voice_ts1[] = { 0x05, 0xD5, 0x77, 0xF7, 0x75, 0x7F, 0xF0 };
static const uint8_t dmr_sync_pattern_direct_data_ts1[]  = { 0x0F, 0x7F, 0xDD, 0x5D, 0xDF, 0xD5, 0x50 };
static const uint8_t dmr_sync_pattern_direct_voice_ts2[] = { 0x07, 0xDF, 0xFD, 0x5F, 0x55, 0xD5, 0xF0 };
static const uint8_t dmr_sync_pattern_direct_data_ts2[]  = { 0x0D, 0x75, 0x57, 0xF5, 0xFF, 0x7F, 0x50 };
static const uint8_t dmr_sync_pattern_mask[]             = { 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0 };
static const uint8_t dmr_sync_pattern_unknown[]          = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define RS_12_9_DATASIZE      9
#define RS_12_9_CHECKSUMSIZE  3

typedef struct {
  uint8_t data[RS_12_9_DATASIZE+RS_12_9_CHECKSUMSIZE];
} rs_12_9_codeword_t;

typedef struct {
  uint8_t bytes[3];
} rs_12_9_checksum_t;

typedef struct {
  bool raw[196];
  bool dec[196];
} bptc_196_96_t;

typedef struct {
  bool bits[4];
} bptc_196_96_error_vector_t;

typedef enum {
    DMR_SYNC_PATTERN_BS_SOURCED_VOICE = 0x00,
    DMR_SYNC_PATTERN_BS_SOURCED_DATA,
    DMR_SYNC_PATTERN_MS_SOURCED_VOICE,
    DMR_SYNC_PATTERN_MS_SOURCED_DATA,
    DMR_SYNC_PATTERN_MS_SOURCED_RC,
    DMR_SYNC_PATTERN_DIRECT_VOICE_TS1,
    DMR_SYNC_PATTERN_DIRECT_DATA_TS1,
    DMR_SYNC_PATTERN_DIRECT_VOICE_TS2,
    DMR_SYNC_PATTERN_DIRECT_DATA_TS2,
    DMR_SYNC_PATTERN_UNKNOWN
} dmr_sync_pattern;

int dmr_sync_pattern_encode(uint8_t packet[33], dmr_sync_pattern pattern);
void slot_type_encode(uint8_t color_code, uint8_t data_type, uint8_t data[33]);
void golay_20_8_encode(uint8_t data[3]);
int full_lc_encode(full_lc_t *lc, uint8_t packet[33], data_type_t data_type);
int generate_headers(full_lc_t *lc, emb_t *emb, uint8_t header[33], uint8_t terminator[33], uint8_t embdata[33]);
