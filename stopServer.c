#include "server.h"


int main(int argc, char **argv) {


	//declare all required variables
	int coid;
	client_data msg;


	/* find our server to get a coid*/
	coid = name_open(SERVER_NAME, 0);

	//tell server to exit
	msg.type       = TERM_SERVER;
	msg.pulse.code = _PULSE_CODE_DISCONNECT;

	/* send a get message to terminate the server */
	MsgSend(coid, &msg, sizeof(msg), NULL, 0);


	return 0;
}
