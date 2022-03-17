#include <obs-module.h>

/* Defines common functions (required) */
OBS_DECLARE_MODULE()

/* Implements common ini-based locale (optional) */
OBS_MODULE_USE_DEFAULT_LOCALE("zls-plugin", "en-US")

bool obs_module_load(void)
{
    blog(LOG_INFO, "ZLS plugin loaded.");
    return true;
}
