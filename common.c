#include "common.h"

/* send_msg send a message on socket sock
   sock: the socket
   code: message's protocol code
   size: message's size
   msg: message to be sent
*/
int send_msg(int sock, unsigned char code, unsigned char size, char *body) 
{
	int error;
  msg_t msg = {
		.code = code,
		.size = size,
	};

  /* Code nécessaire à envoyer le message correspondant au protocle
     sur la socket
  */

	if ((error = send(sock, &msg, HEADSIZE, 0)) < 0) {
		PERROR("Sending header failed");
		return error;
	}

	if ((error = send(sock, &body, msg.size, 0)) < 0) {
		PERROR("Sending body failed");
		return error;
	}

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
  msg_t msg;
	//ssize_t n;

	recv(sock, &msg, HEADSIZE, 0);
	*code = msg.code;

	if (!body || !size) {
		return 0;
	}

	*size = msg.size;
	recv(sock, &body, msg.size, 0);
  return 0;
}
