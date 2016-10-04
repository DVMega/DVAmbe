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


#include "dmr.h"

/*
 * Golay (20, 8)
 */

static const uint16_t golay_20_8_encoder[] = {
    0x0000U, 0xB08EU, 0xE093U, 0x501DU, 0x70A9U, 0xC027U,
    0x903AU, 0x20B4U, 0x60DCU, 0xD052U, 0x804FU, 0x30C1U,
    0x1075U, 0xA0FBU, 0xF0E6U, 0x4068U, 0x7036U, 0xC0B8U,
    0x90A5U, 0x202BU, 0x009FU, 0xB011U, 0xE00CU, 0x5082U,
    0x10EAU, 0xA064U, 0xF079U, 0x40F7U, 0x6043U, 0xD0CDU,
    0x80D0U, 0x305EU, 0xD06CU, 0x60E2U, 0x30FFU, 0x8071U,
    0xA0C5U, 0x104BU, 0x4056U, 0xF0D8U, 0xB0B0U, 0x003EU,
    0x5023U, 0xE0ADU, 0xC019U, 0x7097U, 0x208AU, 0x9004U,
    0xA05AU, 0x10D4U, 0x40C9U, 0xF047U, 0xD0F3U, 0x607DU,
    0x3060U, 0x80EEU, 0xC086U, 0x7008U, 0x2015U, 0x909BU,
    0xB02FU, 0x00A1U, 0x50BCU, 0xE032U, 0x90D9U, 0x2057U,
    0x704AU, 0xC0C4U, 0xE070U, 0x50FEU, 0x00E3U, 0xB06DU,
    0xF005U, 0x408BU, 0x1096U, 0xA018U, 0x80ACU, 0x3022U,
    0x603FU, 0xD0B1U, 0xE0EFU, 0x5061U, 0x007CU, 0xB0F2U,
    0x9046U, 0x20C8U, 0x70D5U, 0xC05BU, 0x8033U, 0x30BDU,
    0x60A0U, 0xD02EU, 0xF09AU, 0x4014U, 0x1009U, 0xA087U,
    0x40B5U, 0xF03BU, 0xA026U, 0x10A8U, 0x301CU, 0x8092U,
    0xD08FU, 0x6001U, 0x2069U, 0x90E7U, 0xC0FAU, 0x7074U,
    0x50C0U, 0xE04EU, 0xB053U, 0x00DDU, 0x3083U, 0x800DU,
    0xD010U, 0x609EU, 0x402AU, 0xF0A4U, 0xA0B9U, 0x1037U,
    0x505FU, 0xE0D1U, 0xB0CCU, 0x0042U, 0x20F6U, 0x9078U,
    0xC065U, 0x70EBU, 0xA03DU, 0x10B3U, 0x40AEU, 0xF020U,
    0xD094U, 0x601AU, 0x3007U, 0x8089U, 0xC0E1U, 0x706FU,
    0x2072U, 0x90FCU, 0xB048U, 0x00C6U, 0x50DBU, 0xE055U,
    0xD00BU, 0x6085U, 0x3098U, 0x8016U, 0xA0A2U, 0x102CU,
    0x4031U, 0xF0BFU, 0xB0D7U, 0x0059U, 0x5044U, 0xE0CAU,
    0xC07EU, 0x70F0U, 0x20EDU, 0x9063U, 0x7051U, 0xC0DFU,
    0x90C2U, 0x204CU, 0x00F8U, 0xB076U, 0xE06BU, 0x50E5U,
    0x108DU, 0xA003U, 0xF01EU, 0x4090U, 0x6024U, 0xD0AAU,
    0x80B7U, 0x3039U, 0x0067U, 0xB0E9U, 0xE0F4U, 0x507AU,
    0x70CEU, 0xC040U, 0x905DU, 0x20D3U, 0x60BBU, 0xD035U,
    0x8028U, 0x30A6U, 0x1012U, 0xA09CU, 0xF081U, 0x400FU,
    0x30E4U, 0x806AU, 0xD077U, 0x60F9U, 0x404DU, 0xF0C3U,
    0xA0DEU, 0x1050U, 0x5038U, 0xE0B6U, 0xB0ABU, 0x0025U,
    0x2091U, 0x901FU, 0xC002U, 0x708CU, 0x40D2U, 0xF05CU,
    0xA041U, 0x10CFU, 0x307BU, 0x80F5U, 0xD0E8U, 0x6066U,
    0x200EU, 0x9080U, 0xC09DU, 0x7013U, 0x50A7U, 0xE029U,
    0xB034U, 0x00BAU, 0xE088U, 0x5006U, 0x001BU, 0xB095U,
    0x9021U, 0x20AFU, 0x70B2U, 0xC03CU, 0x8054U, 0x30DAU,
    0x60C7U, 0xD049U, 0xF0FDU, 0x4073U, 0x106EU, 0xA0E0U,
    0x90BEU, 0x2030U, 0x702DU, 0xC0A3U, 0xE017U, 0x5099U,
    0x0084U, 0xB00AU, 0xF062U, 0x40ECU, 0x10F1U, 0xA07FU,
    0x80CBU, 0x3045U, 0x6058U, 0xD0D6U
};

