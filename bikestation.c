#include "utilities.h"
#include "textual.h"
#include "graphical.h"
#include "files.h"


void Graphical();


int main(int argc, char *argv[])
{
	int option;
	trip_node *head_trip;
	station_node *head_station;

	if(strcmp(argv[1], "-t") == 0) 
	{
		trips = ReadFiles(argv[2], argv[3]);	
		Textual();
	}

	else if(strcmp(argv[1], "-g") == 0) 
	{
		stations = ReadFiles(argv[2], argv[3])
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









void Graphical()
{

}