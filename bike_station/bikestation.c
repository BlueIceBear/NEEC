#include "utilities.h"
#include "textual.h"
#include "graphical.h"
#include "files.h"





int main(int argc, char *argv[])
{
	int option;
	trip_node *headtrip = NULL;
	station_node *headstation = NULL;

	if(strcmp(argv[1], "-t") == 0) 
	{
		headtrip = TripFiles(argv[2]);
		headstation = StationFiles(argv[3]);

		printf("%d\n", headstation->payload.stationID);
		Textual(headtrip, headstation);
	}

	else if(strcmp(argv[1], "-g") == 0) 
	{
		headtrip = TripFiles(argv[2]);
		headstation = StationFiles(argv[3]);
		Graphical();
	}

	else
	{
		printf("Método de vizualização inválido\n");
		return 1;
	}


	//Ask(&option);

	return 0;
}








