#ifndef VEGA_CORE_UUID_H
#define VEGA_CORE_UUID_H

#include <stdint.h>

typedef struct _uuid_t
{
	uint32_t time_low;
	uint16_t time_mid;
	uint16_t time_hi_and_version;
	uint8_t clock_seq_hi_and_reserved;
	uint8_t clock_seq_low;
	uint8_t node[6];
} uuid_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

uuid_t uuid_generate(void);
void uuid_print(uuid_t* uuid);

#endif // VEGA_CORE_UUID_H
