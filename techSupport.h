#ifndef _TECH_SUPPORT_H_
#define _TECH_SUPPORT_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <time.h>
#include <stdbool.h>
#include "time.h"

//typedef int bool;

struct employee{
	 int id;
	 char* name;
	 int password;
	 int dob;
	 int seniority;
	 int workedHours;
	 int weeklyHours;
	 int  isSupervisor;

};

#endif

