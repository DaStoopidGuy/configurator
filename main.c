#include <ctype.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FATAL(msg, ...) do { \
    printf("FATAL: " msg " \n", ##__VA_ARGS__); \
    abort(); \
} while(0) \

// String stuffs
#define Stink_Fmt "%.*s"
#define Stink_Arg(s) (int) (s).len, (s).data

typedef struct {
    char *data;
    size_t len;
} Stink;

Stink stink_from_parts(char *data, size_t len)
{
    Stink s = {
        .data = data,
        .len = len,
    };

    return s;
}

Stink stink_from_cstr(char *cstr)
{
    return stink_from_parts(cstr, strlen(cstr));
}

// trim all whitespace from left side and return trimmed stink
Stink stink_trim_left(Stink s) {
    size_t i = 0;

    // find first non-whitespace character
    while (i < s.len && isspace(s.data[i]))
    {
        i++;
    }

    return stink_from_parts(s.data + i, s.len - i);
}

// trim all whitespace from right side and return trimmed stink
Stink stink_trim_right(Stink s) {
    size_t i = 0;

    // find last non-whitespace character
    while (i < s.len && isspace(s.data[s.len - i - 1])) {
        i++;
    }

    return stink_from_parts(s.data, s.len - i);
}

// trim all surrounding whitespace
Stink stink_trim(Stink s) {
    return stink_trim_right(stink_trim_left(s));
}

Stink stink_chop_by_delim(Stink *s, char delim) {
    // find delimiter
    size_t i = 0;
    while (i < s->len && s->data[i] != delim) {
        i++;
    }

    Stink result = stink_from_parts(s->data, i);

    if (i < s->len) {
        s->data += i + 1;
        s->len  -= i + 1;
    } else {
        s->data += i;
        s->len -= i;
    }

    return result;
}

// Config stuff
typedef struct {
    Stink key;
    Stink val;
} ConfigEntry;

typedef struct {
    ConfigEntry* entries;
    size_t len;
    size_t alloc;

    FILE *file;
    char *buffer;
} Config;

Config config_from(const char* filename) {
    Config c = {0};

    // try to open the file
    c.file = fopen(filename, "r");
    if (c.file == NULL) {
        FATAL("File (%s) could not be opened", filename);
    }

    // find size of file
    fseek(c.file, 0, SEEK_END);
    long filesize = ftell(c.file);
    fseek(c.file, 0, SEEK_SET);

    // allocate buffer of required size
    c.buffer = malloc(filesize);

    // read contents of file into buffer
    if (c.buffer) {
        fread(c.buffer, 1, filesize, c.file);
    }

    { // parsing stuff
        Stink text = stink_trim(stink_from_parts(c.buffer, filesize));
        while (text.len > 0) {
            Stink key = stink_trim(stink_chop_by_delim(&text, '='));
            Stink val = stink_trim(stink_chop_by_delim(&text, '\n'));

            // append to ConfigEntries
            {
                ConfigEntry entry = {
                    .key = key,
                    .val = val,
                };

                if (c.len >= c.alloc) {
                    c.alloc = c.alloc==0 ? 2 : c.alloc*2;
                    c.entries = realloc(c.entries, c.alloc * sizeof(*c.entries));
                    assert(c.entries != NULL);
                }
                c.entries[c.len++] = entry;

                printf("New entry: "Stink_Fmt" = "Stink_Fmt"\n", Stink_Arg(entry.key), Stink_Arg(entry.val));
            }
        }
    }

    return c;
}

void config_close(Config *c) {
    // clean up
    free(c->buffer);
    c->buffer = NULL;
    fclose(c->file);
    c->file = NULL;
    free(c->entries);
    c->entries = NULL;
}

int main(int argc, char **argv) {
    printf("Configurator testing lol\n");

    char *filename = "test.cfg";
    if (argc > 1)
    {
        filename = argv[1];
        printf("1st argument: %s\n", argv[1]);
    }

    Config config = config_from(filename);

    config_close(&config);
    return 0;
}
