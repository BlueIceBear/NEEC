#include "utilities.h"
#include "textual.h"
#include "graphical.h"
#include "files.h"





int main(int argc, char *argv[])
{
	int option;
	trip_node *headtrip;
	station_node *head_station;

	if(strcmp(argv[1], "-t") == 0) 
	{
		headtrip = TripFiles(argv[2]);
		head_station = StationFiles(argv[3]);
		Textual(headtrip);
	}

	else if(strcmp(argv[1], "-g") == 0) 
	{
		headtrip = TripFiles(argv[2]);
		head_station = StationFiles(argv[3]);
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








