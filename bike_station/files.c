#include "files.h"



trip_node* TripFiles(char *trip_file)
{
	char line[MAX_LONG_STRING], *str_aux;

	int count;


	trip_node *trips = NULL, *trip_current = NULL, *trip_aux = NULL;

	/*trips = (trip_node*) malloc(sizeof(trip_node));	// alocation of dummy head node

	if(trip_current == NULL)
	{
		printf("Erro na alocação de memória.\n");
		exit(EXIT_FAILURE);
	}

	trips->next = NULL;*/

	//trip_current = trips->next;



	FILE *ftrip = NULL;

	ftrip = fopen(trip_file,"r");

	if(ftrip == NULL)
	{
		printf("Erro a abrir ficheiro das viagens.\n");
		exit(EXIT_FAILURE);
	}

	while(fgets(line, MAX_LONG_STRING, ftrip) != NULL)
	{
		
		trip_current = (trip_node*) malloc(sizeof(trip_node));

		if(trip_current == NULL)
		{
			printf("Erro na alocação de memória.\n");
			exit(EXIT_FAILURE);
		}

		trip_current->next = NULL;



		if(trips == NULL) 
		{
			trips = trip_current;

			trip_aux = trip_current;
		}
		else
		{
			trip_aux->next = trip_current;

			trip_aux = trip_current;
		}


		str_aux = strtok(line, ",");
		sscanf(str_aux,"%d", &trip_current->payload.tripID);

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%d", &trip_current->payload.duration);

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%d/%d/%d %d:%d", &trip_current->payload.start_date.month, &trip_current->payload.start_date.day, &trip_current->payload.start_date.year, &trip_current->payload.start_time.hours, &trip_current->payload.start_time.minutes);

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%d", &trip_current->payload.start_stationID);

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%d/%d/%d %d:%d", &trip_current->payload.end_date.month, &trip_current->payload.end_date.day, &trip_current->payload.end_date.year, &trip_current->payload.end_time.hours, &trip_current->payload.end_time.minutes);

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%d", &trip_current->payload.end_stationID);

		str_aux = strtok(NULL, ",");

		if(strcmp(str_aux,"Casual") != 0 && strcmp(str_aux, "Registered") != 0)
		{
			strcpy(trip_current->payload.bikeID, str_aux);

			str_aux = strtok(NULL, ",");
		}
		else
		{
			trip_current->payload.bikeID[0] = '\0';
		}

		
		if(strcmp(str_aux,"Casual") == 0)
		{
			trip_current->payload.user.membership = 0;
		}
		else 
		{
			trip_current->payload.user.membership = 1;

			str_aux = strtok(NULL, ",");
			sscanf(str_aux,"%d", &trip_current->payload.user.birth);

			str_aux = strtok(NULL, "\n");
			if(strcmp(str_aux,"Male") == 0) trip_current->payload.user.gender = 0;
			else trip_current->payload.user.gender = 1;
		}


		trip_current = trip_current->next;


		count++;
	}

	fclose(ftrip);

	return trips;
}

station_node* StationFiles(char *station_file)
{
	char line[MAX_LONG_STRING], *str_aux;

	int count = 0;

	station_node *stations = NULL, *station_current = NULL, *station_aux = NULL;


	FILE *fstation;

	fstation = fopen(station_file,"r");

	if(fstation == NULL)
	{
		printf("Erro a abrir ficheiro das viagens.\n");
		exit(EXIT_FAILURE);
	}


	fgets(line, MAX_LONG_STRING, fstation);


	while(fgets(line, MAX_LONG_STRING, fstation) != NULL)
	{
		
		station_current = (station_node*) malloc(sizeof(station_node));

		if(station_current == NULL)
		{
			printf("Erro na alocação de memória.\n");
			exit(EXIT_FAILURE);
		}


		station_current->next = NULL;

		if(stations == NULL) 
		{
			stations = station_current;

			station_aux = station_current;
		}
		else
		{
			station_aux->next = station_current;

			station_aux = station_current;
		}


		str_aux = strtok(line, ",");
		sscanf(str_aux,"%d", &station_current->payload.stationID);

		str_aux = strtok(NULL, ",");
		strcpy(station_current->payload.terminal, str_aux);

		str_aux = strtok(NULL, ",");
		strcpy(station_current->payload.long_name, str_aux);

		str_aux = strtok(NULL, ",");
		strcpy(station_current->payload.municipal, str_aux);

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%f", &station_current->payload.place.latitude); 

		str_aux = strtok(NULL, ",");
		sscanf(str_aux,"%f", &station_current->payload.place.longitude);

		str_aux = strtok(NULL, "\n");
		
		if(strcmp(str_aux,"Existing") == 0) station_current->payload.status = 0;
		else station_current->payload.status = 1;


		station_current = station_current->next;

		count++;
	}

	fclose(fstation);

	return stations;
}




