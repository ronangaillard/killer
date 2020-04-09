#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>

#define MAX_PLAYER_SIZE     10
#define MAX_PLAYER_NAME_SIZE    20
#define MAX_FORFEIT_SIZE        200
#define MAX_FORFEIT_TEXT_SIZE   200

void shuffle(int *array, size_t n);

int main() {
    char players[MAX_PLAYER_SIZE][MAX_PLAYER_NAME_SIZE];
    char forfeits[MAX_FORFEIT_SIZE][MAX_FORFEIT_TEXT_SIZE];
    int playersVictim[MAX_PLAYER_SIZE + 1];
    int forfeitAssignment[MAX_FORFEIT_SIZE];;

    int i = 0;
    int numberOfPlayers = 0;
    int numberOfForfeits = 0;

    srand(time(NULL));

    FILE* file = fopen("names.txt", "r");

    while(fgets(players[i], MAX_PLAYER_NAME_SIZE, file)) {
        players[i][strlen(players[i]) - 1] = '\0';
        i++;
    }

    fclose(file);

    numberOfPlayers = i;

    // Prepare players victim
    for(i = 0; i < numberOfPlayers; i++) {
        playersVictim[i] = i;
    }

    // Shuffle the array to assign victims
    shuffle(playersVictim, numberOfPlayers);
    playersVictim[numberOfPlayers] = playersVictim[0]; // Circular victims 

    i = 0;

    file = fopen("forfeits.txt", "r");

    while(fgets(forfeits[i], MAX_FORFEIT_TEXT_SIZE, file)) {
        forfeits[i][strlen(forfeits[i]) - 1] = '\0';
        i++;
    }

    fclose(file);

    numberOfForfeits = i;

    // Prepare forfeitAssignment array
    for(i = 0; i < numberOfForfeits; i++) {
        forfeitAssignment[i] = i;
    }

    // Shuffle the array to randomize forfeits
    shuffle(forfeitAssignment, numberOfForfeits);

    // Save game rules to txt file
    file = fopen("lastgameinfo.txt", "w");

    for(i = 0; i < numberOfPlayers; i++) {
        fprintf(file, "%s must kill %s\n", players[playersVictim[i]], players[playersVictim[i + 1]]);
        fprintf(file, "%s must [ %s ] in order to kill\n", players[playersVictim[i]], forfeits[forfeitAssignment[playersVictim[i]]]);
    }

    fclose(file);

    int playerNumber = 0;

    while(playerNumber != -1) {
        system("clear");

        printf("Indiquez votre numero de joueur :\n\n");

        for(i = 0; i < numberOfPlayers; i++) {
            printf("%d. %s\n", i, players[i]);
        }

        printf("\nVotre numero : ");
        scanf("%d", &playerNumber);

        if(playerNumber == -1) {
            break;
        }

        // Find victim
        i = 0;
        int victimId = -1;
        while(victimId == -1) {
            if(playersVictim[i] == playerNumber) {
                victimId = playersVictim[i + 1];
            }
            i++;
        }

        printf("\n\nBonjour %s, vous devez killer %s !\nPour cela il vous faudra effectuer le gage suivant : \n\n%s\n\nN'oubliez pas d'appuyer sur Entree pour effacer cet ecran", players[playerNumber], players[victimId], forfeits[forfeitAssignment[playerNumber]]);
        getchar();
        getchar();

    }
}

void shuffle(int *array, size_t n)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand48(usec);


    if (n > 1) {
        size_t i;
        for (i = n - 1; i > 0; i--) {
            size_t j = (unsigned int) (drand48()*(i+1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
