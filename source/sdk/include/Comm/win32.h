#ifndef _WIN32_DRAND48_KYOSHO_2015_08321_
#define _WIN32_DRAND48_KYOSHO_2015_08321_

#if defined(_WINDOWS)

#define MNWZ 0x100000000
#define ANWZ 0x5DEECE66D
#define CNWZ 0xB16
#define INFINITY 0xFFFFFFFFF

int labelsize;
int dim;

static unsigned long long seed = 1;

namespace W32_RAND{

	double drand48(void)
	{
		seed = (ANWZ * seed + CNWZ) & 0xFFFFFFFFFFFFLL;
		unsigned int x = seed >> 16;
		return ((double)x / (double)MNWZ);
	}

	//static unsigned long long seed = 1;

	void srand48(unsigned int i)
	{
		seed = (((long long int)i) << 16) | rand();
	}
}
#endif//_WINDOWS
#endif//_WIN32_DRAND48_KYOSHO_2015_08321_

