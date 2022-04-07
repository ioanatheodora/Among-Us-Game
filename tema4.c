#include "utils.h"
#define SIZE 10
#define KILL_SENTENCE 100


void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

void *RotateMatrix(void *input)
{
	int **matrix, *size = input, i, j;
	char *aux = calloc(SIZE * *size, sizeof(char)),
	*buffer = calloc(SIZE * *size * *size, sizeof(char));
	matrix = calloc(*size, sizeof(int *));
	if(matrix)
	{
		for(i = 0; i < *size; i++){
			matrix[i] = calloc(*size, sizeof(int));
			
		}}
	   for(i = 0; i < *size; i++){//cream matricea cu elementele deja inversate
        for(j = 0; j  < *size; j++){
            if(i+1 < 10){
            matrix[i][j] = (*size - j) * 10 + i + 1;
            }else
            { if(i+1 < 100)
                matrix[i][j] = (*size - j) * 100 + i+1;
				else{
					matrix[i][j] = (*size - j) * 1000 + i + 1;
				}
            }
            
        }
    }
     for(i = 0; i < *size; i++){
        for(j = 0; j  < *size; j++){
			if(j == *size - 1)
				sprintf(aux, "%d", matrix[i][j]);
			else
          		 sprintf(aux, "%d ", matrix[i][j]);
		   strcat(buffer, aux);

        }
		if(i != *size - 1)
       		strcat(buffer, "\n");
    }
	for(i = 0; i < *size; i++)
		free(matrix[i]);
	free(matrix);
	free(aux);
	return (void *) buffer;
}

void *DecodeString(void *input)
{

	char *number;
	input = (char *) input;
	int sum = 0, n = 0;
    number = strtok (input, "_");
    while(number != NULL){
        sum += atoi(number);
        number = strtok(NULL, "_");
        n++;
    }
    char *res = calloc(SIZE, sizeof(char));
    sprintf(res, "%d", sum);
    return (void *) res;
}


void *InvertArray(void *input)
{
	int *vector = input;
	char *aux, *result;
	aux = calloc(SIZE , sizeof(char));
	result = calloc(6 * vector[0], sizeof(char));
    if(vector[0]%2 == 0){
        for(int i=1; i <= vector[0]; i=i+2){
            int temp;
            temp = vector[i];
            vector[i] = vector[i+1];
            vector[i+1] = temp;
        }
    }else{
        for(int i = 1; i <= vector[0]/2; i++){
            int temp;
            temp = vector[vector[0] - i +1];
            vector[vector[0] - i +1] = vector [i];
            vector[i] = temp;
        }
    }
	for(int i = 1; i <= vector[0]; i++){
		if(i == vector[0])
			sprintf(aux, "%d", vector[i]);
		else
			sprintf(aux, "%d ", vector[i]);
		strcat(result, aux);
	}
	free(aux);
	return (void *)result;
}

Player *allocPlayer()
{
	Player* ptr = NULL;
	ptr =(Player *) calloc(1, sizeof( Player ));
	ptr -> name = calloc(MAX_LEN_STR_ATR, sizeof(char));
	ptr -> color = calloc(MAX_LEN_STR_ATR, sizeof(char));
	ptr -> hat = calloc(MAX_LEN_STR_ATR, sizeof(char));
	ptr -> alive = 1;
	return ptr;
}

Game *allocGame()
{
	Game *ptr = NULL;
	ptr = calloc(1, sizeof(Game ));
	ptr -> name = calloc(MAX_LEN_STR_ATR, sizeof(char));
	return ptr;
}

Player *ReadPlayer(FILE *inputFile)
{
	Player *ptr;
	ptr = allocPlayer();
	fscanf(inputFile, "%s", ptr -> name);
	fscanf(inputFile, "%s", ptr -> color);
	fscanf(inputFile, "%s", ptr -> hat);
	fscanf(inputFile, "%d\n", &(ptr -> numberOfLocations));
	ptr -> locations = (Location *)calloc(ptr -> numberOfLocations, sizeof(Location));
	for(int i = 0 ; i < ptr -> numberOfLocations ; i ++){
		fscanf(inputFile,"(%d, %d) ", &(ptr -> locations[i]).x, &(ptr -> locations[i]).y);
    }
	char *str = calloc(SIZE, sizeof(char));
	fscanf(inputFile, "%s", str);
	for (PlayerRoles i = Rotator; i <= Impostor; i++){
		char *role = fromEnumtoString(i);
		if(strcmp(role,str) == 0)
			{ptr -> playerRole = i;
			}
			
	free(role);
	}
	free(str);
	ptr -> ability = Abilities[ ptr -> playerRole ];
	return ptr;

}

