#include <string.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <time.h>
#include<stdbool.h>
#include<supervisor.h>

void supervisor_con(struct employee* obj, const int id, const char* name, const char*password, const time dob, const int seniority,
		const int workedHours, const bool isSupervisor);

bool addNewEmp(char* name ,time  dob,time startDate);

bool removeEmp(char* name,char* dob ,time lastDate);



int main() {


    return 0;
}

void supervisor_con(struct employee* obj, const int id, const char* name, const char*password, const time dob, const int seniority,
		const int workedHours, const bool isSupervisor){
	employee_ctor(&obj->base ,id,name,password,dob,workedHours,seniority,workedHours,isSupervisor);
	obj->isSupervisor = 1;
}

bool addNewEmp(char* name ,time  dob,time startDate){
	//send message to the server to check the name ,date of birth and start date is valid
	//if it valid the server will add the employee to data
	if(1){
		return 1;
	}else{
		return 0;
	}
	return 0;
}

bool removeEmp(char* name,char* dob ,time lastDate){
	//send message to the server to check the name ,date of birth and start date is valid
		//if it valid the server will add the employee to fired employee data
		//if it valid the server will remove the employee to current employee data
	if(1){
			return 1;
		}else{
			return 0;
		}
		return 0;
}

