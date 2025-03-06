#include <stdio.h>
#include <assert.h>
#define CONFIG_IMPLEMENTATION
#include "configurator.h"

int main(int argc, char **argv) {
    printf("Configurator testing lol\n");

    char *filename = "test.cfg";
    if (argc > 1)
    {
        filename = argv[1];
        printf("1st argument: %s\n", argv[1]);
    }

    Config config = config_open(filename);


    const char *key = "age";
    const char *val = config_get(&config, key);
    printf("[%s] -> \"%s\"\n", key, val);

    config_close(&config);
    return 0;
}