Game *ReadGame(FILE *inputFile)
{
	Game *ptr;
	ptr = allocGame();
	fscanf(inputFile, "%s", ptr -> name);
	fscanf(inputFile, "%d", &ptr -> killRange);
	fscanf(inputFile, "%d", &ptr -> numberOfCrewmates);
	ptr -> crewmates = calloc(ptr -> numberOfCrewmates, sizeof(Player *));
	for(int i = 0; i < ptr -> numberOfCrewmates; i++)
	{
		ptr -> crewmates[i] = ReadPlayer(inputFile);
	}
	ptr -> impostor = ReadPlayer(inputFile);
	return ptr;
}

void WritePlayer(Player *player, FILE *outputFile)
{
	char *role = fromEnumtoString(player -> playerRole);
	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n",
	 player -> name, player -> color, player -> hat, role);
	 free(role);
	fprintf(outputFile,"Player's locations: ");
	for( int i = 0; i < player -> numberOfLocations; i++){
		fprintf(outputFile, "(%d,%d) ", (player -> locations[i]).x, (player -> locations[i]).y);
	}
	fprintf(outputFile, "\n");
	return;
}

void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game->numberOfCrewmates);
	fprintf(outputFile, "\tCrewmates:\n");
	for(int i = 0; i < game -> numberOfCrewmates; i++){
		WritePlayer(game -> crewmates[i], outputFile);
	}
	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);
	return;
}


int distManhattan(Player *crew, Player *imp){
	int abs_x, abs_y;
	abs_x = imp -> locations[imp -> indexOfLocation].x - crew -> locations[crew -> indexOfLocation].x;
	abs_y = imp -> locations[imp -> indexOfLocation].y - crew -> locations[crew -> indexOfLocation].y;
	if(abs_x < 0)
		abs_x = -abs_x;
	if(abs_y < 0)
		abs_y = -abs_y;
	return (abs_x+abs_y);
}

void *KillPlayer(void *input)
{
	Game *input1 = input;
	
	int distance_crt, distance_min = 1000000, poz = -1;
	char *result = calloc(KILL_SENTENCE, sizeof(char));

	for(int i = 0; i < input1 -> numberOfCrewmates; i++){
		distance_crt = distManhattan(input1 -> crewmates[i], input1 -> impostor);
		if(distance_crt <= distance_min && input1 -> crewmates[i] -> alive){
			distance_min = distance_crt;
			poz = i;
		}
	}

	if(distance_min <= input1 -> killRange){
		sprintf(result, "Impostor %s has just killed crewmate %s from distance %d.",input1 -> impostor -> name,
		 input1 -> crewmates[poz] -> name, distance_min);
		input1 -> crewmates[poz] -> alive = 0;
		return (void *)result;
	}else{
		sprintf(result, "Impostor %s couldn't kill anybody.", input1 -> impostor -> name);
		return (void *)result;
	}

}


void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	for(int i = 0; i < game -> numberOfCrewmates; i++){
		if(game -> crewmates[i] -> alive){
			int index = (game -> crewmates[i] -> indexOfLocation + 1) % game -> crewmates[i] -> numberOfLocations;
			game -> crewmates [i] -> indexOfLocation = index;
			fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game -> crewmates[i] -> name,
			game -> crewmates[i] -> locations[index].x, game -> crewmates[i] -> locations[index].y);
			fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[i]->name);
			char *output = game -> crewmates[i] -> ability(inputMatrix[i]);
			fprintf(outputFile, "%s\n", output);
			free(output);
		}
	else{
		fprintf(outputFile,"Crewmate %s is dead.\n", game->crewmates[i]->name);
		}
	}
	int index = (game->impostor->indexOfLocation + 1)%game->impostor->numberOfLocations;
	game -> impostor -> indexOfLocation = index;
	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", game->impostor->name,
	 game->impostor->locations[index].x, game->impostor->locations[index].y);
	fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);
	char *res;
	res = (char *)KillPlayer((void *)game);
	fprintf(outputFile, "%s\n", res);
	free(res);
	return;
}

void FreePlayer(Player *player)
{
	free(player -> name);
	free(player -> color);
	free(player -> hat);
	free(player -> locations);
	free(player);
	return;
}

void FreeGame(Game *game)
{
	free(game -> name);
	for(int i = 0; i < game -> numberOfCrewmates; i++){
		FreePlayer(game -> crewmates[i]);
	}
	free(game -> crewmates);
	FreePlayer(game -> impostor);
	free(game);
	return;
}