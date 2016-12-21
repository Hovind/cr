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

	printf("Sent: %d\n", code);

	if (!body) {
		return 0;
	}

	if ((error = send(sock, body, msg.size, 0)) < 0) {
		PERROR("Sending body failed");
		return error;
	}

	printf("%s\n", body);

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

	recv(sock, &msg, HEADSIZE, 0);
	*code = msg.code;
	
	printf("Recvt %d, %d\n", *code, msg.size);

	if (!body || !size) {
		return 0;
	}

	*size = msg.size;
	printf("before: %s\n", *body);
	int n = recv(sock, *body, msg.size, 0);

	printf("n: %d\n", n);
	PERROR("recv");
	printf("%s\n", *body);
	return 0;
}