void golay_20_8_encode(uint8_t data[3])
{
    uint16_t checksum = golay_20_8_encoder[data[0]];
    data[1] = checksum;
    data[2] = checksum >> 8;
}

/*
 * Reed-Solomon (12, 9)
 */

// See DMR AI. spec. page 138.
static uint8_t rs_12_9_galois_exp_table[256] = {
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1D, 0x3A, 0x74, 0xE8, 0xCD, 0x87, 0x13, 0x26,
  0x4C, 0x98, 0x2D, 0x5A, 0xB4, 0x75, 0xEA, 0xC9, 0x8F, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0,
  0x9D, 0x27, 0x4E, 0x9C, 0x25, 0x4A, 0x94, 0x35, 0x6A, 0xD4, 0xB5, 0x77, 0xEE, 0xC1, 0x9F, 0x23,
  0x46, 0x8C, 0x05, 0x0A, 0x14, 0x28, 0x50, 0xA0, 0x5D, 0xBA, 0x69, 0xD2, 0xB9, 0x6F, 0xDE, 0xA1,
  0x5F, 0xBE, 0x61, 0xC2, 0x99, 0x2F, 0x5E, 0xBC, 0x65, 0xCA, 0x89, 0x0F, 0x1E, 0x3C, 0x78, 0xF0,
  0xFD, 0xE7, 0xD3, 0xBB, 0x6B, 0xD6, 0xB1, 0x7F, 0xFE, 0xE1, 0xDF, 0xA3, 0x5B, 0xB6, 0x71, 0xE2,
  0xD9, 0xAF, 0x43, 0x86, 0x11, 0x22, 0x44, 0x88, 0x0D, 0x1A, 0x34, 0x68, 0xD0, 0xBD, 0x67, 0xCE,
  0x81, 0x1F, 0x3E, 0x7C, 0xF8, 0xED, 0xC7, 0x93, 0x3B, 0x76, 0xEC, 0xC5, 0x97, 0x33, 0x66, 0xCC,
  0x85, 0x17, 0x2E, 0x5C, 0xB8, 0x6D, 0xDA, 0xA9, 0x4F, 0x9E, 0x21, 0x42, 0x84, 0x15, 0x2A, 0x54,
  0xA8, 0x4D, 0x9A, 0x29, 0x52, 0xA4, 0x55, 0xAA, 0x49, 0x92, 0x39, 0x72, 0xE4, 0xD5, 0xB7, 0x73,
  0xE6, 0xD1, 0xBF, 0x63, 0xC6, 0x91, 0x3F, 0x7E, 0xFC, 0xE5, 0xD7, 0xB3, 0x7B, 0xF6, 0xF1, 0xFF,
  0xE3, 0xDB, 0xAB, 0x4B, 0x96, 0x31, 0x62, 0xC4, 0x95, 0x37, 0x6E, 0xDC, 0xA5, 0x57, 0xAE, 0x41,
  0x82, 0x19, 0x32, 0x64, 0xC8, 0x8D, 0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0, 0xDD, 0xA7, 0x53, 0xA6,
  0x51, 0xA2, 0x59, 0xB2, 0x79, 0xF2, 0xF9, 0xEF, 0xC3, 0x9B, 0x2B, 0x56, 0xAC, 0x45, 0x8A, 0x09,
  0x12, 0x24, 0x48, 0x90, 0x3D, 0x7A, 0xF4, 0xF5, 0xF7, 0xF3, 0xFB, 0xEB, 0xCB, 0x8B, 0x0B, 0x16,
  0x2C, 0x58, 0xB0, 0x7D, 0xFA, 0xE9, 0xCF, 0x83, 0x1B, 0x36, 0x6C, 0xD8, 0xAD, 0x47, 0x8E, 0x01,
};

