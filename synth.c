#include "synth.h"

void	choose_waveform(float *wavetable, const char *waveform)
{
	int		i;
	float	t;

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
	if(strcmp(waveform, "triangle") == 0)
	{
		while(i < TABLE_SIZE)
		{
			t = (float)i / TABLE_SIZE;
			wavetable[i] = 2.0 * fabs(2.0 * t - 1.0) - 1.0;
			i++;
		}
	}
	if(strcmp(waveform, "saw") == 0)
	{
		while(i < TABLE_SIZE)
		{
			wavetable[i] = (i / (float)TABLE_SIZE) * 2.0 - 1.0;
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

t_synth	*create_synth_from_type(char *type)
{
	if (strcmp(type, "sine") == 0)
		return create_synth("sine");
	else if (strcmp(type, "square") == 0)
		return create_synth("square");
	else if (strcmp(type, "triangle") == 0)
		return create_synth("triangle");
	else if (strcmp(type, "saw") == 0)
		return create_synth("saw");
	else
		return create_synth("sine");
}

t_mixer	*create_mixer(int num_voices)
{
	t_mixer	*mixer = malloc(sizeof(t_mixer));
	mixer->mixbuffer = malloc(FRAMES_PER_BUFFER * sizeof(float));
	mixer->num_voices = num_voices;
	mixer->synths = malloc(num_voices * sizeof(t_synth*));
	return (mixer);
}

void	add_synth_to_mixer(t_mixer *mixer, t_synth *synth, int voice_index)
{
	if(voice_index < mixer->num_voices)
		mixer->synths[voice_index] = synth;
}

void	destroy_synth(t_synth *synth)
{
	free(synth->wavetable);
	free(synth);
}

void	destroy_synths_and_mixer(t_mixer *mixer)
{
	int	i;

	i = 0;
	while(i < mixer->num_voices)
	{
		if(mixer->synths[i] != NULL)
			destroy_synth(mixer->synths[i]);
		i++;
	}
	free(mixer->mixbuffer);
	free(mixer->synths);
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
	memset(mixer->mixbuffer, 0, framesPerBuffer * sizeof(float));
	i = 0;
	while(i < mixer->num_voices)
	{
		if(mixer->synths[i] != NULL)
			synth_to_mix_with_pitch(mixer->synths[i], mixer);
		i++;
	}
	memcpy(out, mixer->mixbuffer, framesPerBuffer * sizeof(float));
	return (paContinue);
}

int	main(void)
{
	int	num_voices;
	int	i;
	t_synth *synth;
	// from parsing:
	num_voices = NUM_VOICES;
	char	*track_types[] = {"sine", "square", "triangle", "saw"};
	
	t_mixer *mixer = create_mixer(num_voices);
	Pa_Initialize();
	PaStream	*stream;
	
	i = 0;
	while(i < num_voices)
	{
		synth = create_synth_from_type(track_types[i]);
		add_synth_to_mixer(mixer, synth, i);
 		i++;
	}
	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, mixer);
	Pa_StartStream(stream);
	set_note(mixer->synths[0], "g3", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[1], "b3", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[0], "d3", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[1], "d4", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[1], "d4", 0);
	Pa_Sleep(500);
	set_note(mixer->synths[2], "g3", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[3], "b3", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[2], "d3", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[3], "d4", 1);
	Pa_Sleep(500);
	set_note(mixer->synths[3], "d4", 0);
	Pa_Sleep(500);
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
	destroy_synths_and_mixer(mixer);
	return (0);
}
