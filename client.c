#include "common.h"
#include <signal.h>

char *srv_name = "localhost";
int clt_sock = -1;

int DFLAG;

void sig_handler(int s)
{
	//msg_t msg;
	
	switch (s) {
	case SIGINT: /* Fallthrough */
		//msg.code = END_OK;
		//msg.size = 0;
		
		//send(clt_sock, &msg, HEADSIZE, 0);
		send_msg(clt_sock, END_OK, 0, NULL);
		
		close(clt_sock);
		exit(EXIT_SUCCESS);
	default:
		return;
	}
}

/* Établie une session TCP vers srv_name sur le port srv_port
 * char *srv_name: nom du serveur (peut-être une adresse IP)
 * int srv_port: port sur lequel la connexion doit être effectuée
 *
 * renvoie: descripteur vers le socket
 */ 
int connect_to_server(char *srv_name, int srv_port)
{
	struct addrinfo hints, *res;
	char service[6];
	/* Code nécessaires à la création d'une socket en
	   écoute : 
	     
	   - résolution du nom avec gethostbyname
	     
	   - appel à socket() 
	     
	   - appel à connect()
	   
	   avec les bons paramètres et contrôles d'erreurs.

	   La fonction retourne l'identifiant de la socket cliente ou -1 en cas d'erreur
	*/

	/* Get address info */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	sprintf(service, "%d", srv_port);
	getaddrinfo(srv_name, service, &hints, &res);

	/* Get socket */
	clt_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (clt_sock < 0)
		return clt_sock;


	/* Connect */
	connect(clt_sock, res->ai_addr, res->ai_addrlen);

	return clt_sock;
}

int authenticate(int clt_sock)
{
	unsigned char code;
	char login[BUFFSIZE];
  /* Code nécessaire à l'authentification auprès du serveur :

     - attendre un paquet AUTH_REQ

     - répondre avec un paquet AUTH_RESP
     
     - attendre un paquet ACCESS_OK / ACCESS_DENIED / AUTH_REQ

     - agir en conséquence ...

  */
	recv_msg(clt_sock, &code, NULL, NULL);
	if (code != AUTH_REQ) {
		 DEBUG("AUTH_REQ not received");
		return -1;
	}

	printf("Enter login: ");
	scanf("%s", login);
	send_msg(clt_sock, AUTH_RESP, strlen(login) + 1, login);

	recv_msg(clt_sock, &code, NULL, NULL);
	if (code != ACCESS_OK) {
		DEBUG("ACCESS_OK not received");
		return -1;
	}

	return 0;
}

int instant_messaging(int clt_sock)
{  
	for (;;) {
	fd_set rset;
	unsigned char code;
	unsigned char size;
	char *data;
	
	FD_ZERO(&rset);
	FD_SET(clt_sock, &rset);
	FD_SET(STDIN_FILENO, &rset);
	
	
	/* pour les étapes 2 à 4 se contenter de recevoir les messages
	envoyés par le serveur et les afficher à l'utilisateur
	*/
	
	if (select(clt_sock + 1, &rset, NULL, NULL, NULL) < 0){
		PERROR("select");
		exit(EXIT_FAILURE);
	}
	
	if (FD_ISSET(STDIN_FILENO, &rset)){
		/* l'utilisateur a tapé un nouveau message */
		DEBUG("STDIN_FILENO isset");
		data = malloc(BUFFSIZE);
		if (fgets(data, BUFFSIZE, stdin) == NULL){
			/* gérer feof et ferror */
		
			//   <COMPLÉTER>
			
			return 0;
		}
		size = strlen(data)+1;
		
		DEBUG("sending MESG %s(%d)", data, size);
		
		//  <COMPLÉTER>
		send_msg(clt_sock, MESG, size, data);
		
		free(data);
	
	}

    if (FD_ISSET(clt_sock, &rset)) {
      /* réception d'un message du serveur */
      /* expected: <code><datalen>[<data>] */

      //  <COMPLÉTER>
	recv_msg(clt_sock, &code, &size, &data);
	printf("%s\n", data);
      
    }
    
  } /* for (;;) */

  return 0;
}

int main(int argc, char *argv[])
{
  // char srv_name[BUFFSIZE];
	int clt_sock;
  int srv_port = 4444;

  DFLAG = 1;

  signal(SIGINT, sig_handler);
  
  clt_sock = connect_to_server(srv_name, srv_port);
  if (clt_sock < 0)
    exit(EXIT_FAILURE);

  if (authenticate(clt_sock) < 0){
    close(clt_sock);
    eprintf("connection closed, authentication failed\n");
    exit(EXIT_FAILURE);
  }

  if (instant_messaging(clt_sock) < 0) {
    close(clt_sock);
    eprintf("connection closed, instant messaging failed\n");
    exit(EXIT_FAILURE);
  }

  close(clt_sock);
  eprintf("connection closed, success\n");
  exit(EXIT_SUCCESS);
}
