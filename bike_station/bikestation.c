#include "utilities.h"
#include "textual.h"
#include "graphical.h"
#include "files.h"





int main(int argc, char *argv[])
{
	int option;
	trip_node *headtrip = NULL;
	station_node *headstation = NULL;

	headtrip = TripFiles(argv[2]);
	headstation = StationFiles(argv[3]);

	if(strcmp(argv[1], "-t") == 0) 
	{
		Textual(headtrip, headstation);
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








