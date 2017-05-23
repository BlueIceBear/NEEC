#include "utilities.h"

int CalculateWday(int month, int day, int year)
{
	int wday, adjustment, month_aux, year_aux;
 
	adjustment = (14 - month) / 12;

	month_aux = month + 12 * adjustment - 2;

	year_aux = year - adjustment;

	return (day + (13 * month_aux - 1) / 5 + year_aux + year_aux / 4 - year_aux / 100 + year_aux / 400) % 7;
}

void FreeLists(trip_node *headtrip, station_node *headstation)
{
	trip_node *trip_aux = headtrip;
	station_node *station_aux = headstation;

	while(trip_aux != NULL)
	{
		trip_aux = trip_aux->next;

		free(headtrip);

		headtrip = trip_aux;
	}

	while(station_aux != NULL)
	{
		station_aux = station_aux->next;

		free(headstation);

		headstation = station_aux;
	}
}