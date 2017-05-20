#include "files.h"

void ReadFiles(char *trip_file, char *station_file)
{
	char line[MAX_LONG_STRING];


	trip_node *trips, *trip_aux, *trip_current;

	trips = (trip_node*) malloc(sizeof(trip_node*));

	trips->next = NULL;

	trip_aux = trips;


	station_node *stations, *station_aux, *station_current;

	stations = (station_node*) malloc(sizeof(station_node*));

	stations->next = NULL;

	station_aux = stations;



	FILE *ftrip, *fstation;

	ftrip = fopen(trip_file, "r");



	while(fgets(line, MAX_LONG_STRING, ftrip) != NULL)
	{
		if(trip_current == NULL)
		{
			trip_current = (trip_node*) malloc(sizeof(trip_node*));

			trip_aux->next = trip_current;
		}

		trip_current = trip_current->next;

		trip_aux = trip_aux->next;
	}

	fclose(ftrip);

	fstation = fopen(station_file, "r");

	while(fgets(line, MAX_LONG_STRING, fstation) != NULL)
	{

	}

	fclose(fstation);
}