#ifndef SYNTH_H
#define SYNTH_H

//#include "/opt/homebrew/include/portaudio.h"
#include "portaudio.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAMPLE_RATE			48000
#define FRAMES_PER_BUFFER	256
#define TABLE_SIZE			2048
#define NUM_VOICES			3

typedef struct s_synth
{
	double	phase;
	double	phaseIncrement;
	float	*wavetable;
	double	frequency;
	double	amplitude;
}	t_synth;

typedef struct s_ntof
{
	char	*note;
	double	freq;
}	t_ntof;

void	set_note(t_synth *synth, float *mix_buffer, char *note, double amplitude);
double	note_to_freq(char *note);

#endif
