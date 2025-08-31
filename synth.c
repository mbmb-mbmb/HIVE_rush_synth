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

t_synth	*create_synth(const char *waveform)
{
	t_synth	*synth;

	synth = malloc(sizeof(t_synth));
	synth->wavetable = malloc(TABLE_SIZE * sizeof(float));
	synth->phase = 0.0;
	synth->phaseIncrement = 0.1;
	synth->amplitude = 0.0;
	synth->frequency = 0.1;
	choose_waveform(synth->wavetable, waveform);
	return (synth);
}

t_mixer	*create_mixer(t_synth *synth1, t_synth *synth2)
{
	t_mixer	*mixer = malloc(sizeof(t_mixer));
	mixer->mixbuffer = malloc(FRAMES_PER_BUFFER * sizeof(float));
	mixer->num_voices = 1;
	mixer->one_synth = synth1;
	mixer->second_synth = synth2;
	return (mixer);
}

void	destroy_synth(t_synth *synth)
{
	free(synth->wavetable);
	free(synth);
}

void	destroy_mixer(t_mixer *mixer)
{
	free(mixer->mixbuffer);
	free(mixer);
}

void	synth_to_mix_with_pitch(t_synth *synth, t_mixer *mixer)
{
	int		i;
	float	*buffer_ptr;

	i = 0;
	buffer_ptr = mixer->mixbuffer;
	while(i < FRAMES_PER_BUFFER)
	{
		int wt_idx = (int)(synth->phase * TABLE_SIZE) % TABLE_SIZE;
		*buffer_ptr += synth->wavetable[wt_idx] * synth->amplitude * (1.0f / (float) NUM_VOICES);
		buffer_ptr++; 
		synth->phase += synth->phaseIncrement;
		if(synth->phase >= 1.0)
			synth->phase -= 1.0;
		i++;
	}
}

static int paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData)
{
	t_mixer	*mixer;
	float	*out;
	int		i;

	mixer = (t_mixer*)userData;
	out = (float*)outputBuffer;
	i = 0;
	memset(mixer->mixbuffer, 0, framesPerBuffer * sizeof(float));
	synth_to_mix_with_pitch(mixer->one_synth, mixer);
	synth_to_mix_with_pitch(mixer->second_synth, mixer);
	memcpy(out, mixer->mixbuffer, framesPerBuffer * sizeof(float));
	return (paContinue);
}

int	main(void)
{
	t_synth *synth1 = create_synth("square");
	t_synth	*synth2 = create_synth("sine");
	t_mixer *mixer = create_mixer(synth1, synth2);


	Pa_Initialize();
	PaStream	*stream;

	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, mixer);
	Pa_StartStream(stream);
	set_note(synth1, "g3", 1);
	Pa_Sleep(500);
	set_note(synth2, "b3", 1);
	Pa_Sleep(500);
	set_note(synth1, "d3", 1);
	Pa_Sleep(500);
	set_note(synth2, "d4", 1);
	Pa_Sleep(500);
	set_note(synth2, "d4", 0);
	Pa_Sleep(500);
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
	destroy_synth(synth1);
	destroy_synth(synth2);
	destroy_mixer(mixer);
	return (0);
}
