#ifndef _TIME_H_
#define _TIME_H_

typedef struct
{
	char time;
	int hour;
	int minutes;
	int seconds;
	int day;
	int month;
	int year;
} pseudo_time_t;


void initTime();
void stopTime();
void resetTime();

void setDay(int day);
void setMonth(int month);
void setYear(int year);
void setDate(int day, int month, int year);

int getDay();
int getMonth();
int getYear();
int getMaxDay();

void *get_attr(pseudo_time_t, int field);


#endif
