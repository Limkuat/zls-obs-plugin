#include <obs-module.h>
#include <util/platform.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct zls_source_data {
    obs_source_t *source;
    obs_data_t *settings;
    pthread_t thread;
    bool initialized;
} zls_source_data;

static const char *zls_source_name(void *type_data) {
    UNUSED_PARAMETER(type_data);

    return obs_module_text("ZLSSource");
}

static void zls_source_defaults(obs_data_t *settings)
{
    blog(LOG_ERROR, "Defaults?");
}

static bool setting_root_path_modified(obs_properties_t *props, obs_property_t *p, obs_data_t *settings) {
    bool enabled = obs_data_get_bool(settings, "setting_a");
    p = obs_properties_get(props, "setting_b");
    obs_property_set_enabled(p, enabled);

    return true;
}

static obs_properties_t *zls_source_properties(void *data) {
    blog(LOG_ERROR, "Properties?");

    obs_properties_t *props = obs_properties_create();
    obs_properties_add_path(props, "zls_root_path", obs_module_text("ZLSRootPath"), OBS_PATH_DIRECTORY, NULL, NULL);

    UNUSED_PARAMETER(data);
    return props;
}


static void zls_source_destroy(void *data){
    bfree(data);
}

static void *zls_source_create(obs_data_t *settings, obs_source_t *source) {
    blog(LOG_ERROR, "HERE create_source");

    uint64_t now = os_gettime_ns() + 1e9;
    uint8_t frame_bitstream[MAX_AV_PLANES];

    // https://obsproject.com/docs/reference-libobs-media-io.html
    // https://obsproject.com/docs/reference-sources.html#c.obs_source_output_video

    struct obs_source_frame frame;
    frame.width = WIDTH;
    frame.height = HEIGHT;
    frame.format = VIDEO_FORMAT_UYVY;
    frame.data[0] = frame_bitstream;  // TODO: lolwut? Taurre, plz help
    frame.timestamp = now;
    obs_source_output_video(source, &frame);

    struct zls_source_data *zls_data = bmalloc(sizeof(struct zls_source_data));
    zls_data->source = source;
    zls_data->settings = settings;
    zls_data->initialized = true;
    return (void*)zls_data;  // TODO Demander à Taurre si c'est une best practice de caster explicitement
}

struct obs_source_info zls_source = {
        .id             = "zls_source",
        .type           = OBS_SOURCE_TYPE_INPUT,
        .output_flags   = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO,
        .get_name       = zls_source_name,
        .icon_type      = OBS_ICON_TYPE_CAMERA,
        .get_defaults   = zls_source_defaults,
        .get_properties = zls_source_properties,
        .create         = zls_source_create,
        .destroy        = zls_source_destroy
};


/*
 * struct obs_source_frame frame;
			frame.width = video_format.width;
			frame.height = video_format.height;
			frame.format = VIDEO_FORMAT_UYVY;
			frame.linesize[0] = video_format.stride;
			frame.data[0] = video_frame.data + video_offset;
			frame.timestamp = cur_time;
			frame.flip = 0;
			frame.full_range = rt->range == VIDEO_RANGE_FULL;
			video_format_get_parameters(rt->space, rt->range,
			frame.color_matrix, frame.color_range_min, frame.color_range_max);

 */
