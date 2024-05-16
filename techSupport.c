#include "techSupport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <time.h>
#include <stdbool.h>

char employeeName[256];
char passwordP[10];

void employee_ctor(struct employee* obj, const int id, const char* name, const char*password,const pseudo_time_t dob, const int seniority,
		const int workedHours, const bool isSupervisor);

bool bookShift(time h );

char checkSchedule();

bool login(char* name , char password );

bool bookShift(int shift);

bool changeShift(int oldShift, int newShift, int superAuthPin);

int main() {


	    return 0;
	}

void employee_ctor(struct employee* obj, const int id, const char* name, const char*password,const time dob, const int seniority,
		const int workedHours, const bool isSupervisor){
	obj->id = id;
	obj->name = name;
	obj-> password = name;
	obj->seniority = 0;
	obj->workedHours = 0;
	obj->dob = dob;
	obj->isSupervisor=0;
}
bool bookShift(time h ){

		if(validDate(h)== 0){
			return 0;
		}else{

		//send message to the server to bookshift
			//if the server book the shift, message return
			return 1;
		}
		return 0;
	}

char checkSchedule(){
		//send message to the server disply the schedule
		// the server return file
		//employee open the file and read
		//print to the screen
	}

bool login(char* name , char password ){
		int x,y;
		 printf("Enter the Name:\n");
		 scanf("%s", &name);

		 printf("Enter the Password:\n");
		 scanf("%s", &password);

		 x=strcmp(id, employeeName);
		 y=strcmp(password, passwordp);

		 //send message to the server to check the name and password
		 //if return true
		 if(1){
			 printf("User successfully logged in..."");
			 return 1;

		 }
		 else{
			 printf("Name or Password entered is invalid\n");

			 return 0;
		 }
		 return 0;
	}
bool bookShift(int shift){
		//send to the server to check if the shift valid
		if(1){
			return 1;
		}else{
			return 0;
		}
		return 0;

	}
bool changeShift(int oldShift, int newShift, int superAuthPin){
		//send to the server to check if the old shift and superAuthpin are valid
		//server make the change
		if(1){
					return 1;
				}else{
					return 0;
				}
				return 0;
	}

