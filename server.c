#include "server.h"
#define MAX_STRING_LEN    256


int checkEmpID(int id){
	int exists = 0;

	for(int i = 0; i < employeeCount; ++i){
		if(emps[i].id == id){
			exists = 1;
		}
	}

	return exists;
}


/*
 * @purpose - prints all shifts
 * @param s - nothing
 * @returns - nothing
 */
void printShifts()
{
	for (int i = 0; i < shiftCount; ++i)
	{
		printf("\nShift scheduled for %s at %d:%d%d for %d hours on %d/%d/%d with id %d and employee id %d\n",
					 shifts[i].name, shifts[i].time.hour, shifts[i].time.minutes, shifts[i].time.seconds,
					 shifts[i].shiftDuration, shifts[i].time.month, shifts[i].time.day, shifts[i].time.year, shifts[i].id, shifts[i].employeeID);
	}
}


/*
 * @purpose - reads shift data from a shifts array and check if theres a gap that needs to be filled
 * @param - nothing
 * @returns - nothing
 */
void fillSched()
{
	int count = 0;
	for (int i = 0; i < shiftCount; ++i)
	{
		if (shifts[i].time.hour < 13)
		{
			for (int j = 0; j < shiftCount; ++j)
			{
				if (shifts[i].time.day == shifts[j].time.day && shifts[i].time.month == shifts[j].time.month && shifts[i].time.year == shifts[j].time.year && shifts[j].time.hour == 13)
				{
					count++;
				}
			}
			if (count == 0)
			{
				struct shift *s = (struct shift *)malloc(sizeof(struct shift));
				// select a random employee
				int randIndex = rand() % employeeCount;
				s->employeeID = emps[randIndex].id;
				s->name = shifts[i].name;
				// shift duration either 4 or 8 hours
				s->shiftDuration = rand() % 2 == 0 ? 4 : 8;
				s->time = shifts[i].time;
				s->time.day = shifts[i].time.day;
				s->time.month = shifts[i].time.month;
				s->time.year = shifts[i].time.year;
				s->time.hour = 13;
				s->time.minutes = 0;
				s->time.seconds = 0;
				schedShift(s);
			}
			count = 0;
		}
	}
	printf("\nSchedule filled\n");
}


//Maryam

/* @purpose         - represents the path of two employees of the same seniority level trying to book the same shift

 * @param a         - first employee
 * @param b		    - second employee
 *
 * @returns         - return 1 if employee a has higher priority , return 0 if employee b has higher priority
 */

int checksEmpLevelB(struct employee *a,struct employee *b){
	// both employees have equal seniority is true
		//both employee are supervisor is true
		if(a->isSupervisor && b->isSupervisor){
			//check the id , because the older employee will have small number of id
			if(a->id < b->id){
				return 1;
			}else{
				return 0;
				}
		//not both of employee are supervisor , check if a is supervisor
		}else if(a->isSupervisor){
				return 1;
		// check if b is supervisor
		}else if(b->isSupervisor){
				return 0;
		// no supervisor is found, check the employee id
		}else{
			if(a->id < b->id){
				return 1;
			}else{
				return 0;
				}
		}

}
/* @purpose         - represents the path of two employees of different  seniority level trying to book the same shift

 * @param a         - first employee
 * @param b		    - second employee
 *
 * @returns         - return 1 if employee a has higher priority , return 0 if employee b has higher priority
 */
int checksEmpLevelA(struct employee *a,struct employee *b){
	// the employees' seniority are not equal
		//check if a is supervisor
		if(a->isSupervisor){
			return 1;
			//check if b is supervisor
		}else{
			if(b->isSupervisor){
				return 0;
			}else{// no supervisor is found , check the employee seniority
				if(a->seniority > b->seniority){
					return 1;
				}else{
					return 0;
				}
			}
		}

	}
//Maryam

/* @purpose         - gets a schedule for either a single employee 'e' OR the entire
 * 			          work week.
 * @param e         - an employee
 * @param fullSched - whether the user wants to get the full weekly schedule
 * 					  or not
 * @param size      - an output parameter representing the size of the returned
 * 				      list of scheduled shifts.
 * @returns         - a list of shifts either worked by a single employee 'e' or
 * 			          the entire work week
 */