// See DMR AI. spec. page 138.
static uint8_t rs_12_9_galois_log_table[256] = {
  0,    0,    1,    25,   2,    50,   26,   198,  3,    223,  51,   238,  27,   104,  199,  75,
  4,    100,  224,  14,   52,   141,  239,  129,  28,   193,  105,  248,  200,  8,    76,   113,
  5,    138,  101,  47,   225,  36,   15,   33,   53,   147,  142,  218,  240,  18,   130,  69,
  29,   181,  194,  125,  106,  39,   249,  185,  201,  154,  9,    120,  77,   228,  114,  166,
  6,    191,  139,  98,   102,  221,  48,   253,  226,  152,  37,   179,  16,   145,  34,   136,
  54,   208,  148,  206,  143,  150,  219,  189,  241,  210,  19,   92,   131,  56,   70,   64,
  30,   66,   182,  163,  195,  72,   126,  110,  107,  58,   40,   84,   250,  133,  186,  61,
  202,  94,   155,  159,  10,   21,   121,  43,   78,   212,  229,  172,  115,  243,  167,  87,
  7,    112,  192,  247,  140,  128,  99,   13,   103,  74,   222,  237,  49,   197,  254,  24,
  227,  165,  153,  119,  38,   184,  180,  124,  17,   68,   146,  217,  35,   32,   137,  46,
  55,   63,   209,  91,   149,  188,  207,  205,  144,  135,  151,  178,  220,  252,  190,  97,
  242,  86,   211,  171,  20,   42,   93,   158,  132,  60,   57,   83,   71,   109,  65,   162,
  31,   45,   67,   216,  183,  123,  164,  118,  196,  23,   73,   236,  127,  12,   111,  246,
  108,  161,  59,   82,   41,   157,  85,   170,  251,  96,   134,  177,  187,  204,  62,   90,
  203,  89,   95,   176,  156,  169,  160,  81,   11,   245,  22,   235,  122,  117,  44,   215,
  79,   174,  213,  233,  230,  231,  173,  232,  116,  214,  244,  234,  168,  80,   88,   175
};

static uint8_t rs_12_9_galois_multiplication(uint8_t a, uint8_t b) {
  if (a == 0 || b == 0)
    return 0;
  return rs_12_9_galois_exp_table[(rs_12_9_galois_log_table[a] + rs_12_9_galois_log_table[b]) % 255];
}

