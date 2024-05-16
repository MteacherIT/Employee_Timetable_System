#include "server.h" // defines messages between client and server

/*
 * @purpose       - function passed to created threads to be used as their routine
 * @param useCase - the use case (1-3) to execute
 * @returns       - nothing
 *
 */
void *perform_usecae(void *useCase)
{

	// get the use case and free the memory (as the pointer is not needed)
	int u_case = *((int *)useCase);

	// declare all required variables
	int coid, empID, empPass;
	server_data reply;
	client_data msg;
	struct employee e;
	struct shift s;

	// by default, no users are logged in
	msg.loggedIn = 0;

	switch (u_case)
	{
	// execute use case 1
	case USE_CASE_ONE:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		empID = 2;
		empPass = 6574;
		e.password = empPass;
		e.id = empID;

		// message to server details
		msg.type = u_case;
		msg.emp_data = e;
		msg.size = sizeof(msg);

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		// if the user is authenticated
		if (reply.auth)
		{
			printf("Use case 1: logged in\n");

			// create the shift data to schedule
			msg.loggedIn = 1;
			s.employeeID = empID;
			s.shiftDuration = 4;
			s.time.day = 30;
			s.time.month = 11;
			s.time.year = 22;
			s.time.hour = 12;
			s.time.minutes = 30;
			msg.shift_data = s;
			msg.size = sizeof(msg);

			// this is the last message sent by this thread, disconnect
			msg.pulse.code = _PULSE_CODE_DISCONNECT;

			/* send a get message to book a shift */
			MsgSend(coid, &msg, sizeof(msg) + sizeof(s.time), &reply, sizeof(reply));

			// check if the shift was booked
			if (reply.success)
			{
				printf("Use case 1: the shift has been booked!\n");
			}
			else
			{
				printf("Use case 1: could not book shift!\n");
			}
		}
		else
		{
			printf("Use case 1: unable to login, access denied\n");
		}
		break;
	// execute use case 2
	case USE_CASE_TWO:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		empID = 6;
		empPass = 7564;
		e.password = empPass;
		e.id = empID;

		// message to server details
		msg.type = u_case;
		msg.emp_data = e;
		msg.size = sizeof(msg);
		msg.shiftsReceived = 0;

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		// if login successful
		if (reply.auth)
		{
			printf("Use case 2: logged in\n");
			msg.loggedIn = 1;
			msg.size = sizeof(msg);
			// This infinite loop repeatedly gets query's the server for the user's shifts
			// then prints the data. Once all shifts have been shared, it exits
			while (1)
			{
				/* send a get message to a shift for the given employee */
				MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

				// if all shifts have been gotten, exit the loop
				if (!reply.moreDataToSend)
				{
					break;
				}

				// print the shift data received from the server
				printf("Use case 2: received shift %02d/%02d/%02d @ %02d:%02d for employee %d\n", reply.shift_data.time.day,
							 reply.shift_data.time.month, reply.shift_data.time.year, reply.shift_data.time.hour,
							 reply.shift_data.time.minutes, empID);

				// increment the number of shifts received
				msg.shiftsReceived++;
			}
		}
		else
		{
			printf("Use case 2: unable to login, access denied\n");
		}

		// this is the last message sent by this thread, disconnect
		msg.pulse.code = _PULSE_CODE_DISCONNECT;
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		break;

	case USE_CASE_THREE:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		empID = 0;
		empPass = 2222;
		e.password = empPass;
		e.id = empID;
		e.isSupervisor = 1;
		msg.type = u_case;
		msg.emp_data = e;
		msg.size = sizeof(msg);
		msg.shiftsReceived = 0;

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		// if login successful
		if (reply.auth)
		{
			printf("Use case 3: logged in\n");
			msg.loggedIn = 1;
			msg.size = sizeof(msg);

			// This infinite loop repeatedly gets query's the server for all shifts
			// then prints the data. Once all shifts have been shared, it exits
			while (1)
			{
				/* send a get message to a shift for the given employee */
				MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

				// if all shifts have been gotten, exit the loop
				if (!reply.moreDataToSend)
				{
					break;
				}
				// print the shift data received from the server
				printf("Use case 3: received shift %02d/%02d/%02d @ %02d:%02d for employee %d\n", reply.shift_data.time.day,
							 reply.shift_data.time.month, reply.shift_data.time.year, reply.shift_data.time.hour,
							 reply.shift_data.time.minutes, reply.shift_data.employeeID);
				// increment the number of shifts received
				msg.shiftsReceived++;
			}
		}
		else
		{

			printf("Use case 2: unable to login, access denied\n");
		}

		// this is the last message sent by this thread, disconnect
		msg.pulse.code = _PULSE_CODE_DISCONNECT;
		/* send a get message to a shift for the given employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		break;
	case USE_CASE_EIGHT:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		empID = 2;
		empPass = 6574;
		e.password = empPass;
		e.id = empID;
		e.isSupervisor = 0;
		msg.type = u_case;
		msg.emp_data = e;
		msg.size = sizeof(msg);
		msg.shiftsReceived = 0;

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));
		// This infinite loop repeatedly gets query's the server for all shifts
		// then prints the data. Once all shifts have been shared, it exits
		if (reply.auth)
		{
			printf("Use case 8: logged in\n");
			msg.loggedIn = 1;
			msg.size = sizeof(msg);
			MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));
			// This infinite loop repeatedly gets query's the server for all shifts
			// then prints the data. Once all shifts have been shared, it exits
			while (1)
			{
				// msg recieved is unsuccessful if the employee has worked more than 40 hours
				if (reply.success == 0)
				{
					printf("Use case 8: Employee has worked more than 40 hours!\n");
					break;
				}
			}
		}
		else
		{

			printf("Use case 8: unable to login, access denied\n");
		}
		break;
	case USE_CASE_NINE:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		// employee variables
		empID = 0;
		empPass = 2222;
		e.password = empPass;
		e.id = empID;
		e.isSupervisor = 1;
		msg.type = u_case;
		msg.emp_data = e;
		msg.size = sizeof(msg);
		msg.shiftsReceived = 0;

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));
		// This infinite loop repeatedly gets query's the server for all shifts
		// then prints the data. Once all shifts have been shared, it exits
		if (reply.auth)
		{
			printf("Use case 9: logged in\n");
			msg.loggedIn = 1;
			msg.size = sizeof(msg);
			MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));
			while (1)
			{
				// msg recieved is unsuccessful if the employee has worked more than 40 hours
				if (reply.success == 1)
				{
					printf("Use case 9: Supervisor has filled all schedule gaps!\n");
					break;
				}
			}
		}
		else
		{

			printf("Use case 9: unable to login, access denied\n");
		}
		break;
	default:
		break;
	}
	return EOK;
}

int main(int argc, char **argv)
{
	// create a menu for the user to select a use case to run
	int useCases[] = {USE_CASE_ONE, USE_CASE_TWO, USE_CASE_THREE, USE_CASE_EIGHT, USE_CASE_NINE};
	int choice = 0;
	while (choice != 6)
	{
		printf("BGH Use Cases: Please select a use case to run:\n");
		printf("1. Use Case 1: Employee logs in and selecting a shift for themself\n");
		printf("2. Use Case 2: Employee logs in and check their own schedule\n");
		printf("3. Use Case 3: Supervisor logs in and check the schedule for the whole workforce\n");
		printf("4. Use Case 8: Employee try to get a schedule while reaching the maximum number of hours a week.\n");
		printf("5. Use Case 9: Supervisor logs in and fills schedule gaps\n");
		printf("6. Exit\n");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			perform_usecae((void *)&useCases[0]);
			break;
		case 2:
			perform_usecae((void *)&useCases[1]);
			break;
		case 3:
			perform_usecae((void *)&useCases[2]);
			break;
		case 4:
			perform_usecae((void *)&useCases[3]);
			break;
		case 5:
			perform_usecae((void *)&useCases[4]);
			break;
		case 6:
			break;
		default:
			printf("BGH Use Cases: Invalid choice, please try again\n");
			break;
		}
	}

	return EXIT_SUCCESS;
}
