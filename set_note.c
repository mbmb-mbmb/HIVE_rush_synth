#include "synth.h"

double	note_to_freq(char *note)
{
	int	i;

	t_ntof	notetable[] =
	{
		{"c3", 130.81},
		{"c3#", 138.59},
		{"d3", 146.83},
		{"d3#", 155.56},
		{"e3", 164.81},
		{"f3", 174.61},
		{"f3#", 185.00},
		{"g3", 196.00},
		{"g3#", 207.65},
		{"a3", 220.0},
		{"a3#", 233.08},
		{"b3", 246.94},
		{"c4", 261.63},
		{"c4#", 277.18},
		{"d4", 293.66},
		{"d4#", 311.13},
		{"e4", 329.63},
		{"f4", 349.23},
		{"f4#", 369.99},
		{"g4", 392.00},
		{"g4#", 415.30},
		{"a4", 440.00},
		{"a4#", 466.16},
		{"b4", 493.88},
		{"c5", 523.25},
		{"c5#", 554.37},
		{"d5", 587.33},
		{"d5#", 622.25},
		{"e5", 659.26},
		{"b4", 493.88},
		{"c5", 523.25},
		{"c5#", 554.37},
		{"d5", 587.33},
		{"d5#", 622.25},
		{"e5", 659.26}
	};
	i = 0;
	while(notetable[i].note != NULL)
	{
		if(strcmp(note, notetable[i].note) == 0)
			return notetable[i].freq;
		i++;
	}
	return (0.0);
}

void	set_note(t_synth *synth, char *note, double amplitude)
{
	double	freq = note_to_freq(note);
	synth->frequency = freq;
	synth->phaseIncrement = freq / SAMPLE_RATE;
	synth->amplitude = amplitude;
}