struct shift * getSch(struct employee* e, int fullSched, int * size){
	//if the user wants the full week scehdule
	if(fullSched){
		//make sure they are a supervisor
		if(!e->isSupervisor){
			return NULL;
		}
		//get the shifts
		struct shift * sched = (struct shift*) malloc (sizeof(struct shift)*shiftCount);
		for(int i = 0; i < shiftCount; ++i){
			sched[i] = shifts[i];
		}
		*size = shiftCount;
		return sched;
	}else{
		//get the number of shift to return
		int n = 0;
		for(int i = 0; i < shiftCount; ++i){
			if(e->id == shifts[i].employeeID){
				n++;
			}
		}

		//find all the shifts owned by the employee
		struct shift * sched = (struct shift*) malloc (sizeof(struct shift)*n);
		int index = 0;
		for(int i = 0; i < shiftCount; ++i){
			if(e->id == shifts[i].employeeID){
				sched[index] = shifts[i];
				index++;
			}
		}
		*size = n;
		return sched;
	}
}

/*
 * @purpose - checks that a given shift is valid
 * @param s - a shift to schedule
 * @returns - 1 if the shift is valid and 0 otherwise
 */
int checkShift(struct shift * s){

	//check if maximum allowable shifts have been reached
	//check if the shift duration is valid (1-8 hours)
	if(shiftCount >= MAX_SHIFTS || (s->shiftDuration < 1 || s->shiftDuration > 8)){

		return 0;
	}

	//check employee has not exceeded maximum hours they can work a week
	int weekly_hours_worked = 0;
	for(int i = 0; i < shiftCount; ++i){
		if(shifts[i].employeeID == s->employeeID){
			weekly_hours_worked += shifts[i].shiftDuration;
		}
	}
	if(weekly_hours_worked > MAX_HOURS_WORK){
        printf("ghdjks\n");
        printf("weekly_hours_worked %d\n",weekly_hours_worked);
		return 0;
	}

	int endTime = s->time.hour + s->shiftDuration;
	//check start time is valid
	if(s->time.hour < OPENING_HOUR){

		return 0;
	}

	//check end time
	if(endTime > CLOSING_HOUR && s->time.minutes != 0){

		return 0;
	}

	return 1;
}

/*
 * @purpose - schedules a shift for an employee
 * @param s - a shift to schedule
 * @returns - nothing
 */
void schedShift(struct shift * s){
	shifts[shiftCount].employeeID    = s->employeeID;
	shifts[shiftCount].id            = shiftCount;
	shifts[shiftCount].name          = (char *) malloc(MAX_NAME_LENGTH);
	shifts[shiftCount].name          = s->name;
	shifts[shiftCount].shiftDuration = s->shiftDuration;
	shifts[shiftCount].time          = s->time;
	shiftCount++;
}

/*
 * @purpose        - used to save the shifts data by writing the information into a file and free the shifts array
 * @param fileName - the name of the file to write to
 * @returns        - nothing
 */
void saveShifts(char* fileName){
	FILE* file = fopen(fileName, "w+");
	fputs("shiftID | Hours | Emplyee | Date | startTime\n", file);
	for(int i = 0; i < shiftCount; ++i){
		fprintf(file, "%d %d %s %d/%d/%d %d:%d\n", shifts[i].id, shifts[i].shiftDuration, shifts[i].name, shifts[i].time.day, shifts[i].time.month, shifts[i].time.year, shifts[i].time.hour, shifts[i].time.minutes);
	}
	free(shifts);
	fclose(file);
}


/*
 * @purpose  - used to initialize the scheduled shifts. Reads a file and populates the shifts list
 * @param    - the full path to the file (must be stored in /tmp/ directory of VM)
 * @returns  - the number of scheduled shifts
 */