rs_12_9_checksum_t *rs_12_9_calc_checksum(rs_12_9_codeword_t *codeword) {
  // See DMR AI. spec. page 136 for these coefficients.
  static uint8_t genpoly[] = { 0x40, 0x38, 0x0e, 0x01 };
  static rs_12_9_checksum_t rs_12_9_checksum;
  uint8_t i;
  uint8_t feedback;
  rs_12_9_checksum.bytes[0] = rs_12_9_checksum.bytes[1] = rs_12_9_checksum.bytes[2] = 0;
  for (i = 0; i < 9; i++) {
    feedback = codeword->data[i] ^ rs_12_9_checksum.bytes[0];
    rs_12_9_checksum.bytes[0] = rs_12_9_checksum.bytes[1] ^ rs_12_9_galois_multiplication(genpoly[2], feedback);
    rs_12_9_checksum.bytes[1] = rs_12_9_checksum.bytes[2] ^ rs_12_9_galois_multiplication(genpoly[1], feedback);
    rs_12_9_checksum.bytes[2] = rs_12_9_galois_multiplication(genpoly[0], feedback);
  }
  return &rs_12_9_checksum;
}

/*
 * Bytes to bits and vice versa
 */

uint8_t bits_to_byte(bool bits[8])
{
  uint8_t val = 0, i = 0;
  for (i=0;i < 8; i++) {
    if (bits[i]) val |= (1 << (7 - i));
  }
  return val;
}

void bits_to_bytes(bool *bits, size_t bits_length, uint8_t *bytes, size_t bytes_length)
{
  uint16_t i = 0;
  for (i=0 ; i < min(bits_length/8, bytes_length); i++)
    bytes[i] = bits_to_byte(&bits[i * 8]);
} 

void byte_to_bits(uint8_t byte, bool bits[8]) {
  bits[0] = (byte & 128) > 0;
  bits[1] = (byte & 64) > 0;
  bits[2] = (byte & 32) > 0;
  bits[3] = (byte & 16) > 0;
  bits[4] = (byte & 8) > 0;
  bits[5] = (byte & 4) > 0;
  bits[6] = (byte & 2) > 0;
  bits[7] = (byte & 1) > 0;
}

void bytes_to_bits(uint8_t *bytes, size_t bytes_length, bool *bits, size_t bits_length)
{
  uint16_t i = 0;
  for (i=0; i < min(bits_length/8, bytes_length); i++)
    byte_to_bits(bytes[i], &bits[i * 8]);
}

static void bptc_196_96_hamming_15_11_3_get_parity_bits(bool *data_bits, bptc_196_96_error_vector_t *error_vector) {
  if (data_bits == NULL || error_vector == NULL)
    return;
    
  error_vector->bits[0] = (data_bits[0] ^ data_bits[1] ^ data_bits[2] ^ data_bits[3] ^ data_bits[5] ^ data_bits[7] ^ data_bits[8]);
  error_vector->bits[1] = (data_bits[1] ^ data_bits[2] ^ data_bits[3] ^ data_bits[4] ^ data_bits[6] ^ data_bits[8] ^ data_bits[9]);
  error_vector->bits[2] = (data_bits[2] ^ data_bits[3] ^ data_bits[4] ^ data_bits[5] ^ data_bits[7] ^ data_bits[9] ^ data_bits[10]);
  error_vector->bits[3] = (data_bits[0] ^ data_bits[1] ^ data_bits[2] ^ data_bits[4] ^ data_bits[6] ^ data_bits[7] ^ data_bits[10]);
}

static void bptc_196_96_hamming_13_9_3_get_parity_bits(bool *data_bits, bptc_196_96_error_vector_t *error_vector) {
  if (data_bits == NULL || error_vector == NULL)
    return;
    
  error_vector->bits[0] = (data_bits[0] ^ data_bits[1] ^ data_bits[3] ^ data_bits[5] ^ data_bits[6]);
  error_vector->bits[1] = (data_bits[0] ^ data_bits[1] ^ data_bits[2] ^ data_bits[4] ^ data_bits[6] ^ data_bits[7]);
  error_vector->bits[2] = (data_bits[0] ^ data_bits[1] ^ data_bits[2] ^ data_bits[3] ^ data_bits[5] ^ data_bits[7] ^ data_bits[8]);
  error_vector->bits[3] = (data_bits[0] ^ data_bits[2] ^ data_bits[4] ^ data_bits[5] ^ data_bits[8]);
}

