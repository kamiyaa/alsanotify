#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

snd_mixer_t *create_alsa_handle();
long alsa_get_max_vol(snd_mixer_t *handle, const char *interface);
unsigned int alsa_change_by_percent(snd_mixer_t *handle, const char *interface, int percent);
int selems();
