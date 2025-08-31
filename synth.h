#ifndef SYNTH_H
#define SYNTH_H

#include "portaudio.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SAMPLE_RATE			48000
#define FRAMES_PER_BUFFER	256
#define TABLE_SIZE			2048
#define NUM_VOICES			4

typedef enum e_track_type
{
	SINE,
	SAW,
	TRIANGLE,
	SQUARE
}	t_track_type;

typedef struct s_synth
{
	double	phase;
	double	phaseIncrement;
	float	*wavetable;
	double	frequency;
	double	amplitude;
}	t_synth;

typedef struct s_mixer
{
	float	*mixbuffer;
	int		num_voices;
	t_synth	**synths;
}	t_mixer;

typedef struct s_ntof
{
	char	*note;
	double	freq;
}	t_ntof;

double	note_to_freq(char *note);
void	set_note(t_synth *synth, char *note, double amplitude);
t_mixer	*create_mixer(int num_voices);
void	add_synth_to_mixer(t_mixer *mixer, t_synth *synth, int voice_index);
void	choose_waveform(float *wavetable, t_track_type waveform_type);
void	render_synth_to_buffer(t_synth *synth, t_mixer *mixer);
void	destroy_mixer_and_synths(t_mixer *mixer);
#endif
