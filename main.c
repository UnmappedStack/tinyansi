#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>

#define IS_VALID_ESCAPE(s) ((s[0] == '\x1b' || s[0] == 27) || s[1] == '[')
#define MAX_ANSI_VALS 27
typedef struct {
    char cmd;
    size_t vals[MAX_ANSI_VALS];
    size_t nvals;
} EscapeCode;

int parse_ansi(const char *s, EscapeCode *buf) {
    if (!IS_VALID_ESCAPE(s)) return -1;
    size_t len = strlen(s);
    buf->cmd = s[len - 1];
    buf->nvals = 0;
    for (s+=2;isdigit(*s) || *s == buf->cmd;) {
        char strbuf[4];
        size_t i = 0;
        for (;isdigit(s[i]); i++);
        memcpy(strbuf, s, i), strbuf[i] = 0;
        buf->vals[buf->nvals++] = strtol(strbuf, NULL, 10);
        s+=i+1;
    }
    return 0;
}

int main(void) {
    EscapeCode code;
    if (parse_ansi("\x1b[12;13H", &code) < 0) {
        fprintf(stderr, "Failed to parse ANSI.\n");
        return -1;
    }
    printf("Command is %c, vals:\n", code.cmd);
    for (size_t i = 0; i < code.nvals; i++) {
        printf("[%zu] %zu\n", i, code.vals[i]);
    }
    return 0;
}
