#pragma once
#include <stdlib.h>
#include <string.h>

// Allocated strings
typedef struct {
    char *data;
    size_t len;
    size_t alloc;
} StringBuilder;

StringBuilder *sb_new();
StringBuilder *sb_from_size(size_t size);
StringBuilder *sb_from_parts(const char* data, size_t len);
StringBuilder *sb_from_cstr(const char *cstr);
void sb_free(StringBuilder *s);

void sb_append_cstr(StringBuilder *s, const char *cstr);
const char *sb_get_cstr(StringBuilder *s);

#ifdef SB_IMPL
// String implemntation

StringBuilder *sb_new() {
    StringBuilder *s = malloc(sizeof(StringBuilder));
    s->data = NULL;
    s->len = 0;
    s->alloc = 16; // default allocation size
    s->data = malloc(s->alloc);

    return s;
}

StringBuilder *sb_from_size(size_t size) {
    StringBuilder *s = malloc(sizeof(StringBuilder));
    s->data = NULL;
    s->len = 0;
    s->alloc = size;
    s->data = malloc(s->alloc);

    return s;
}

StringBuilder *sb_from_parts(const char* data, size_t len) {
    StringBuilder *s = sb_from_size(len);

    memcpy(s->data, data, len);
    s->len = len;

    return s;
}

StringBuilder *sb_from_cstr(const char *cstr) {
    return sb_from_parts(cstr, strlen(cstr));
}

void sb_free(StringBuilder *s) {
    free(s->data);
    s->data = NULL;
    s->len = 0;
    s->alloc = 0;

    free(s);
}

void sb_realloc_if_required(StringBuilder *s, size_t required_size) {
    while (s->alloc < required_size) {
        if (s->alloc == 0) exit(1);

        s->alloc = s->alloc * 2;
        s->data = realloc(s->data, s->alloc);
    }
}

void sb_append_cstr(StringBuilder *s, const char *cstr) {
    size_t cstr_len = strlen(cstr);
    size_t required_len = s->len + cstr_len;
    sb_realloc_if_required(s, required_len);
    
    memcpy(s->data + s->len, cstr, cstr_len);
}

const char *sb_get_cstr(StringBuilder *s) {
    // make sure we have space for null-term
    sb_realloc_if_required(s, s->len + 1);

    if (s->data[s->len] != '\0') {
        // append null terminator
        s->data[s->len] = '\0';
    }

    return s->data;
}
#endif // SB_IMPL
