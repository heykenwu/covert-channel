#ifndef FILE_MONITOR_H
#define FILE_MONITOR_H

typedef void (*inotify_callback)(char *filename);
void monitor(const char *list[][2], int len, inotify_callback c);

#endif