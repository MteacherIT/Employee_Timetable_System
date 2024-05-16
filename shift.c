#include <shift.h>

typedef int bool;

struct shift{
	 int id;
	 int time;
	 int employeeID;
};

//adds user to shift
boolean occupy(int id);

//remove user from shift
boolean deoccupy(int id);


//checks if shift is occupied
boolean isOccupied();

int main(){
	return 0;
}