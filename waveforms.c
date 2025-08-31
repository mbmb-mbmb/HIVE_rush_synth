#include "synth.h"

static void generate_sine(float *wavetable)
{
	int	i;

	i = 0;
	while(i < TABLE_SIZE)
	{
		wavetable[i] = sin(2.0 * M_PI * (double)i / (double)TABLE_SIZE);
		i++;
	}
}

static void generate_square(float *wavetable)
{
	int	i;

	i = 0;
	while(i < TABLE_SIZE)
	{
		if(i <= TABLE_SIZE / 2)
			wavetable[i] = 1.0;
		else
			wavetable[i] = -1.0;
		i++;
	}
}
static void generate_triangle(float *wavetable)
{
	int		i;
	float	t;

	i = 0;
	while(i < TABLE_SIZE)
	{
		t = (float)i / TABLE_SIZE;
		wavetable[i] = 2.0 * fabs(2.0 * t - 1.0) - 1.0;
		i++;
	}
}

static void generate_saw(float *wavetable)
{
	int	i;

	i = 0;
	while(i < TABLE_SIZE)
	{
		wavetable[i] = (i / (float)TABLE_SIZE) * 2.0 - 1.0;
		i++;
	}
}

void	choose_waveform(float *wavetable, const char *waveform)
{
	if(strcmp(waveform, "sine") == 0)
		generate_sine(wavetable);
	if(strcmp(waveform, "square") == 0)
		generate_square(wavetable);
	if(strcmp(waveform, "triangle") == 0)
		generate_triangle(wavetable);
	if(strcmp(waveform, "saw") == 0)
		generate_saw(wavetable);
}