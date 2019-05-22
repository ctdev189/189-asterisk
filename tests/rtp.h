#ifdef __MACH__
#include <machine/endian.h>
#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#else
#include <endian.h>
#endif

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct rtp_header
{
#if __BYTE_ORDER == __BIG_ENDIAN
    uint16_t version : 2;
    uint16_t padding : 1;
    uint16_t extension : 1;
    uint16_t csrccount : 4;
    uint16_t markerbit : 1;
    uint16_t type : 7;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t csrccount : 4;
    uint16_t extension : 1;
    uint16_t padding : 1;
    uint16_t version : 2;
    uint16_t type : 7;
    uint16_t markerbit : 1;
#endif
    uint16_t seq_number;
    uint32_t timestamp;
    uint32_t ssrc;
    uint32_t csrc[16];
} rtp_header;
typedef rtp_header dev189_rtp_header;