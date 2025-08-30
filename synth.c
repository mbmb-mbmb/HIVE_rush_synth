#include "synth.h"

void	choose_waveform(float *wavetable, const char *waveform)
{
	int	i;

	i = 0;
	if(strcmp(waveform, "sine") == 0)
	{
		while(i < TABLE_SIZE)
		{
			wavetable[i] = sin(2.0 * M_PI * (double)i / (double)TABLE_SIZE);
			i++;
		}
	}
	if(strcmp(waveform, "square") == 0)
	{
		while(i < TABLE_SIZE)
		{
			if(i <= TABLE_SIZE / 2)
				wavetable[i] = 1.0;
			else
				wavetable[i] = -1.0;
			i++;
		}
	}
}

t_synth* create_synth(const char *waveform, double freq, double amplitude)
{
	t_synth	*synth;

	synth = malloc(sizeof(t_synth));
	synth->wavetable = malloc(TABLE_SIZE * sizeof(float));
	synth->phase = 0.0;
	synth->phaseIncrement = freq / SAMPLE_RATE;
	synth->amplitude = amplitude;
	synth->frequency = freq;
	choose_waveform(synth->wavetable, waveform);
	return synth;
}

void destroy_synth(t_synth *synth)
{
	free(synth->wavetable);
	free(synth);
}

static int paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData)
{
	float	*in;
	float	*out;
	int		i;

	in = (float*)userData;
	out = (float*)outputBuffer;
	i = 0;
	while(i < framesPerBuffer)
	{
		*out++ = in[i] * (1 / NUM_VOICES);
		i++;
	}
	return (paContinue);
}

int	main(void)
{
	t_synth	*synth1;
	float	*mix;

	mix = malloc(FRAMES_PER_BUFFER * sizeof(float));
	synth1 = create_synth("sine", note_to_freq("c4"), 0.5);
	Pa_Initialize();
	PaStream	*stream;

	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, mix);
	Pa_StartStream(stream);
	Pa_Sleep(1000);
	set_note(synth1, mix, "g3", 0.5);
	Pa_Sleep(1000);
	set_note(synth1, mix, "c4", 0.5);
	Pa_Sleep(1000);
	set_note(synth1, mix, "d4", 0.5);
	Pa_Sleep(1000);
	set_note(synth1, mix, "e4", 0.5);
	Pa_Sleep(1500);
	set_note(synth1, mix, "g4", 0.5);
	Pa_Sleep(500);
	set_note(synth1, mix, "e4", 0.5);
	Pa_Sleep(1000);
	set_note(synth1, mix, "d4", 0.5);
	Pa_Sleep(1000);
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
	destroy_synth(synth1);
	free(mix);
	return (0);
}