int initSched(char* fileName){
	FILE* file = fopen(fileName, "r");
	int count = 0, i = 0;
	char line[256];

	while(fgets(line, sizeof(line), file)){
		if(count != 0){
			shifts[i].name = (char *) malloc(MAX_NAME_LENGTH);
			sscanf(line, "%d %d %s %d/%d/%d %d:%d", &shifts[i].id, &shifts[i].shiftDuration, shifts[i].name, &shifts[i].time.day, &shifts[i].time.month, &shifts[i].time.year, &shifts[i].time.hour, &shifts[i].time.minutes);
			for(int j = 0; j < employeeCount; ++j){
				if(!strcmp(emps[j].name, shifts[i].name)){
					shifts[i].employeeID = emps[j].id;
				}
			}
			i++;
		}
		count++;
	}
	fclose(file);
	return i;
}


/*
 * @purpose - removes (i.e., fires) an employee from the list of employees
 * @param supName - name of the supervisor doing the removal
 * @param empname - the name of the employee being removed
 * @param supPass - the supervisor password. Used to authenticate the deletion process
 * @param empID   - the ID of the employee being removed
 * @returns auth  - an integer wherein 1 denotes that the removal can/will be performed
 * 					and 0 if it cannot be performed
 */
int removeEmpFromCurr(char* supName, char* empName, int supPass, int empID){
	int auth = 0;
	for(int i = 0; i < employeeCount; ++i){
		if(!strcmp(supName, emps[i].name)){
			if(supPass == emps[i].password && emps[i].isSupervisor){
				auth = 1;
			}
		}
	}
	if(auth){
		for(int i = 0; i < employeeCount; ++i){
			if(emps[i].id == empID && !strcmp(empName, emps[i].name)){
				struct employee * tmp = &emps[i];
				for(int j = i; j < employeeCount-1; ++j){
					emps[j] = emps[j+1];
				}
				employeeCount--;
				free(tmp->name);
				break;
			}
		}
	}
	return auth;
}


/*
 * @purpose           - used to validate the employee before adding them to the list of employees.
 * 			            a new employee must have a name length less than the max (50) and be at least
 * 			            18 years old
 * @param name        - the name of the employee
 * @param yearOfBirth - the year the employee was born
 * @returns           - 1 if the employee information is valid and 0 otherwise
 */
int validateEmpInfo(char* name, int yearOfBirth){
	if(strlen(name) > MAX_NAME_LENGTH){
		printf("Name to long\n");
		return 0;
	}
	time_t sec = time(NULL);
	struct tm* curr_time = localtime(&sec);
	if(((curr_time->tm_year + 1900) - yearOfBirth) < 18){
		return 0;
	}
	return 1;
}


/*
 * @purpose - used to add an employee to the company
 * @param name  - the name of the employee
 * @param pass  - the employee password, used for authentication
 * @param isSup - whether the employee registering is a supervisor or not
 * @returns     - 1 if the employee is successfully added, and 0 otherwise
 */
int addEmpToCurrent(char *name, int pass, int isSup){
	if(employeeCount == MAX_EMPLOYEES || !isSup){
		return 0;
	}
	emps[employeeCount].id           = employeeCount;
	emps[employeeCount].name         = (char *) malloc(MAX_NAME_LENGTH);
	emps[employeeCount].isSupervisor = 0;
	emps[employeeCount].password     = pass;
	emps[employeeCount].seniority    = 0;
	emps[employeeCount].weeklyHours  = 0;
	emps[employeeCount].workedHours  = 0;
	strcpy(emps[employeeCount].name, name);

	printf("ADDED: %s\n", emps[employeeCount].name);


	employeeCount++;
	return 1;
}


/*
 * @purpose - used to check whether employee 'a' has more seniority than employee 'b'
 * @param a - an employee struct
 * @param b - an employee struct
 * @returns - 1 if employee 'a' has more seniority than employee 'b' and 0 otherwise
 */
int checksEmpLevel(struct employee* a, struct employee* b){
	if(a->isSupervisor){
		a->seniority = a->workedHours/6;
	}else if(!a->isSupervisor){
		a->seniority = a->workedHours/8;
	}else if(!b->isSupervisor){
		b->seniority = b->workedHours/6;
	}else if(!b->isSupervisor){
		b->seniority = b->workedHours/8;
	}
	return a->seniority > b->seniority;
}

/*
 * @purpose    - used to authorize a user when they log in
 * @param pass - the user's password
 * @returns    - 1 if the password exists and 0 otherwise
 */
