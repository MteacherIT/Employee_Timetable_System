#include "server.h" // defines messages between client and server

/*
 * @purpose       - function passed to created threads to be used as their routine
 * @param useCase - the use case (6,7) to execute
 * @returns       - nothing
 *
 */

#define MAX_STRING_LEN    256

void *perform_usecae(void *useCase){

	// get the use case and free the memory (as the pointer is not needed)
	int u_case = *((int *)useCase);
	free(useCase);

	// declare all required variables 
	int coid, superID, superPass, empID;
	server_data reply;
	client_data msg;
	struct employee super;
	struct employee e;

	// by default, no users are logged in
	msg.loggedIn = 0;

	switch (u_case)
	{
	// execute use case 6: supervisor adds a new employee to the system
	case USE_CASE_SIX:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		superID = 0;
		superPass = 2222;
		super.password = superPass;
		super.id = superID;

		// message to server details
		msg.type = u_case;
		msg.emp_data = super;
		msg.size = sizeof(msg);

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		// if the user is authenticated
		if (reply.auth)
		{
			printf("Use case 6: add new employee\n");

			// add the employee to the system
			msg.loggedIn = 1;
			
			empID = 2;
			e.id = empID;
			e.password = 9999;
			e.name = EMP_TO_ADD;
			e.dob = 1990;

			msg.size = sizeof(msg);

			// this is the last message sent by this thread, disconnect
			msg.pulse.code = _PULSE_CODE_DISCONNECT;

			/* send a get message to book a shift */
			MsgSend(coid, &msg, sizeof(msg) + sizeof(e), &reply, sizeof(reply));

			// check if the employee was added
			if (reply.success)
			{
				printf("Use case 6: the new employee was added!\n");
				//increment number of employees
			}
			else
			{
				printf("Use case 6: could not add new employee!\n");
			}
		}
		break;
	// execute use case 7
	case USE_CASE_SEVEN:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		superID = 0;
		superPass = 2222;
		super.password = superPass;
		super.id = superID;


		// message to server details
		msg.type = u_case;
		msg.emp_data = super;
		msg.size = sizeof(msg);
		msg.shiftsReceived = 0;

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		// if login successful
		if (reply.auth)
		{
			printf("Use case 7: remove employee\n");

			// add the employee to the system
			msg.loggedIn = 1;
			
			empID = 2;
			e.id = empID;

			msg.size = sizeof(msg);

			// this is the last message sent by this thread, disconnect
			msg.pulse.code = _PULSE_CODE_DISCONNECT;

			/* send a get message to book a shift */
			MsgSend(coid, &msg, sizeof(msg) + sizeof(e), &reply, sizeof(reply));

			// check if the employee was added
			if (reply.success)
			{
				printf("Use case 7: the employee was removed!\n");
				//decrement number of employees
			}
			else
			{
				printf("Use case 7: could not remove employee!\n");
			}
		}
		// this is the last message sent by this thread, disconnect
		msg.pulse.code = _PULSE_CODE_DISCONNECT;
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		break;

	default:
		break;
	}
	return EOK;
}

int main(int argc, char **argv)
{
	// define the use cases
	int useCases[] = {USE_CASE_SIX, USE_CASE_SEVEN};
	int n = sizeof(useCases) / (sizeof(useCases[0]));
	// define an array of threads
	pthread_t myThreads[n];

	// create a thread for each use case and provide the correct routine
	for (int i = 0; i < n; ++i)
	{
		int *curr_useCase = (int *)malloc(sizeof(useCases[i]));
		if (curr_useCase == NULL)
		{
			printf("Use Cases 6-7: Malloc Error!\n");
			return -1;
		}
		*curr_useCase = useCases[i];

		int ret = pthread_create(&myThreads[i], NULL, perform_usecae, curr_useCase);
		if (ret != 0)
		{
			printf("Use Cases 6-7: error: Cannot create thread # %d\n", i);
			return -1;
		}
	}

	// wait for the threads to terminate
	for (int i = 0; i < n; ++i)
	{
		pthread_join(myThreads[i], NULL);
	}

	return EXIT_SUCCESS;
}