// Generates 196 BPTC payload info bits from 96 data bits.
int bptc_196_96_generate(bptc_196_96_t *bptc) {
    bptc_196_96_error_vector_t error_vector;
    uint8_t col, row;
    uint8_t dbp;
    flag_t column_bits[9] = {0,};

    memset(bptc->raw, 0, sizeof(bptc->raw));

    dbp = 0;
    for (row = 0; row < 9; row++) {
        if (row == 0) {
            for (col = 3; col < 11; col++) {
                // +1 because the first bit is R(3) and it's not used so we can ignore that.
                bptc->raw[col+1] = bptc->dec[dbp++];
            }
        } else {
            for (col = 0; col < 11; col++) {
                // +1 because the first bit is R(3) and it's not used so we can ignore that.
                bptc->raw[col+row*15+1] = bptc->dec[dbp++];
            }
        }

        // +1 because the first bit is R(3) and it's not used so we can ignore that.
        bptc_196_96_hamming_15_11_3_get_parity_bits(&bptc->raw[row*15+1], &error_vector);
        bptc->raw[row*15+11+1] = error_vector.bits[0];
        bptc->raw[row*15+12+1] = error_vector.bits[1];
        bptc->raw[row*15+13+1] = error_vector.bits[2];
        bptc->raw[row*15+14+1] = error_vector.bits[3];
    }

    for (col = 0; col < 15; col++) {
        for (row = 0; row < 9; row++)
            column_bits[row] = bptc->raw[col+row*15+1];

        bptc_196_96_hamming_13_9_3_get_parity_bits(column_bits, &error_vector);
        bptc->raw[col+135+1] = error_vector.bits[0];
        bptc->raw[col+135+15+1] = error_vector.bits[1];
        bptc->raw[col+135+30+1] = error_vector.bits[2];
        bptc->raw[col+135+45+1] = error_vector.bits[3];
    }

    return 0;
}

int dmrpacket_data_bptc_interleave(bptc_196_96_t *bptc) {
    uint16_t i;
      for (i = 0; i < 196U; i++) bptc->dec[(i * 181U) % 196U] = bptc->raw[i];
    return 0;
}

int bptc_196_96_encode(bptc_196_96_t *bptc, uint8_t *packet, uint8_t *data)
{
  if (bptc == NULL || packet == NULL || data == NULL)
    return -1;
  bytes_to_bits(data, 12, bptc->dec, 96);

  bptc_196_96_generate(bptc);
  dmrpacket_data_bptc_interleave(bptc);

  bits_to_bytes(&bptc->dec[0]  , 96, &packet[0] , 12);
  uint8_t byte = bits_to_byte(&bptc->dec[96]);
  packet[12] = (packet[12] & 0x3f) | ((byte >> 0) & 0xc0);
  packet[13] = (packet[13] & 0xfc) | ((byte >> 4) & 0x03);
  bits_to_bytes(&bptc->dec[100], 96, &packet[21], 12);
  
  return 0;
}

/*
 * Quadres (16, 7)
 */

