#include "synth.h"

void	choose_waveform(float *wavetable, char *waveform)
{
	if(strcmp(waveform, "sine") == 0)
	{
		int i = 0;
		while(i < TABLE_SIZE)
		{
			wavetable[i] = sin(2.0 * M_PI * (double)i / (double)TABLE_SIZE);
			i++;	
		}
	}
}

t_synth* create_synth(const char *waveform, double freq)
{
	t_synth *synth = malloc(sizeof(t_synth));
	synth->wavetable = malloc(TABLE_SIZE * sizeof(float));
	synth->phase = 0.0;
	synth->phaseIncrement = freq / SAMPLE_RATE;
	synth->amplitude = 0.5;
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
	t_synth *data = (t_synth*)userData;
	float *out = (float*)outputBuffer;
	int	i = 0;
	while(i < framesPerBuffer)
	{
		int idx = (int)(data->phase * TABLE_SIZE) % TABLE_SIZE;
		*out++ = data->wavetable[idx];
		data->phase += data->phaseIncrement;
		if(data->phase >= 1.0)
			data->phase -= 1.0;
		i++;
	}
	return (paContinue);
}

int	main(void)
{
	t_synth *synth1;
	
	synth1 = create_synth("sine", note_to_freq("c4"));
	Pa_Initialize();
	PaStream *stream;
	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, synth1);
	Pa_StartStream(stream);
	Pa_Sleep(1000);
	set_note(synth1, "g3");
	Pa_Sleep(1000);
	set_note(synth1, "c4");
	Pa_Sleep(1000);
	set_note(synth1, "d4");
	Pa_Sleep(1000);
	set_note(synth1, "e4");
	Pa_Sleep(1500);
	set_note(synth1, "g4");
	Pa_Sleep(500);
	set_note(synth1, "e4");
	Pa_Sleep(1000);
	set_note(synth1, "d4");
	Pa_Sleep(1000);
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
	destroy_synth(synth1);
	return (0);
}
