#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define SV_IMPLEMENTATION
#include "string_view.h"

#define FATAL(msg, ...) do { \
    printf("FATAL: " msg " \n", ##__VA_ARGS__); \
    abort(); \
} while(0) \

// String
typedef struct {
    char *data;
    size_t len;
    size_t alloc;
} String;

String str_from_parts(const char* data, size_t len) {
    String s = {0};
    s.len = len;
    s.alloc = len + 1;
    s.data = malloc(s.alloc);

    memcpy(s.data, data, len);
    s.data[s.len] = '\0'; // append null terminator

    return s;
}

String str_from_cstr(const char *cstr) {
    return str_from_parts(cstr, strlen(cstr));
}

void str_free(String s) {
    free(s.data);
    s.data = NULL;
    s.len = 0;
    s.alloc = 0;
}

// Config stuff
typedef struct {
    String key;
    String val;
} ConfigEntry;

typedef struct {
    ConfigEntry* entries;
    size_t len;
    size_t alloc;
} Config;

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

        if (memcmp(key, entry.key.data, strlen(key))) {
            return entry.val.data;
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    printf("Configurator testing lol\n");

    char *filename = "test.cfg";
    if (argc > 1)
    {
        filename = argv[1];
        printf("1st argument: %s\n", argv[1]);
    }

    Config config = config_open(filename);


    const char *key = "pp";
    const char *val = config_get(&config, key);
    printf("[%s] -> \"%s\"\n", key, val);

    config_close(&config);
    return 0;
}
