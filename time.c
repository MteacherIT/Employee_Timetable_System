#include "time.h"

time_t time;

int main(){
	initTime(time);
	return 0;
}

void initTime(time_t time)
{
	if (time == null) {
		time.day = 0;
		time.month = 0;
		time.year = 0;
		time.hour = 0;
		time.minutes = 0;
		time.seconds = 0;
		// note sure will check it out later. TODO: discuss the format.
		time_t.time = scanf("%d/%d/%d clock: %d:%d:%d", time.day, time.month, time.year,
												        time.hour, time.minutes, time.seconds);

	}
	// TODO: counter..
	return;
}

void stopTime(){}
void resetTime(){}

void setDay(int day){ time_t.day = day; }
void setMonth(int month){ time_t.month = month; }
void setYear(int year){ time_t.year = year; }

void setDate(int day, int month, int year)
{
	time_t.day = day;
	time_t.month = month;
	time_t.year = year;
}

int getDay() { return time.day; }
int getMonth() { return time.month; }
int getYear() { return time.year; }
int getMaxDay() { /* ??? */ }

// TODO: useful method to return specific field.
void *get_field(struct time_t *time, int field)
{
    switch(field) {
		case 0 : return (void*)&time->time;
		case 1 : return (void*)&time->hour;
		case 2 : return (void*)&time->minutes;
		case 3 : return (void*)&time->seconds;
		case 4 : return (void*)&time->day;
		case 5 : return (void*)&time->month;
		case 6 : return (void*)&time->year;
    }
    return NULL;
}