static uint16_t qr_16_7_encoder[] = {
  0x0000U, 0x0273U, 0x04E5U, 0x0696U, 0x09C9U, 0x0BBAU,
  0x0D2CU, 0x0F5FU, 0x11E2U, 0x1391U, 0x1507U, 0x1774U,
  0x182BU, 0x1A58U, 0x1CCEU, 0x1EBDU, 0x21B7U, 0x23C4U,
  0x2552U, 0x2721U, 0x287EU, 0x2A0DU, 0x2C9BU, 0x2EE8U,
  0x3055U, 0x3226U, 0x34B0U, 0x36C3U, 0x399CU, 0x3BEFU,
  0x3D79U, 0x3F0AU, 0x411EU, 0x436DU, 0x45FBU, 0x4788U,
  0x48D7U, 0x4AA4U, 0x4C32U, 0x4E41U, 0x50FCU, 0x528FU,
  0x5419U, 0x566AU, 0x5935U, 0x5B46U, 0x5DD0U, 0x5FA3U,
  0x60A9U, 0x62DAU, 0x644CU, 0x663FU, 0x6960U, 0x6B13U,
  0x6D85U, 0x6FF6U, 0x714BU, 0x7338U, 0x75AEU, 0x77DDU,
  0x7882U, 0x7AF1U, 0x7C67U, 0x7E14U, 0x804FU, 0x823CU,
  0x84AAU, 0x86D9U, 0x8986U, 0x8BF5U, 0x8D63U, 0x8F10U,
  0x91ADU, 0x93DEU, 0x9548U, 0x973BU, 0x9864U, 0x9A17U,
  0x9C81U, 0x9EF2U, 0xA1F8U, 0xA38BU, 0xA51DU, 0xA76EU,
  0xA831U, 0xAA42U, 0xACD4U, 0xAEA7U, 0xB01AU, 0xB269U,
  0xB4FFU, 0xB68CU, 0xB9D3U, 0xBBA0U, 0xBD36U, 0xBF45U,
  0xC151U, 0xC322U, 0xC5B4U, 0xC7C7U, 0xC898U, 0xCAEBU,
  0xCC7DU, 0xCE0EU, 0xD0B3U, 0xD2C0U, 0xD456U, 0xD625U,
  0xD97AU, 0xDB09U, 0xDD9FU, 0xDFECU, 0xE0E6U, 0xE295U,
  0xE403U, 0xE670U, 0xE92FU, 0xEB5CU, 0xEDCAU, 0xEFB9U,
  0xF104U, 0xF377U, 0xF5E1U, 0xF792U, 0xF8CDU, 0xFABEU,
  0xFC28U, 0xFE5BU
};

void qr_16_7_encode(uint8_t buf[2])
{
  uint16_t dv = buf[0] >> 1;
  uint16_t qr = qr_16_7_encoder[dv & 0x7fU];
  buf[0] = qr >> 8;
  buf[1] = qr & 0xffU;
}

/*
 * Link Control
 */

int full_lc_encode(full_lc_t *lc, uint8_t packet[33], data_type_t data_type)
{
  rs_12_9_checksum_t *parity = rs_12_9_calc_checksum((rs_12_9_codeword_t *)lc->bytes);

  switch (data_type) {
    case DATA_TYPE_VOICE_LC:
      lc->fields.crc[0] = parity->bytes[0] ^ CRC_MASK_VOICE_LC[0];
      lc->fields.crc[1] = parity->bytes[1] ^ CRC_MASK_VOICE_LC[1];
      lc->fields.crc[2] = parity->bytes[2] ^ CRC_MASK_VOICE_LC[2];
      break;
    case DATA_TYPE_TERMINATOR_WITH_LC:
      lc->fields.crc[0] = parity->bytes[0] ^ CRC_MASK_TERMINATOR_WITH_LC[0];
      lc->fields.crc[1] = parity->bytes[1] ^ CRC_MASK_TERMINATOR_WITH_LC[1];
      lc->fields.crc[2] = parity->bytes[2] ^ CRC_MASK_TERMINATOR_WITH_LC[2];
      break;
    default:
      break;
  }
  bptc_196_96_t bptc;
  return bptc_196_96_encode(&bptc, packet, lc->bytes);
}

/*
 * Embedded signalling
 */

int emb_encode(emb_t *emb, uint8_t packet[33])
{
  if (emb == NULL || packet == NULL)
    return -1;
        
  qr_16_7_encode(emb->bytes);
  packet[13] = (packet[13] & 0xf0U) | (emb->bytes[0] >> 4);
  packet[14] = (packet[14] & 0x0fU) | (emb->bytes[0] << 4);
  packet[18] = (packet[18] & 0xf0U) | (emb->bytes[1] >> 4);
  packet[19] = (packet[19] & 0x0fU) | (emb->bytes[1] << 4);
  return 0;
}

