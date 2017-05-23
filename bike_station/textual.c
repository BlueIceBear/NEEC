#include "textual.h"


void Textual(trip_node *headtrip, station_node *headstation)
{
	int option = 0;

	trip_node *copytrip = headtrip;
	station_node *copystation = headstation;

	printf("headtrip %d\n", headtrip->payload.tripID);

	printf("%d\n", headstation->payload.stationID);

	while(option != -1)
	{
		//if(option != 0) scanf("%d", &option);

		switch(option)
		{
			case 0:
				MainMenu(&option);
				break;

			case 1:
				DataSelection(&copytrip, &copystation);
				option = 0;
				break;

			case 2:
				TripListing(copytrip);
				option = 0;
				break;

			case 3:
				StationListing();
				option = 0;
				break;

			case 4:
				RouteListing();
				option = 0;
				break;

			case 5:
				StatsListing();
				option = 0;
				break;

			case 6:
				option = -1;
				break;

			default:
				break;
		}
	}
}

void MainMenu(int *option)
{
	int aux = 0;

	printf("Escolha a opção que deseja:\n\n");

	printf("\t1. Selecção de dados\n\t2. Listagem de viagens\n\t3. Listagem de estações\n\t4. Listagem de rotas\n\t5. Listagem de estatísticas\n\t6. Exit\n");

	while(*option < 1 || *option > 6)
	{
		if(aux == 1) printf("Escolha inválida\n");

		scanf("%d", &(*option));

		aux = 1;
	}
}

void DataSelection(trip_node **copytrip, station_node **copystation)
{
	int pri_option = 0, sec_option = 0, aux = 0;

	char selection[MAX_STRING];

	getchar();

	while(((sscanf(selection,"%d %d", &pri_option, &sec_option) != 2) || (pri_option < 1) || (pri_option > 4) || (sec_option < 0)) && (pri_option != 4)) 
	{
		if(aux == 1) printf("\nEscolha inválida\n\n");

		printf("Escolha o(s) parâmetros que deseja (insira 4 para sair):\n");
		printf("\t(escreva 'del <número do parâmetro>' para retirar esse parâmetro)\n\n");

		printf("\t1. Período de tempo:\n\t-> add: 1 <período de tempo>\n\n\t2. Dia da semana\n\t-> add: 2 <dia da semana>\n\n\t3. Duração\n\t-> add: 3 <duração>\n\n\t4. Exit to main menu\n");

		fgets(selection, MAX_STRING, stdin);

		aux = 1;
	}

	switch(pri_option)
	{
		case 1:
			// code for period of time selection
			TimePeriod(&(*copytrip), &(*copystation), sec_option);
			break;

		case 2:
			// code for day of the week selection
			WeekDay();
			break;

		case 3:
			// code for duration selection
			TripDuration();
			break;

		default:
			break;
	}
}

