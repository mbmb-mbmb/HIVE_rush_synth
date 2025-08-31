#include "synth.h"

void	add_synth_to_mixer(t_mixer *mixer, t_synth *synth, int voice_index)
{
	if(voice_index < mixer->num_voices)
		mixer->synths[voice_index] = synth;
}

t_mixer	*create_mixer(int num_voices)
{
	t_mixer	*mixer = malloc(sizeof(t_mixer));
	mixer->mixbuffer = malloc(FRAMES_PER_BUFFER * sizeof(float));
	mixer->num_voices = num_voices;
	mixer->synths = malloc(num_voices * sizeof(t_synth*));
	return (mixer);
}

void	destroy_mixer_and_synths(t_mixer *mixer)
{
	int	i;
	
	i = 0;
	while(i < mixer->num_voices)
	{
		if(mixer->synths[i] != NULL)
		{
			free(mixer->synths[i]->wavetable);
			free(mixer->synths[i]);
		}
		i++;
	}
	free(mixer->synths);
	free(mixer->mixbuffer);
	free(mixer);
}