/*
 * Generate header, terminator and EMB bytes for a given full LC (72 bits + 24 bits empty).
 */

int generate_headers(full_lc_t *lc, emb_t *emb, uint8_t header[33], uint8_t terminator[33], uint8_t embdata[33])
{
  int ret;
  
  if ((ret = full_lc_encode(lc, header, DATA_TYPE_VOICE_LC)) != 0)
    return ret;
    
  if ((ret = full_lc_encode(lc, terminator, DATA_TYPE_TERMINATOR_WITH_LC)) != 0)
    return ret;

  if ((ret = emb_encode(emb, embdata)) != 0)
    return ret;

  return 0;
}


/*
 * Generate Slot Type
 */

void slot_type_encode(uint8_t color_code, uint8_t data_type, uint8_t data[33])
{
    uint8_t bytes[3];
    memset(bytes, 0, sizeof(bytes));
    bytes[0] = (color_code << 4) | (data_type & 0x0f);
    golay_20_8_encode(bytes);

    data[12] = (data[12] & 0xc0) | ((bytes[0] >> 2) & 0x3f);
    data[13] = (data[13] & 0x0f) | ((bytes[0] << 6) & 0xc0) | ((bytes[1] >> 2) & 0x30);
    data[19] = (data[19] & 0xf0) | ((bytes[1] >> 2) & 0x0f);
    data[20] = (data[20] & 0x03) | ((bytes[1] << 6) & 0xc0) | ((bytes[2] >> 2) & 0x3c);
}

/*
 * Add sync word
 */

int dmr_sync_pattern_encode(uint8_t packet[33], dmr_sync_pattern pattern)
{
    if (packet == NULL)
        return -1;

    const uint8_t *sync_pattern;
    switch (pattern) {
        case DMR_SYNC_PATTERN_BS_SOURCED_VOICE:
            sync_pattern = dmr_sync_pattern_bs_sourced_voice;
            break;
        case DMR_SYNC_PATTERN_BS_SOURCED_DATA:
            sync_pattern = dmr_sync_pattern_bs_sourced_data;
            break;
        case DMR_SYNC_PATTERN_MS_SOURCED_VOICE:
            sync_pattern = dmr_sync_pattern_ms_sourced_voice;
            break;
        case DMR_SYNC_PATTERN_MS_SOURCED_DATA:
            sync_pattern = dmr_sync_pattern_ms_sourced_data;
            break;
        case DMR_SYNC_PATTERN_MS_SOURCED_RC:
            sync_pattern = dmr_sync_pattern_ms_sourced_rc;
            break;
        case DMR_SYNC_PATTERN_DIRECT_VOICE_TS1:
            sync_pattern = dmr_sync_pattern_direct_voice_ts1;
            break;
        case DMR_SYNC_PATTERN_DIRECT_DATA_TS1:
            sync_pattern = dmr_sync_pattern_direct_data_ts1;
            break;
        case DMR_SYNC_PATTERN_DIRECT_VOICE_TS2:
            sync_pattern = dmr_sync_pattern_direct_voice_ts2;
            break;
        case DMR_SYNC_PATTERN_DIRECT_DATA_TS2:
            sync_pattern = &dmr_sync_pattern_direct_data_ts2[0];
            break;
        case DMR_SYNC_PATTERN_UNKNOWN:
        default:
            sync_pattern = dmr_sync_pattern_unknown; 
            return -1;
    }

    uint8_t i;
    for (i = 0; i < 7; i++) {
        packet[i + 13] = (packet[i + 13] & ~dmr_sync_pattern_mask[i]) | sync_pattern[i];
    }
    return 0;
}
