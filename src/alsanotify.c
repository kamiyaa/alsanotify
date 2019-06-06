#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <libnotify/notify.h>
#include <libnotify/notification.h>

#include "volume.h"

/* struct for holding command line arguments */
struct alsanotify_args {
	bool set;
};

int main(int argc, char **argv)
{
	if (argc == 1) {
		selems();
		return 0;
	}

	extern int optind;
	extern char *optarg;

	struct alsanotify_args args = {
		.set = false,
	};

	const char *interface = "Master";
	int c, change = 0, exit_status = 0;
	while ((c = getopt(argc, argv, "i:s:c:")) != EOF) {
		switch(c) {
		case 'i':
			interface = optarg;
			break;
		case 's':
			args.set = true;
			change = atoi(optarg);
			break;
		case 'c':
			change = atoi(optarg);
			break;
		default:
			exit_status = 1;
			break;
		}
	}

	if (exit_status || change == 0) {
		fprintf(stderr, "%s options volume\n", argv[0]);
		return 1;
	}

	if (args.set == true) {
		if (change < 0) {
			fprintf(stderr, "Cannot set negative volume\n");
			return 1;
		}
	}

	// printf("interface %s\n", interface);

	snd_mixer_t *alsa_handle = create_alsa_handle();

	unsigned int volume = alsa_change_by_percent(alsa_handle, interface, change);
	snd_mixer_close(alsa_handle);

	static char buf[5];
	snprintf(buf, 5, "%u%%", volume);

	if (notify_init("Volume") == FALSE) {
		fprintf(stderr, "Failed to initialized notify_init()");
		return 1;
	}

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
