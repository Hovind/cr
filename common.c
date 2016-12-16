#include "common.h"

/* send_msg send a message on socket sock
   sock: the socket
   code: message's protocol code
   size: message's size
   msg: message to be sent
*/
int send_msg(int sock, unsigned char code, unsigned char size, char *body) 
{
  // msg_t msg;
  
  // msg.code = code;
  // msg.size = size;

  /* Code nécessaire à envoyer le message correspondant au protocle
     sur la socket
  */

  return 0;
}

/* recv_msg recv a message from the socket sock
   sock: the socket
   code: message's protocol code
   size: message's size
   msg: message to be received
*/
int recv_msg(int sock, unsigned char *code, unsigned char *size, char **body) 
{
  // msg_t msg;
  
  return 0;
}
