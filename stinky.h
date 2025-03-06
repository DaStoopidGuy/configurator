/*
 * You need:
 * #define STINKY_IMPLEMENTATION
 * before #include(-ing) this header
 *
 * This header-only "library" includes two things:
 * 1. String(s)
 *      which are allocated with their own memory.
 *      and are required to be freed after use
 *
 * ------
 * 2. StringView(s)
 *      which are like "slices"
 *      only point to memory which is already allocated
 *      and therefore, do not need to be freed.
 *      just do stuff with 'em and forget about them
 */
#pragma once
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Allocated strings
typedef struct {
    char *data;
    size_t len;
    size_t alloc;
} String;

String str_from_parts(const char* data, size_t len);
String str_from_cstr(const char *cstr);
void str_free(String s);

// String view
#define SV_Fmt "%.*s"
#define SV_Arg(s) (int) (s).len, (s).data

typedef struct {
    const char *data;
    size_t len;
} StringView;

StringView sv_from_parts(const char *data, size_t len);
StringView sv_from_cstr(const char *cstr);

StringView sv_trim_left(StringView s);
StringView sv_trim_right(StringView s);
StringView sv_trim(StringView s);

StringView sv_chop_by_delim(StringView *s, char delim);
bool sv_equals(StringView a, StringView b);

#ifdef STINKY_IMPLEMENTATION
// String implemntation
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

// String view implementation
StringView sv_from_parts(const char *data, size_t len) {
    StringView s = {
        .data = data,
        .len = len,
    };

    return s;
}

StringView sv_from_cstr(const char *cstr) {
    return sv_from_parts(cstr, strlen(cstr));
}

// trim all whitespace from left side and return trimmed stink
StringView sv_trim_left(StringView s) {
    size_t i = 0;

    // find first non-whitespace character
    while (i < s.len && isspace(s.data[i]))
    {
        i++;
    }

    return sv_from_parts(s.data + i, s.len - i);
}

// trim all whitespace from right side and return trimmed stink
StringView sv_trim_right(StringView s) {
    size_t i = 0;

    // find last non-whitespace character
    while (i < s.len && isspace(s.data[s.len - i - 1])) {
        i++;
    }

    return sv_from_parts(s.data, s.len - i);
}

// trim all surrounding whitespace
StringView sv_trim(StringView s) {
    return sv_trim_right(sv_trim_left(s));
}

StringView sv_chop_by_delim(StringView *s, char delim) {
    // find delimiter
    size_t i = 0;
    while (i < s->len && s->data[i] != delim) {
        i++;
    }

    StringView result = sv_from_parts(s->data, i);

    if (i < s->len) {
        s->data += i + 1;
        s->len  -= i + 1;
    } else {
        s->data += i;
        s->len -= i;
    }

    return result;
}

bool sv_equals(StringView a, StringView b) {
    if (a.len != b.len) 
        return false;
    else
        return memcmp(a.data, b.data, a.len) == 0;
}
#endif // SV_IMPLEMENTATION
