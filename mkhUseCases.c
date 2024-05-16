#include "server.h" // defines messages between client and server

/*
 * @purpose       - function passed to created threads to be used as their routine
 * @param useCase - the use case (4-5a-5b) to execute
 * @returns       - nothing
 *
 */
#define MAX_STRING_LEN    256

void *perform_usecae(void *useCase){

	//get the use case and free the memory (as the pointer is not needed)
	int u_case = *((int*) useCase);
	free(useCase);

	//declare all required variables
	int coid, empIDA, empPassA, empIDB, empPassB, empIDC, empPassC, empIDD, empPassD,empIDE, empPassE;
	server_data reply;
	client_data msg;

	struct employee a;
	struct employee b;
	struct employee c;
	struct employee d;
	struct employee e;
	struct employee h;
	struct shift s1;
	struct shift s2;
	struct shift s3;
	struct shift s4;


	//by default, no users are logged in
	msg.loggedIn = 0;



	switch(u_case){
	//execute use case 4 :Employee timeout while selecting a shift
	case USE_CASE_FOUR:


		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		//employee e variables
		empIDE = 5;
		empPassE = 2545;
		e.id         = empIDE;
		e.password   = empPassE;
		e.seniority = 123;
		e.workedHours = 984;
		e.isSupervisor = 0;

		//message to server details
		msg.type     = u_case;
		msg.emp_data = e;
		msg.size     = sizeof(msg);

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		if(reply.auth){
			printf("Use case 4: logged in\n");

			sleep(SESSION_LEN);
			printf("Use case 4: Session terminated by server after 60s(for testing purposes), it should be 15 mins from employee login \n");

		}else{
			printf("Use case 4: unable to login, access denied\n");
		}


		break;
	//execute use case 5A : Prioritised Employee Shift Selection: two employees of different seniority level reach for same shift
	case USE_CASE_FIVE_A:
		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);


		//employee a variables
			empIDA   = 0;
			empPassA = 2222;
			a.password   = empPassA;
			a.id         = empIDA;
			a.seniority = 8;
			a.workedHours = 64;
			a.isSupervisor = 1;




			//employee c variables
			empIDC   = 2 ;
			empPassC = 6574;
			c.password   = empPassC;
			c.id         = empIDC;
			c.seniority = 19;
			c.workedHours = 152;
			c.isSupervisor = 0;

			//employee d variables
			empIDD   = 4 ;
			empPassD = 2968;
			d.password   = empPassD;
			d.id         = empIDD;
			d.seniority = 2;
			d.workedHours = 16;
			d.isSupervisor = 0;

			//message to server details
			msg.type     = u_case;
			msg.emp_d[0] = a;
			msg.emp_d[2] = c;
			msg.emp_d[3] = d;
			msg.size     = sizeof(msg);

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		//if the user is authenticated
		if(reply.auth){
			printf("Use case 5A: logged in\n");

			//create the shift data to schedule
			msg.loggedIn    = 1;

			//shift for first test case
			s1.employeeID    = -1;
			s1.shiftDuration = 2;
			s1.time.day     = 3;
			s1.time.month   = 12;
			s1.time.year    = 22;
			s1.time.hour    = 9;
			s1.time.minutes = 0;
			msg.shift_d[0]  = s1;
			msg.size        = sizeof(msg);

			s2.employeeID    = -1;
			s2.shiftDuration = 2;
			s2.time.day     = 5;
			s2.time.month   = 12;
			s2.time.year    = 22;
			s2.time.hour    = 9;
			s2.time.minutes = 0;
			msg.shift_d[1]  = s2;

			//this is the last message sent by this thread, disconnect
			msg.pulse.code = _PULSE_CODE_DISCONNECT;

			/* send a get message to book a shift */
			MsgSend(coid, &msg, sizeof(msg) + sizeof(msg.shift_d), &reply, sizeof(reply));

			//check if the shift was booked

			if(reply.success1){

				printf("Use case 5A: \n 1-a Tow employees have the Different seniority , One of them is supervisor: \n Employee a (Supervisor) : The shift is booked successfully \n Employee c (Teach Support)  : It is not allowed to book this shift , choose another shift, please! \n");

			}else{
				printf("Use case 5A:\n 1-a shift is not valid ! \n ");

			}


			if(reply.success3){
				printf("Use case 5A: \n 2-b Tow employees have the Different seniority , Both of them is supervisor or both of them are teach Support :\n Employee d (Teach Support) : The shift is booked successfully. \n Employee c (Teach Support): It is not allowed to book this shift , choose another shift, please!  \n");


			}else{
				printf("Use case 5A:\n  2-b shift is not valid !\n");

			}


		}else{
			printf("Use case 5A: unable to login, access denied\n");
		}
		break;
	//execute use case 5B :Prioritised Employee Shift Selection :two employees of same seniority level reach for same shift
	case USE_CASE_FIVE_B:

		/* find our server to get a coid*/
		coid = name_open(SERVER_NAME, 0);

		//employee a variables
		empIDA   = 0;
		empPassA = 2222;
		a.password   = empPassA;
		a.id         = empIDA;
		a.seniority = 8;
		a.workedHours = 64;
		a.isSupervisor = 1;

		//employee b variables

		empIDB   = 1 ;
		empPassB = 1234;
		b.password   = empPassB;
		b.id         = empIDB;
		b.seniority = 8;
		b.workedHours = 64;
		b.isSupervisor = 1;

		//employee c variables
		empIDC   = 5 ;
		empPassC = 2545;
		h.password   = empPassC;
		h.id         = empIDC;
		h.seniority = 167;
		h.workedHours = 1006;
		h.isSupervisor = 0;



		//message to server details
		msg.type     = u_case;
		msg.emp_d[0] = a;
		msg.emp_d[1] = b;
		msg.emp_d[5] = h;
		msg.size     = sizeof(msg);

		/* send a get message to authenticate the employee */
		MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

		//if the user is authenticated
		if(reply.auth){
			printf("Use case 5B: logged in\n");
			msg.loggedIn    = 1;
			//create the shift data to schedule

			s3.employeeID    = -1;
			s3.shiftDuration = 1;
			s3.time.day     = 4;
			s3.time.month   = 12;
			s3.time.year    = 22;
			s3.time.hour    = 9;
			s3.time.minutes = 0;
			msg.shift_d[2]  = s3;
			msg.size        = sizeof(msg);

			//create the shift data to schedule

			s4.employeeID    = -1;
			s4.shiftDuration = 2;
			s4.time.day     = 4;
			s4.time.month   = 12;
			s4.time.year    = 22;
			s4.time.hour    = 13;
			s4.time.minutes = 0;
			msg.shift_d[3]  = s4;

			//this is the last message sent by this thread, disconnect
			msg.pulse.code = _PULSE_CODE_DISCONNECT;

			/* send a get message to book a shift */
			MsgSend(coid, &msg, sizeof(msg) + sizeof(msg.shift_d), &reply, sizeof(reply));

			//check if the shift was booked


			if(reply.success1){

				printf("Use case 5B: \n 1-a Tow employees have the same seniority , They are both supervisor: \n Employee(Supervisor) a : The shift is booked successfully. \n Employee(Supervisor) b : It is not allowed to book this shift , choose another shift, please!  \n");

			}else{
				printf("Use case 5B 1-a: shift is not valid ! \n");

			}


			if(reply.success3){

				printf("Use case 5B \n 2-b :Tow employees have the same seniority , They one of them is supervisor:\nEmployee(Supervisor) a : The shift is booked successfully.\n Employee(Teach Support) h : It is not allowed to book this shift , choose another shift, please!  \n");

			}else{
				printf("Use case 5B 2-b: shift is not valid ! \n");

			}

		}else{
			printf("Use case 5B: unable to login, access denied\n");
		}
		break;

	//Maryam

	default:;

	}
	return EOK;
}

int main(int argc, char **argv) {
	//define the use cases
	int useCases[] = {USE_CASE_FOUR,USE_CASE_FIVE_A,USE_CASE_FIVE_B};
	int n = sizeof(useCases)/(sizeof(useCases[0]));
	//define an array of threads
	pthread_t myThreads[n];

	//create a thread for each use case and provide the correct routine
	for(int i = 0; i < n; ++i){
		int *curr_useCase = (int*) malloc(sizeof(useCases[i]));
		if(curr_useCase == NULL){
			printf("MKH Use Cases: Malloc Error!\n");
			return -1;
		}
		*curr_useCase = useCases[i];

		int ret = pthread_create(&myThreads[i], NULL, perform_usecae, curr_useCase);
		if(ret !=  0){
	          printf("MKH Use Cases: error: Cannot create thread # %d\n", i);
	          return -1;
		}

	}

	//wait for the threads to terminate
	for(int i = 0; i < n; ++i){
		pthread_join(myThreads[i], NULL);
	}

	return EXIT_SUCCESS;
}
