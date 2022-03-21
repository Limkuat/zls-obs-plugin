#include <obs-module.h>

/* Defines common functions (required) */
OBS_DECLARE_MODULE()

/* Implements common ini-based locale (optional) */
OBS_MODULE_USE_DEFAULT_LOCALE("zls-plugin", "en-US")

extern struct obs_source_info  zls_source;

bool obs_module_load(void) {
    blog(LOG_INFO, "ZLS plugin loaded.");
    obs_register_source(&zls_source);
    return true;
}
