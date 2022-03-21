#include <obs-module.h>
#include <util/platform.h>

#define CHUNK_SIZE 4194304 // 4 MiB

#define WIDTH 640
#define HEIGHT 480
#define UYVY_FILE "/home/amaury/tmp/example.uyvy"

typedef struct zls_source_data {
    obs_source_t *source;
    obs_data_t *settings;
    pthread_t thread;
    bool initialized;
} zls_source_data_t;


// FILE *fopen(char *chemin, char *mode);

uint8_t *load_file(char* src) {
    FILE *fd = os_fopen(src, "rb");
    size_t size = os_fgetsize(fd);

    // https://github.com/obsproject/obs-studio/blob/f40eede37a791f835c6554a3beda9f0290643b18/libobs/util/platform.c#L328

    uint8_t *buffer = bmalloc(size * sizeof(uint8_t)); // TODO Taurre sizeof(char) toujours pertinent vu que c'est constant partout désormais ?

    size_t read_n = fread(buffer, sizeof(uint8_t), CHUNK_SIZE, fd);
    if (read_n != size) {
        blog(LOG_ERROR, "C'pas normal ça… Got %d != Want %d", read_n, size);
    } else {
        blog(LOG_ERROR, "trkl ;)");
    }
    fclose(fd);
    blog(LOG_ERROR, src);
    blog(LOG_ERROR, "%d", size);
    return buffer;
}

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
    blog(LOG_ERROR, "%d", 614400/(480*640));

    // https://obsproject.com/docs/reference-libobs-media-io.html
    // https://obsproject.com/docs/reference-sources.html#c.obs_source_output_video

    /*
     *
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

    uint64_t now = os_gettime_ns();

    uint8_t *frame_data = load_file(UYVY_FILE);

    struct obs_source_frame frame;
    frame.width = WIDTH;
    frame.height = HEIGHT;
    frame.format = VIDEO_FORMAT_UYVY;
    frame.data[0] = frame_data;
    frame.linesize[0] = 256;
    frame.timestamp = now;
    frame.flip = 0;
    obs_source_output_video(source, &frame);
    //bfree(frame_data);

    /*
     struct obs_source_frame {
        uint8_t             *data[MAX_AV_PLANES];
        uint32_t            linesize[MAX_AV_PLANES];
        uint32_t            width;
        uint32_t            height;
        uint64_t            timestamp;

        enum video_format   format;
        float               color_matrix[16];
        bool                full_range;
        float               color_range_min[3];
        float               color_range_max[3];
        bool                flip;
};
     */

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
