#include "utilities.h"
#include "textual.h"
#include "graphical.h"
#include "files.h"





int main(int argc, char *argv[])
{
	int option, trip_count = 0, station_count = 0, max_id = 0;
	trip_node *headtrip = NULL;
	station_node *headstation = NULL;

	headtrip = TripFiles(argv[2], &trip_count);
	headstation = StationFiles(argv[3], &station_count, &max_id);

	if(strcmp(argv[1], "-t") == 0) 
	{
		Textual(headtrip, headstation, trip_count, station_count, max_id);
	}

	else if(strcmp(argv[1], "-g") == 0) 
	{
		Graphical();
	}

	else
	{
		printf("Método de vizualização inválido\n");
		return 1;
	}

	FreeLists(headtrip, headstation);
	//Ask(&option);

	return 0;
}








