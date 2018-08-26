#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

#include "volume.h"

#define SOUNDCARD "default"

snd_mixer_t *create_alsa_handle()
{
	snd_mixer_t *handle;

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, SOUNDCARD);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	return handle;
}

long alsa_get_max_vol(snd_mixer_t *handle)
{
	snd_mixer_selem_id_t *sid;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	long min, max;

	snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);

        snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

	return max;
}

unsigned int alsa_change_volume(snd_mixer_t *handle, long change)
{
	long volume;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	elem = snd_mixer_find_selem(handle, sid);

	snd_mixer_selem_get_playback_volume(elem,
		SND_MIXER_SCHN_FRONT_LEFT, &volume);

	volume += change;

	snd_mixer_selem_set_playback_volume_all(elem, volume);
//	snd_mixer_selem_set_playback_volume(elem,
//		SND_MIXER_SCHN_FRONT_RIGHT, volume);

	snd_mixer_selem_get_playback_volume(elem,
		SND_MIXER_SCHN_FRONT_LEFT, &volume);

	return volume * 100;
}

int selems()
{
	int err;
	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_alloca(&sid);
	
	if ((err = snd_mixer_open(&handle, 0)) < 0) {
		fprintf(stderr, "Mixer %s open error: %s", SOUNDCARD, snd_strerror(err));
		return err;
	}
	if ((err = snd_mixer_attach(handle, SOUNDCARD)) < 0) {
		fprintf(stderr, "Mixer attach %s error: %s", SOUNDCARD, snd_strerror(err));
		snd_mixer_close(handle);
		return err;
	}
	if ((err = snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
		fprintf(stderr, "Mixer register error: %s", snd_strerror(err));
		snd_mixer_close(handle);
		return err;
	}
	err = snd_mixer_load(handle);
	if (err < 0) {
		fprintf(stderr, "Mixer %s load error: %s", SOUNDCARD, snd_strerror(err));
		snd_mixer_close(handle);
		return err;
	}
	for (elem = snd_mixer_first_elem(handle); elem; elem = snd_mixer_elem_next(elem)) {
		snd_mixer_selem_get_id(elem, sid);
		printf("Simple mixer control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
	}
	snd_mixer_close(handle);
	return 0;
}

