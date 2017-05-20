#ifndef _ESTRUTURAS_H_
#define _ESTRUTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_STRING 100
#define MAX_LONG_STRING 1000

typedef struct time_val{
	int hours;
	int minutes;
}time_val;

typedef struct coordenates{
	float latitude;
	float longitude;
}coordenates;

typedef struct date_val{
	int year;
	int month;
	int day;
}date_val;

struct user{
	int membership;		//0 = casual, 1 = member
	int birth;			//year of birth
	int gender;			//0 = male, 1 = female
};

typedef struct trip{
	int tripID;
	int duration;
	time_val start_time;
	time_val final_time;
	int start_station;
	int final_station;
	char bikeID[6];
	struct user user;
}trip;

typedef struct station{
	int stationID;
	char short_name[6];
	char long_name[MAX_STRING];
	char area[MAX_STRING];
	coordenates place;
	int status;			//if it exists, status = 0, if not, status = 1
}station;

typedef struct trip_node{
	trip s_payload;
	struct trip_node *next;
	//struct trip_node *prev;
}trip_node;

typedef struct station_node{
	station payload;
	struct station_node *next;
	//struct station_node *prev;
}station_node;

trip_node* InsertTail();

#endif