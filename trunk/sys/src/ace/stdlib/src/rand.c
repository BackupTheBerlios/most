/*
 * rand.c
 *
 *  Created on: 02.12.2012
 *      Author: maik
 */

#include <ace/stdlib.h>
#include <ace/stddef.h>

static ACE_u32_t x = 0;

static ACE_u32_t xorshift32() {
	if (x == 0) { x = 314159265; }
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}

extern void ACE_seed(ACE_u32_t seed)
{
	x = seed;
}

extern ACE_u32_t ACE_rand(void)
{
	return xorshift32();
}
