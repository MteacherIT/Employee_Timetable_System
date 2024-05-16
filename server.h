#ifndef _SERVER_H_
#define _SERVER_H_

#include <unistd.h>
#include <sys/iomsg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <pthread.h>

#include "employee.h"
#include "shift.h"
#include "time.h"

//define global variables
#define SERVER_NAME "TimeTable" //use this as the name to register in the pathname space to create and connect to the channel
#define MAX_NAME_LENGTH 50
#define MAX_SHIFTS 50
#define MAX_HOURS_WORK 50
#define MAX_EMPLOYEES 50
#define OPENING_HOUR 9
#define CLOSING_HOUR 17
#define SESSION_LEN    6


//define cases
#define USE_CASE_ONE 1
#define USE_CASE_TWO 2
#define USE_CASE_THREE 3
#define USE_CASE_FOUR 4
#define USE_CASE_FIVE_A 5
#define USE_CASE_FIVE_B 6
#define USE_CASE_SIX 7
#define USE_CASE_SEVEN 8
#define USE_CASE_EIGHT 9
#define USE_CASE_NINE 10
#define USE_CASE_TEN 11
#define TERM_SERVER 1000
#define TIMER_PULSE_EVENT (_PULSE_CODE_MINAVAIL + 7)

//list of employees
struct employee * emps;

//list of shifts
struct shift * shifts;

//counter variables
int employeeCount = 0;
int shiftCount    = 0;

//timer
struct sigevent sigevent;
struct itimerspec itime;
timer_t timer_id;

//all implemented functions
int authUser(int, int);
int checksEmpLevel(struct employee*, struct employee*);
int initializeEmps(char*);
void saveEmployees(char*);
int addEmpToCurrent(char*, int, int);
int validateEmpInfo(char*, int);
int removeEmpFromCurr(char*, char*, int, int);
int initSched(char*);
void saveShifts(char*);
void schedShift(struct shift *);
int checkShift(struct shift *);
struct shift * getSch(struct employee*, int, int*);
char * getEmpName(int);
//maryam
int checksEmpLevelB(struct employee* ,struct employee* );
int checksEmpLevelA(struct employee* ,struct employee* );
int addSupToCurrent(char* , int , int );
//maryam



//not really necessary functions
int notifyEmployee(struct shift);
int replyFailed(int);
int replySuccess(int);
pseudo_time_t * checkAccessTime();
int addEmpToFired(char*, pseudo_time_t*);
int validateShiftChange(struct shift*, struct shift*, int);
int changeShift(struct shift*);


//data sent from the server
typedef struct server_data {
	uint16_t type;
	uint16_t size;
	uint16_t auth;
	uint16_t success;
	uint16_t success1;
	uint16_t success2;
	uint16_t success3;
	uint16_t success4;

	uint16_t moreDataToSend;
	struct shift shift_data;
} server_data;



//data sent from the employee
typedef struct client_data {
	uint16_t type;
	uint16_t size;
	uint16_t loggedIn;
	uint16_t shiftsReceived;
	struct shift shift_data;
	struct employee emp_data;
	struct employee emp_d[MAX_EMPLOYEES];
	struct shift shift_d[4];
	struct _pulse pulse;
} client_data;




#endif //_SERVER_H_

