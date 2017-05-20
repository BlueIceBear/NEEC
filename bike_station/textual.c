#include "textual.h"


void Textual()
{
	int option = 0;

	while(option != -1)
	{
		//if(option != 0) scanf("%d", &option);

		switch(option)
		{
			case 0:
				MainMenu(&option);
				break;

			case 1:
				DataSelection();
				option = 0;
				break;

			case 2:
				TripListing();
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

void DataSelection()
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
			//TimePeriod();
			break;

		case 2:
			// code for day of the week selection
			//WeekDay();
			break;

		case 3:
			// code for duration selection
			//TripDuration();
			break;

		default:
			break;
	}
}

void TripListing()
{

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