int authUser(int empID, int pass){
	int valid = 0;
	for(int i = 0; i < employeeCount; ++i){
		if(emps[i].password == pass && emps[i].id == empID){
			valid = 1;
		}
	}

	return valid;
}

/*
 * @purpose  - used to initialize the known employees. Reads a file and populates the employee list
 * @param    - the full path to the file (must be stored in /tmp/ directory of VM)
 * @returns  - the number of known employees
 */
int initializeEmps(char* fileName){
	FILE* file = fopen(fileName, "r");
	int count = 0, i = 0;
	char line[256];

	while(fgets(line, sizeof(line), file)){
		if(count != 0){
			emps[i].name = (char *) malloc(MAX_NAME_LENGTH);
			sscanf(line, "%d %s %d %d %d", &emps[i].id, emps[i].name, &emps[i].password, &emps[i].workedHours, &emps[i].isSupervisor);
			i++;
		}
		count++;
	}
	fclose(file);
	return i;
}

/*
 * @purpose        - used to save the employee data by writing the information into a file and free the employees array
 * @param fileName - the name of the file to write to
 * @returns        - nothing
 */
void saveEmployees(char* fileName){
	FILE* file = fopen(fileName, "w+");
	fputs("id | employeeName | passwrd | hoursWorked | isSup\n", file);
	for(int i = 0; i < employeeCount; ++i){
		fprintf(file, "%d %s %d %d %d\n", emps[i].id, emps[i].name, emps[i].password, emps[i].workedHours, emps[i].isSupervisor);
	}
	free(emps);
	fclose(file);
}

/*
 * @purpose  - getter function that returns the name of an employee
 * 			   based on their employee ID
 * @param id - an employee ID
 * @returns  - the name of the employee associated with the ID if found,
 * 			   an empty string otherwise
 */
