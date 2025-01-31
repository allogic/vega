#include <stdio.h>

#include <vega/core/random.h>
#include <vega/core/uuid.h>

#include <vega/core/debug/tracy.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

uuid_t uuid_generate(void)
{
	TRACY_ZONE_BEGIN

	uuid_t uuid = { 0 };

	uuid.time_low = random_uint32();
	uuid.time_mid = random_uint16();
	uuid.time_hi_and_version = (random_uint16() & 0x0FFF) | 0x4000;
	uuid.clock_seq_hi_and_reserved = (random_uint8() & 0x3F) | 0x80;
	uuid.clock_seq_low = random_uint8() & 0xFF;
	uuid.node[0] = random_uint8();
	uuid.node[1] = random_uint8();
	uuid.node[2] = random_uint8();
	uuid.node[3] = random_uint8();
	uuid.node[4] = random_uint8();
	uuid.node[5] = random_uint8();

	TRACY_ZONE_END

	return uuid;
}
void uuid_print(uuid_t* uuid)
{
	TRACY_ZONE_BEGIN

	printf("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid->time_low, uuid->time_mid, uuid->time_hi_and_version,
		uuid->clock_seq_hi_and_reserved, uuid->clock_seq_low,
		uuid->node[0], uuid->node[1], uuid->node[2], uuid->node[3], uuid->node[4], uuid->node[5]);

	TRACY_ZONE_END
}
