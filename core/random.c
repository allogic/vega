#include <stdlib.h>
#include <time.h>

#include <vega/core/random.h>

#include <vega/core/debug/tracy.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void random_seed(char const* seed)
{
	TRACY_ZONE_BEGIN

	if (seed)
	{
		uint32_t hash = 0;

		while (*seed++)
		{
			hash = hash * 31 + (uint32_t)*seed;
		}

		srand(hash);
	}
	else
	{
		srand((uint32_t)time(0));
	}

	TRACY_ZONE_END
}
int8_t random_int8(void)
{
	TRACY_ZONE_BEGIN

	int8_t value = (int8_t)((rand() % 0x100U) - 0x7F);

	TRACY_ZONE_END

	return value;
}
int16_t random_int16(void)
{
	TRACY_ZONE_BEGIN

	int16_t value = (int16_t)((rand() % 0x10000U) - 0x7FFF);

	TRACY_ZONE_END

	return value;
}
int32_t random_int32(void)
{
	TRACY_ZONE_BEGIN

	int32_t value = (int32_t)(((rand() << 16) | rand()) - 0x7FFFFFFF);

	TRACY_ZONE_END

	return value;
}
int64_t random_int64(void)
{
	TRACY_ZONE_BEGIN

	int64_t value = (int64_t)((((int64_t)((rand() << 16) | rand()) << 32) | (int64_t)((rand() << 16) | rand())) - 0x7FFFFFFFFFFFFFFF);

	TRACY_ZONE_END

	return value;
}
int8_t random_int8_between(int8_t min, int8_t max)
{
	TRACY_ZONE_BEGIN

	int8_t value = (int8_t)(min + rand() % (max - min + 1));

	TRACY_ZONE_END

	return value;
}
int16_t random_int16_between(int16_t min, int16_t max)
{
	TRACY_ZONE_BEGIN

	int16_t value = (int16_t)(min + rand() % (max - min + 1));

	TRACY_ZONE_END

	return value;
}
int32_t random_int32_between(int32_t min, int32_t max)
{
	TRACY_ZONE_BEGIN

	int32_t value = (int32_t)(min + (int32_t)rand() * ((int64_t)(max - min + 1)) / (RAND_MAX + 1));

	TRACY_ZONE_END

	return value;
}
int64_t random_int64_between(int64_t min, int64_t max)
{
	TRACY_ZONE_BEGIN

	int64_t value = (int64_t)(min + (int64_t)rand() * ((int64_t)(max - min + 1)) / (RAND_MAX + 1));

	TRACY_ZONE_END

	return value;
}
uint8_t random_uint8(void)
{
	TRACY_ZONE_BEGIN

	uint8_t value = (uint8_t)(rand() % 0x100ULL);

	TRACY_ZONE_END

	return value;
}
uint16_t random_uint16(void)
{
	TRACY_ZONE_BEGIN

	uint16_t value = (uint16_t)((rand() % 0x10000ULL));

	TRACY_ZONE_END

	return value;
}
uint32_t random_uint32(void)
{
	TRACY_ZONE_BEGIN

	uint32_t value = ((uint32_t)rand() << 16) | (uint32_t)rand();

	TRACY_ZONE_END

	return value;
}
uint64_t random_uint64(void)
{
	TRACY_ZONE_BEGIN

	uint64_t value = (uint64_t)(((uint64_t)(((uint32_t)rand() << 16) | (uint32_t)rand()) << 32) | (uint64_t)(((uint32_t)rand() << 16) | (uint32_t)rand()));

	TRACY_ZONE_END

	return value;
}
uint8_t random_uint8_between(uint8_t min, uint8_t max)
{
	TRACY_ZONE_BEGIN

	uint8_t value = (uint8_t)(min + rand() % (max - min + 1));

	TRACY_ZONE_END

	return value;
}
uint16_t random_uint16_between(uint16_t min, uint16_t max)
{
	TRACY_ZONE_BEGIN

	uint16_t value = (uint16_t)(min + rand() % (max - min + 1));

	TRACY_ZONE_END

	return value;
}
uint32_t random_uint32_between(uint32_t min, uint32_t max)
{
	TRACY_ZONE_BEGIN

	uint32_t value = (uint32_t)(min + (uint32_t)rand() * ((uint64_t)(max - min + 1)) / (RAND_MAX + 1));

	TRACY_ZONE_END

	return value;
}
uint64_t random_uint64_between(uint64_t min, uint64_t max)
{
	TRACY_ZONE_BEGIN

	uint64_t value = (uint64_t)(min + (uint64_t)rand() * ((uint64_t)(max - min + 1)) / (RAND_MAX + 1));

	TRACY_ZONE_END

	return value;
}
float random_real32(void)
{
	TRACY_ZONE_BEGIN

	float value = (float)rand() / (float)RAND_MAX;

	TRACY_ZONE_END

	return value;
}
double random_real64(void)
{
	TRACY_ZONE_BEGIN

	double value = (double)rand() / (double)RAND_MAX;

	TRACY_ZONE_END

	return value;
}
float random_real32_between(float min, float max)
{
	TRACY_ZONE_BEGIN

	float value = min + (max - min) * ((float)rand() / (float)RAND_MAX);

	TRACY_ZONE_BEGIN

	return value;
}
double random_real64_between(double min, double max)
{
	TRACY_ZONE_BEGIN

	double value = min + (max - min) * ((double)rand() / (double)RAND_MAX);

	TRACY_ZONE_BEGIN

	return value;
}