void TimePeriod(trip_node **copytrip, station_node **copystation, int parameter)
{
	//percorrer listas para copiar nós

	trip_node *T_head = NULL, *T_aux = NULL, *T_current = NULL;
	station_node *S_head = NULL, *S_aux = NULL, *S_current = NULL;

	printf("antes trip\n");
	while((*copytrip) != NULL)
	{
		printf("Ciclo\n");
		if(T_aux == NULL)
		{
			T_head = T_current;

			T_aux = T_current;
		}
		else
		{
			T_aux->next = T_current;

			T_aux = T_current;
		}
		printf("depois do else\n");
		T_current = (trip_node *) malloc(sizeof(trip_node));

		//copiar valores

		printf("copytrip %d\n", (*copytrip)->payload.tripID);

		T_current->payload.tripID = (*copytrip)->payload.tripID;

		printf("copytrip\n");

		T_current->payload.duration = (*copytrip)->payload.duration;


		T_current->payload.start_date.month = (*copytrip)->payload.start_date.month;

		T_current->payload.start_date.day = (*copytrip)->payload.start_date.day;

		T_current->payload.start_date.year = (*copytrip)->payload.start_date.year;


		T_current->payload.start_time.hours = (*copytrip)->payload.start_time.hours;

		T_current->payload.start_time.minutes = (*copytrip)->payload.start_time.minutes;

		
		T_current->payload.end_date.month = (*copytrip)->payload.end_date.month;

		T_current->payload.end_date.day = (*copytrip)->payload.end_date.day;

		T_current->payload.end_date.year = (*copytrip)->payload.end_date.year;


		T_current->payload.end_time.hours = (*copytrip)->payload.end_time.hours;

		T_current->payload.end_time.minutes = (*copytrip)->payload.end_time.minutes;


		T_current->payload.start_stationID = (*copytrip)->payload.start_stationID;

		T_current->payload.end_stationID = (*copytrip)->payload.end_stationID;


		strcpy(T_current->payload.bikeID, (*copytrip)->payload.bikeID);


		T_current->payload.user.membership = (*copytrip)->payload.user.membership;

		T_current->payload.user.birth = (*copytrip)->payload.user.birth;

		T_current->payload.user.gender = (*copytrip)->payload.user.gender;


		T_current->next = NULL;

		T_current = T_current->next;

		(*copytrip) = (*copytrip)->next;
	}

	//copytrip = T_head;

	printf("depois trip\n");



	while((*copystation) != NULL)
	{
		printf("Ciclo2\n");

		if(S_aux == NULL)
		{
			S_head = S_current;

			S_aux = S_current;
		}
		else
		{
			printf("bus?\n");

			S_aux->next = S_current;

			printf("bus?\n");

			S_aux = S_current;

			printf("else\n");
		}

		printf("antes de alocar\n");
		S_current = (station_node *) malloc(sizeof(station_node));

		//copiar valores
		printf("antes de payload %d\n", (*copystation)->payload.stationID);
		S_current->payload.stationID = (*copystation)->payload.stationID;

		printf("antes de strcpy\n");
		strcpy(S_current->payload.terminal, (*copystation)->payload.terminal);

		strcpy(S_current->payload.long_name, (*copystation)->payload.long_name);

		strcpy(S_current->payload.municipal, (*copystation)->payload.municipal);

		

		S_current->payload.place.latitude = (*copystation)->payload.place.latitude;

		S_current->payload.place.longitude = (*copystation)->payload.place.longitude;


		S_current->payload.status = (*copystation)->payload.status;



		S_current->next = NULL;

		S_current = S_current->next;

		printf("bus?\n");

		(*copystation) = (*copystation)->next;

		printf("bus?\n");
	}

	printf("depois station\n");
	//copystation = S_head;

}

void WeekDay()
{

}

void TripDuration()
{

}


void TripListing(trip_node *headtrip)
{
	char str_aux[MAX_STRING];

	int trip_num, mult = 0, i;

	trip_node *trips;

	trips = headtrip;

	printf("Quantas viagens a apresentar de cada vez?\n(é possível navegar pelas páginas inserindo \"n\" para ver a próxima, \"p\" para ver a anterior e \"exit\" para sair da listagem para o menu principal)\n");

	while(scanf("%d", &trip_num) != 1 && trip_num < 0)
	{
		printf("Número de viagens inválido.\n");
	}

	getchar();

	while(strcmp(str_aux, "exit\n") != 0)
	{

		for(i = 0; i < mult*trip_num; i++)
		{
			trips = trips->next;
		}

		for(i = 0; i < trip_num; i++)
		{
			if(trips != NULL) printf("%d\n", trips->payload.tripID);
			else printf("Está a NULL.\n");

			trips = trips->next;
		}

		fgets(str_aux, MAX_STRING, stdin);

		if(strcmp(str_aux,"n\n") == 0) mult++;
		if(strcmp(str_aux,"p\n") == 0 && mult > 0) mult--;

		trips = headtrip;
	}


}

void StationListing()
{

}

void RouteListing()
{

}

void StatsListing()
{

}


