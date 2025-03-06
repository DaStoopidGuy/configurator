#pragma once
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#define STINKY_IMPLEMENTATION
#include "stinky.h"

#define FATAL(msg, ...) do { \
    printf("FATAL: " msg " \n", ##__VA_ARGS__); \
    abort(); \
} while(0) \

typedef struct {
    String key;
    String val;
} ConfigEntry;

typedef struct {
    ConfigEntry* entries;
    size_t len;
    size_t alloc;
} Config;

Config config_open(const char* filename);
void config_close(Config *c);
const char *config_get(Config *c, const char* key);

#ifdef CONFIG_IMPLEMENTATION
Config config_open(const char* filename) {
    Config c = {0};

    // try to open the file
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        FATAL("File (%s) could not be opened", filename);
    }

    // find size of file
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // allocate buffer of required size
    char *buffer = malloc(filesize);

    // read contents of file into buffer
    if (buffer) {
        fread(buffer, 1, filesize, f);
    }

    { // parsing stuff
        StringView text = sv_trim(sv_from_parts(buffer, filesize));
        while (text.len > 0) {
            StringView key = sv_trim(sv_chop_by_delim(&text, '='));
            StringView val = sv_trim(sv_chop_by_delim(&text, '\n'));

            // append to ConfigEntries
            {
                ConfigEntry entry = {
                    .key = str_from_parts(key.data, key.len),
                    .val = str_from_parts(val.data, val.len),
                };

                if (c.len >= c.alloc) {
                    c.alloc = c.alloc==0 ? 2 : c.alloc*2;
                    c.entries = realloc(c.entries, c.alloc * sizeof(*c.entries));
                    assert(c.entries != NULL);
                }
                c.entries[c.len++] = entry;
            }
        }
    }

    free(buffer);
    fclose(f);
    return c;
}

void config_close(Config *c) {
    // clean up
    for (size_t i=0; i<c->len; i++) {
        ConfigEntry entry = c->entries[i];
        str_free(entry.key);
        str_free(entry.val);
    }

    free(c->entries);
    c->entries = NULL;
}

const char *config_get(Config *c, const char* key) {
    for(size_t i = 0; i < c->len; i++) {
        const ConfigEntry entry = c->entries[i];

        if (sv_equals(sv_from_cstr(key), sv_from_parts(entry.key.data, entry.key.len))) {
            return entry.val.data;
        }
    }
    return NULL;
}
#endif //CONFIG_IMPLEMENTATION
