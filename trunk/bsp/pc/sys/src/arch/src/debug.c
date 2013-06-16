#include <ace/stdarg.h>
#include <arch/debug.h>

static int x = 0;
static int y = 0;

static char* video = (char*) 0xb8000;

static void putc(char c)
{
    if ((c == '\n') || (x > 79)) {
        x = 0;
        y++;
    }

    if (c == '\n') {
        return;
    }

    if (y > 24) {
        int i;
        for (i = 0; i < 2 * 24 * 80; i++) {
            video[i] = video[i + 160];
        }

        for (; i < 2 * 25 * 80; i++) {
            video[i] = 0;
        }
        y--;
    }

    video[2 * (y * 80 + x)] = c;
    video[2 * (y * 80 + x) + 1] = 0x07;

    x++;
}

void PC_dbg_clr(void)
{
    int i;
    for (i = 0; i < 2 * 25 * 80; i++) {
        video[i] = 0;
    }

    x = y = 0;
}

void PC_dbg_puts(const char* s)
{
    while (*s) {
        putc(*s++);
    }
}
