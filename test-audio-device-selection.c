
#include <pulse/pulseaudio.h>
#include "gvc-mixer-control.h"

typedef struct {
	GvcHeadsetPortChoice choice;
	gchar *name;
} AudioSelectionChoice;

static AudioSelectionChoice audio_selection_choices[] = {
	{ GVC_HEADSET_PORT_CHOICE_HEADPHONES,   "headphones" },
	{ GVC_HEADSET_PORT_CHOICE_HEADSET,      "headset" },
	{ GVC_HEADSET_PORT_CHOICE_MIC,          "microphone" },
};

static void
audio_selection_needed (GvcMixerControl      *control,
			guint                 id,
			gboolean              show_dialog,
			GvcHeadsetPortChoice  choices,
			gpointer              user_data)
{
	char *args[G_N_ELEMENTS (audio_selection_choices) + 1];
	char *choices_str;
	guint i, n;

	if (!show_dialog) {
		g_print ("Audio selection not needed anymore for id %d\n", id);
		return;
	}

	n = 0;
	for (i = 0; i < G_N_ELEMENTS (audio_selection_choices); ++i) {
		if (choices & audio_selection_choices[i].choice)
			args[n++] = audio_selection_choices[i].name;
	}
	args[n] = NULL;

	choices_str = g_strjoinv (", ", args);
	g_print ("+++ Audio selection needed for id %d\n", id);
	g_print ("    Choices are: %s\n", choices_str);
	g_free (choices_str);
}

int main (int argc, char **argv)
{
	GMainLoop *loop;
	GvcMixerControl *volume;

	loop = g_main_loop_new (NULL, FALSE);

	volume = gvc_mixer_control_new ("GNOME Volume Control test");
	g_signal_connect (volume,
			  "audio-device-selection-needed",
			  G_CALLBACK (audio_selection_needed),
			  NULL);
	gvc_mixer_control_open (volume);

	g_main_loop_run (loop);

	return 0;
}
