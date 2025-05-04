#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define CONFIG_IMPL
#define SV_IMPL
#define SB_IMPL
#include "configurator.h"

void print_config(Config *config, const char* key) {
    const char *val = config_get(config, key);
    printf("[%s] -> \"%s\"\n", key, val);
}

int main(int argc, char **argv) {
    printf("Configurator testing lol\n");

    const char *home = getenv("HOME");
    if (home == NULL) exit(1);

    StringBuilder *filepath = sb_from_cstr(home);
    char *filename = "/.config/bingchillin.conf";

    if (argc > 1)
    {
        filename = argv[1];
        printf("1st argument: %s\n", argv[1]);
    }

    sb_append_cstr(filepath, filename);

    printf("%s\n", sb_get_cstr(filepath));

    Config config = config_open(sb_get_cstr(filepath));
    print_config(&config, "name");
    print_config(&config, "age");

    config_close(&config);

    sb_free(filepath);
    return 0;
}