char * getEmpName(int id){
	for(int i = 0; i < employeeCount; ++i){
		if(emps[i].id == id){
			return emps[i].name;
		}
	}
	return "";
}
//maryam
int addSupToCurrent(char *name, int pass, int isSup){
	if(employeeCount == MAX_EMPLOYEES ){
		return 0;
	}
	emps[employeeCount].id           = employeeCount;
	emps[employeeCount].name         = (char *) malloc(MAX_NAME_LENGTH);
	emps[employeeCount].isSupervisor = 1;
	emps[employeeCount].password     = pass;
	emps[employeeCount].seniority    = 0;
	emps[employeeCount].weeklyHours  = 0;
	emps[employeeCount].workedHours  = 0;
	strcpy(emps[employeeCount].name, name);

	employeeCount++;
	return 1;
}
//maryam
int main(){

	// all required data for message passing
	name_attach_t * attach;
	int rcvid, runMain = 1;
	int coid;
	client_data msg;
	server_data serv_resp;
	struct client_data * buff;
	struct _msg_info info;

	//files to initialize
	char * fileNameEmps = "registered_employees.txt";
	char * fileNameShifts = "weekly_shifts.txt";

	//initialize server data
	emps = (struct employee*) malloc(sizeof(struct employee) * MAX_EMPLOYEES);
	shifts = (struct shift*) malloc(sizeof(struct shift) * MAX_SHIFTS);
	employeeCount = initializeEmps(fileNameEmps);
	shiftCount = initSched(fileNameShifts);

	//timer
	struct sigevent sigevent;
	struct itimerspec itime;
	timer_t timer_id;

	//end add employees for testing
	attach = name_attach(NULL, SERVER_NAME, 0);
	coid = ConnectAttach(0,0,attach->chid,_NTO_SIDE_CHANNEL,0);

	/*Fill in the sigevent to request a Pulse when the timer expires*/
	SIGEV_PULSE_INIT (&sigevent, coid, SIGEV_PULSE_PRIO_INHERIT,TIMER_PULSE_EVENT,0);
	timer_create(CLOCK_REALTIME, &sigevent, &timer_id);

	//timer end in 15 mints after the employee login , but her 60 sec for testing
	itime.it_value.tv_sec = SESSION_LEN;
	itime.it_value.tv_nsec = 0;
	itime.it_interval.tv_sec =0;
	itime.it_interval.tv_nsec =0;



	//while the server is running
	while (runMain) {
		//receive message
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), &info);


		if (0 == rcvid) {
			//received a pulse
			switch(msg.pulse.code){
			case TIMER_PULSE_EVENT:
				printf("Server(pulse from timer): The Session is timeout .\n");
				itime.it_value.tv_sec = 0;
				itime.it_value.tv_nsec = 0;
				timer_settime(timer_id, 0, &itime, NULL);
				break;
			case _PULSE_CODE_DISCONNECT:
				printf("Server: client is gone.\n");
				break;

			default:
				printf("Server: received pulse (code, value): ('%d', '%d')\n", msg.pulse.code, msg.pulse.value.sival_int);
				break;
			}


		} else {

			// we got a message, check its type and process the msg based on its type
			switch(msg.type){

			//received message to terminate the server
			case TERM_SERVER:
				runMain = 0;
				break;

			case USE_CASE_ONE:
				// allocate memory for the data
				buff = (struct client_data *) malloc(msg.size);

				//read the data
				MsgRead(rcvid, buff, msg.size, 0);

				//if the user is not logged in, attempt to authenticate then reply
				if(!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}else{
					//the user wants to schedule a shift, create a shift data structure
					struct shift * tmpShift = (struct shift*) malloc(sizeof(struct shift));

					//set all the necessary shift details
					tmpShift->employeeID = buff->shift_data.employeeID;
					tmpShift->name = malloc(MAX_NAME_LENGTH);
					tmpShift->shiftDuration = buff->shift_data.shiftDuration;
					tmpShift->time = buff->shift_data.time;
					strcpy(tmpShift->name, getEmpName(tmpShift->employeeID));

					//make sure the shift is valid
					if(checkShift(tmpShift)){
						//schedule the shift
						schedShift(tmpShift);

						//reply successful
						serv_resp.success = 1;
						MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
					}else{
						//could not create shift, reply fail
						serv_resp.success = 0;
						MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
					}
					//free any allocated memory
					free(tmpShift);
				}
				//free any allocated memory
				free(buff);
				break;

			case USE_CASE_TWO:
				// allocate memory for the data
				buff = (struct client_data *) malloc(msg.size);

				//read the data
				MsgRead(rcvid, buff, msg.size, 0);

				//if the user is not logged in, attempt to authenticate then reply
				if(!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}else{
					//the number of shifts the user has
					int n = 0;

					//get all scheduled shifts and the number, 'n', of shifts
					struct shift * retShift = getSch(&buff->emp_data, 0, &n);

					//set all necessary shift data for employee
					serv_resp.success        = 1;
					serv_resp.size           = n;
					serv_resp.shift_data     = retShift[buff->shiftsReceived];

					//if all the shifts have been sent to the employee, update the more
					//data to send flag
					serv_resp.moreDataToSend = (n == buff->shiftsReceived) ? 0 : 1;

					//reply to the employee with a shift they have scheduled
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}
				//free any allocated memory
				free(buff);
				break;

			case USE_CASE_THREE:
				// allocate memory for the data
				buff = (struct client_data *) malloc(msg.size);

				//read the data
				MsgRead(rcvid, buff, msg.size, 0);

				//if the user is not logged in, attempt to authenticate then reply
				if(!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}else{
					//the number of shifts the user has
					int n = 0;

					//get all scheduled shifts and the number, 'n', of shifts
					struct shift * retShift = getSch(&buff->emp_data, 1, &n);

					//set all necessary shift data for employee
					serv_resp.success        = 1;
					serv_resp.size           = n;
					serv_resp.shift_data     = retShift[buff->shiftsReceived];

					//if all the shifts have been sent to the employee, update the more
					//data to send flag
					serv_resp.moreDataToSend = (n == buff->shiftsReceived) ? 0 : 1;

					//reply to the employee with a shift they have scheduled
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}

				//free any allocated memory
				free(buff);

				break;

				//Maryam

			case USE_CASE_FOUR:
				// allocate memory for the data
				buff = (struct client_data *) malloc(msg.size);
				//read the data
				MsgRead(rcvid, buff, msg.size, 0);

				if(!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
				}

				MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));

				timer_settime(timer_id, 0, &itime, NULL);
				free(buff);
				break;

			case USE_CASE_FIVE_A: //
				// allocate memory for the data
				buff = (struct client_data *)malloc(msg.size);

				// read the data
				MsgRead(rcvid, buff, msg.size, 0);

				// if the user is not logged in, attempt to authenticate then reply
				if (!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_d[0].id, buff->emp_d[0].password);
					serv_resp.auth = authUser(buff->emp_d[2].id, buff->emp_d[2].password);
					serv_resp.auth = authUser(buff->emp_d[3].id, buff->emp_d[3].password);

				}else{//test first case

					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
					int first = 0 ;
					first = checksEmpLevelA(&buff->emp_d[0],&buff->emp_d[2]);
					if(first){
						//the user wants to schedule a shift, create a shift data structure
						struct shift * tmpShift1 = (struct shift*) malloc(sizeof(struct shift));

						//set all the necessary shift details
						tmpShift1->employeeID = buff->emp_d[0].id;
						tmpShift1->name = malloc(MAX_NAME_LENGTH);
						tmpShift1->shiftDuration = buff->shift_d[0].shiftDuration;
						tmpShift1->time = buff->shift_d[0].time;
						strcpy(tmpShift1->name, getEmpName(tmpShift1->employeeID));

						//make sure the shift is valid
						if(checkShift(tmpShift1)){

							//schedule the shift
							schedShift(tmpShift1);
							serv_resp.success1 = 1;


							//free any allocated memory
							free(tmpShift1);
						}else{
							//could not create shift, reply fail
							serv_resp.success1 = 0;

							//free any allocated memory
							free(tmpShift1);
							}

					}else{ // else of first
						//the user wants to schedule a shift, create a shift data structure
						struct shift * tmpShift2 = (struct shift*) malloc(sizeof(struct shift));

						//set all the necessary shift details
						tmpShift2->employeeID = buff->emp_d[2].id;
						tmpShift2->name = malloc(MAX_NAME_LENGTH);
						tmpShift2->shiftDuration = buff->shift_d[0].shiftDuration;
						tmpShift2->time = buff->shift_d[0].time;
						strcpy(tmpShift2->name, getEmpName(tmpShift2->employeeID));

						//make sure the shift is valid
						if(checkShift(tmpShift2)){
							//schedule the shift
							schedShift(tmpShift2);
							//reply successful
							serv_resp.success2 = 1;

							//free any allocated memory
							free(tmpShift2);

						}else{
							//could not create shift, reply fail
							serv_resp.success2 = 0;

							//free any allocated memory
							free(tmpShift2);
							}

					}

				int second = 0;
				second = checksEmpLevelA(&buff->emp_d[2],&buff->emp_d[3]);

				if(second){
					//the user wants to schedule a shift, create a shift data structure
					struct shift * tmpShift1 = (struct shift*) malloc(sizeof(struct shift));

					//set all the necessary shift details
					tmpShift1->employeeID = buff->emp_d[2].id;
					tmpShift1->name = malloc(MAX_NAME_LENGTH);
					tmpShift1->shiftDuration = buff->shift_d[1].shiftDuration;
					tmpShift1->time = buff->shift_d[1].time;
					strcpy(tmpShift1->name, getEmpName(tmpShift1->employeeID));

					//make sure the shift is valid
					if(checkShift(tmpShift1)){
						//schedule the shift
						schedShift(tmpShift1);

						serv_resp.success3 = 1;

						//free any allocated memory
						free(tmpShift1);
					}else{
						//could not create shift, reply fail
						serv_resp.success3 = 0;

						//free any allocated memory
						free(tmpShift1);
						}

				}else{//end of second
					//the user wants to schedule a shift, create a shift data structure
					struct shift * tmpShift2 = (struct shift*) malloc(sizeof(struct shift));

					//set all the necessary shift details
					tmpShift2->employeeID = buff->emp_d[3].id;
					tmpShift2->name = malloc(MAX_NAME_LENGTH);
					tmpShift2->shiftDuration = buff->shift_d[1].shiftDuration;
					tmpShift2->time = buff->shift_d[1].time;
					strcpy(tmpShift2->name, getEmpName(tmpShift2->employeeID));

					//make sure the shift is valid
					if(checkShift(tmpShift2)){
						//schedule the shift
						schedShift(tmpShift2);
						//reply successful

						serv_resp.success4 = 1;


						//free any allocated memory
						free(tmpShift2);

					}else{
						//could not create shift, reply fail

						serv_resp.success4 = 0;


						//free any allocated memory
						free(tmpShift2);

						}

					}

				}
				serv_resp.success3 = 1;
				MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				//free any allocated memory
				free(buff);
				break;

			case USE_CASE_FIVE_B:
				// allocate memory for the data
				buff = (struct client_data *)malloc(msg.size);

				// read the data
				MsgRead(rcvid, buff, msg.size, 0);

				// if the user is not logged in, attempt to authenticate then reply
				if (!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_d[0].id, buff->emp_d[0].password);
					serv_resp.auth = authUser(buff->emp_d[1].id, buff->emp_d[1].password);
					serv_resp.auth = authUser(buff->emp_d[5].id, buff->emp_d[5].password);

				}else{//test second case
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
					int third = 0;
					third = checksEmpLevelB(&buff->emp_d[0],&buff->emp_d[1]);

					if(third){
						//the user wants to schedule a shift, create a shift data structure
						struct shift * tmpShift3 = (struct shift*) malloc(sizeof(struct shift));

						//set all the necessary shift details
						tmpShift3->employeeID = buff->emp_d[0].id;
						tmpShift3->name = malloc(MAX_NAME_LENGTH);
						tmpShift3->shiftDuration = buff->shift_d[2].shiftDuration;
						tmpShift3->time = buff->shift_d[2].time;
						strcpy(tmpShift3->name, getEmpName(tmpShift3->employeeID));

						//make sure the shift is valid
						if(checkShift(tmpShift3)){
							//schedule the shift
							schedShift(tmpShift3);

							serv_resp.success1 = 1;

							//free any allocated memory
							free(tmpShift3);
						}else{
							//could not create shift, reply fail
							serv_resp.success1 = 0;

							//free any allocated memory
							free(tmpShift3);
						}

					}else{
						//the user wants to schedule a shift, create a shift data structure
						struct shift * tmpShift3 = (struct shift*) malloc(sizeof(struct shift));

						//set all the necessary shift details
						tmpShift3->employeeID = buff->emp_d[1].id;
						tmpShift3->name = malloc(MAX_NAME_LENGTH);
						tmpShift3->shiftDuration = buff->shift_d[2].shiftDuration;
						tmpShift3->time = buff->shift_d[2].time;
						strcpy(tmpShift3->name, getEmpName(tmpShift3->employeeID));

						//make sure the shift is valid
						if(checkShift(tmpShift3)){
							//schedule the shift
							schedShift(tmpShift3);
							//reply successful
							serv_resp.success2 = 1;

							//free any allocated memory
							free(tmpShift3);

						}else{
							//could not create shift, reply fail
							serv_resp.success2 = 0;

							//free any allocated memory
							free(tmpShift3);
						}

					}

					int forth = 0;
					forth = checksEmpLevelA(&buff->emp_d[0],&buff->emp_d[5]);

					if(forth){
					//the user wants to schedule a shift, create a shift data structure
						struct shift * tmpShift4 = (struct shift*) malloc(sizeof(struct shift));

						//set all the necessary shift details
						tmpShift4->employeeID = buff->emp_d[0].id;
						tmpShift4->name = malloc(MAX_NAME_LENGTH);
						tmpShift4->shiftDuration = buff->shift_d[3].shiftDuration;
						tmpShift4->time = buff->shift_d[3].time;
						strcpy(tmpShift4->name, getEmpName(tmpShift4->employeeID));

						//make sure the shift is valid
						if(checkShift(tmpShift4)){
							//schedule the shift
							schedShift(tmpShift4);
							serv_resp.success3 = 1;

							//free any allocated memory
							free(tmpShift4);
						}else{
							//could not create shift, reply fail
							serv_resp.success3 = 0;

							//free any allocated memory
							free(tmpShift4);
						}

					}else{
						//the user wants to schedule a shift, create a shift data structure
						struct shift * tmpShift4 = (struct shift*) malloc(sizeof(struct shift));

						//set all the necessary shift details
						tmpShift4->employeeID = buff->emp_d[5].id;
						tmpShift4->name = malloc(MAX_NAME_LENGTH);
						tmpShift4->shiftDuration = buff->shift_d[3].shiftDuration;
						tmpShift4->time = buff->shift_d[3].time;
						strcpy(tmpShift4->name, getEmpName(tmpShift4->employeeID));

						//make sure the shift is valid
						if(checkShift(tmpShift4)){
							//schedule the shift
							schedShift(tmpShift4);
							//reply successful
							serv_resp.success4 = 1;

							//free any allocated memory
							free(tmpShift4);

						}else{
							//could not create shift, reply fail

							serv_resp.success4 = 0;

							//free any allocated memory
							free(tmpShift4);
						}
					}

				}
				MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				//free any allocated memory
				free(buff);
				break;

				//Maryam

			case USE_CASE_SIX:
				// allocate memory for the data
				buff = (struct client_data *) malloc(msg.size);

				//read the data
				MsgRead(rcvid, buff, msg.size, 0);

				//if the user is not logged in, attempt to authenticate then reply
				if(!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}else{
					if(validateEmpInfo(EMP_TO_ADD, buff->emp_data.dob)){
						addEmpToCurrent(EMP_TO_ADD, buff->emp_data.password, 1);
						serv_resp.success = 1;
					}else{
						serv_resp.success = 0;
					}
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}
				break;
			case USE_CASE_SEVEN:
				// allocate memory for the data
				buff = (struct client_data *) malloc(msg.size);

				//read the data
				MsgRead(rcvid, buff, msg.size, 0);

				//if the user is not logged in, attempt to authenticate then reply
				if(!buff->loggedIn){
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}else{
					if(checkEmpID(buff->emp_data.id)){
						removeEmpFromCurr("John", getEmpName(buff->emp_data.id), 2222, buff->emp_data.id);
						serv_resp.success = 1;
					}else{
						serv_resp.success = 0;
					}
				}
				MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				break;
			case USE_CASE_EIGHT:
				// allocate memory for the data
				buff = (struct client_data *)malloc(msg.size);

				// read the data
				MsgRead(rcvid, buff, msg.size, 0);

				// if the user is not logged in, attempt to authenticate then reply
				if (!buff->loggedIn)
				{
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}
				else
				{
					// check shift for the employee (employee already has worked more than 40 hours so it will be invalid.)
					if (!checkShift(&buff->shift_data))
					{
						// reply with unsuccessful attempt to book shift
						serv_resp.success = 0;
						MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
					}
					else
					{
						// the number of shifts the user has
						int n = 0;

						// get all scheduled shifts and the number, 'n', of shifts
						struct shift *retShift = getSch(&buff->emp_data, 1, &n);

						// set all necessary shift data for employee
						serv_resp.success = 1;
						serv_resp.size = n;
						serv_resp.shift_data = retShift[buff->shiftsReceived];

						// if all the shifts have been sent to the employee, update the more
						// data to send flag
						serv_resp.moreDataToSend = (n == buff->shiftsReceived) ? 0 : 1;

						// reply to the employee with a shift they have scheduled
						MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
					}
				}
				// free any allocated memory
				free(buff);
				break;
			case USE_CASE_NINE:
				// allocate memory for the data
				buff = (struct client_data *)malloc(msg.size);

				// read the data
				MsgRead(rcvid, buff, msg.size, 0);

				// if the user is not logged in, attempt to authenticate then reply
				if (!buff->loggedIn)
				{
					serv_resp.auth = authUser(buff->emp_data.id, buff->emp_data.password);
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}
				else
				{
					fillSched();
					printf("printing all shifts:\n");
					printShifts();
					serv_resp.success = 1;
					MsgReply(rcvid, EOK, &serv_resp, sizeof(serv_resp));
				}
				// free any allocated memory
				free(buff);
				break;

			default:
				break;
			}
		}

	}
	//save all data
	saveEmployees(fileNameEmps);
	saveShifts(fileNameShifts);

	return EXIT_SUCCESS;
}











