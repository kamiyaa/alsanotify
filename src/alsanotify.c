#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <libnotify/notify.h>
#include <libnotify/notification.h>

#define SOUNDCARD "default"

long alsa_get_max_vol(snd_mixer_t *handle)
{
	snd_mixer_selem_id_t *sid;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, "Master");

	long min, max;

	snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);

        snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

//	snd_mixer_close(handle);

	return max;
}

snd_mixer_t *create_alsa_handle()
{
	snd_mixer_t *handle;

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, SOUNDCARD);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	return handle;
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

	snd_mixer_selem_set_playback_volume(elem,
		SND_MIXER_SCHN_FRONT_LEFT, volume);
	snd_mixer_selem_set_playback_volume(elem,
		SND_MIXER_SCHN_FRONT_RIGHT, volume);

	snd_mixer_selem_get_playback_volume(elem,
		SND_MIXER_SCHN_FRONT_LEFT, &volume);

	return volume * 100;
}

/* struct for holding command line arguments */
struct alsanotify_args {
	bool set;
};

int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stderr, "alsanotify [options] change\n");
		fprintf(stderr, "  -s\t set volume\n");
		return 1;
	}

	extern int optind;
	extern char *optarg;

	struct alsanotify_args args = {
		.set = false,
	};

	if (notify_init("Volume") == FALSE) {
		fprintf(stderr, "Failed to initialized notify_init()");
		return 1;
	}

	int change = atoi(argv[optind]);

	snd_mixer_t *alsa_handle = create_alsa_handle();
	long alsa_max_vol = alsa_get_max_vol(alsa_handle);

	long vol_change = alsa_max_vol * change / 100;
	unsigned int volume = alsa_change_volume(alsa_handle, vol_change) / alsa_max_vol;

	static char buf[4];
	snprintf(buf, 4, "%u%%", volume);

	NotifyNotification *notif = notify_notification_new("Volume", buf, 0);
	notify_notification_set_timeout(notif, NOTIFY_EXPIRES_DEFAULT);

	GError *err = NULL;

	if (notify_notification_show(notif, &err) == FALSE) {
		fprintf(stderr, "%s\n", err->message);
		return 1;
	}

	notify_uninit();

	return 0;
}
