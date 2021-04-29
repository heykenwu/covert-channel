#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include "header/file_monitor.h"

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void monitor(const char *list[][2], int len, inotify_callback c)
{
    int length, i = 0;
    int fd, wd, count;
    char buffer[EVENT_BUF_LEN];

    fd_set rfds;
    FD_ZERO(&rfds);
    fd = inotify_init();
    FD_SET(fd, &rfds);
    if (fd < 0)
        perror("inotify_init");

    const char *wd_map[len][2];
    for (int i = 0; i < len; i++)
    {
        if (access(list[i][0], F_OK) == 0)
        {
            wd = inotify_add_watch(fd, list[i][0], IN_CREATE);
            wd_map[wd-1][0] = list[i][0];
            wd_map[wd-1][1] = list[i][1];
        }
    }

    while (1)
    {
        i = 0;
        length = read(fd, buffer, EVENT_BUF_LEN);

        /*checking for error*/
        if (length < 0)
            perror("read");

        /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
                if (event->mask & IN_CREATE && !(event->mask & IN_ISDIR))
                {
                    if (strcmp(event->name, wd_map[(event->wd) - 1][1]) == 0)
                    {
                        char buf[50];
                        strcpy(buf, wd_map[(event->wd) - 1][0]);
                        strcat(buf, "/");
                        strcat(buf, event->name);
                        c(buf);
                    }
                }
            i += EVENT_SIZE + event->len;
        }
    }
    inotify_rm_watch(fd, wd);

    close(fd);
}