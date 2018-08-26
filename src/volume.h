#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

snd_mixer_t *create_alsa_handle();
long alsa_get_max_vol(snd_mixer_t *handle);
unsigned int alsa_change_volume(snd_mixer_t *handle, long change);
int selems();
