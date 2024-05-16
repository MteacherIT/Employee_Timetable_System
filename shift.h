#ifndef _SHIFT_H_
#define _SHIFT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "time.h"
#include <stdbool.h>

//typedef int bool;



struct shift{
	 int id;
	 char * name;
	 pseudo_time_t time;
	 int employeeID;
	 int shiftDuration;
};

int occupy(int);   //input id
int deoccupy(int); //input id
int isOccupied();

#endif //_SHIFT_H_



