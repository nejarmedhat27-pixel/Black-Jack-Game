#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CARDS 5
#define MAX_PLAYERS 7
#define MAX_ROUNDS 5
#define NUM_SWAPS 200

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void initializeDeck(int *deck) {
    int index = 0;
    for (int value = 2; value <= 14; value++) {
        for (int count = 0; count < 4; count++) {
            deck[index++] = value;
        }
    }
}

void shuffleDeck(int *deck) {

    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void printHand(int hand[], int handSize) {
    for (int i = 0; i < handSize; i++) {
        if (hand[i] >= 2 && hand[i] <= 10) {
            printf("%d ", hand[i]);
        } else {
            switch (hand[i]) {
                case 11: printf("J ");
                 break;
                case 12: printf("Q ");
                 break;
                case 13: printf("K ");
                 break;
                case 14: printf("A ");
                 break;
                default: printf("? ");
                 break;
            }
        }
    }
}

int calculateHandValue(int hand[], int handSize) {
    int value = 0, aces = 0;
    for (int i = 0; i < handSize; i++) {
        if (hand[i] >= 2 && hand[i] <= 10) value += hand[i];
        else if (hand[i] >= 11 && hand[i] <= 13) value += 10;
        else if (hand[i] == 14) {
            value += 11;
            aces++;
        }
    }
    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    return value;
}

void printPlayerStatus(char name[], int hand[], int handSize) {
    printf("%s's hand: ", name);
    printHand(hand, handSize);
    printf(" (Total: %d)\n", calculateHandValue(hand, handSize));
}

int isBlackjack(int hand[], int handSize) {
    if (handSize != 2) return 0;
    int hasAce = 0, hasTenValue = 0;
    for (int i = 0; i < handSize; i++) {
        if (hand[i] == 14) hasAce = 1;
        if (hand[i] == 10 || (hand[i] >= 11 && hand[i] <= 13)) hasTenValue = 1;
    }
    return hasAce && hasTenValue;
}

int isBusted(int hand[], int handSize) {
    return calculateHandValue(hand, handSize) > 21;
}

void resetHands(int hands[][MAX_CARDS], int handSize[], int dealerHand[], int *dealerHandSize) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        handSize[i] = 0;
        for (int j = 0; j < MAX_CARDS; j++) hands[i][j] = 0;
    }
    *dealerHandSize = 0;
    for (int j = 0; j < MAX_CARDS; j++) dealerHand[j] = 0;
}

void dealInitialCards(int hands[][MAX_CARDS], int handSize[], int *deck, int *deckIndex, int numPlayers) {
    for (int i = 0; i < numPlayers; i++) {
        hands[i][0] = deck[(*deckIndex)++];
        hands[i][1] = deck[(*deckIndex)++];
        handSize[i] = 2;
    }
}

void dealerDraw(int *dealerHand, int *dealerHandSize, int *deck, int *deckIndex) {
    while (calculateHandValue(dealerHand, *dealerHandSize) < 17 && *dealerHandSize < MAX_CARDS) {
        dealerHand[(*dealerHandSize)++] = deck[(*deckIndex)++];
    }
}

void saveStatsToFile(char playerNames[][20], int wins[], int losses[], int ties[], int numPlayers) {
    FILE *file = fopen("playerStats.txt", "a");
    if (!file) {
        printf("Error saving stats.\n");
        return;
    }
    fprintf(file, "Final Scores:\n");
    for (int i = 0; i < numPlayers; i++) {
        fprintf(file, "%s - Wins: %d, Losses: %d, Ties: %d\n", playerNames[i], wins[i], losses[i], ties[i]);
    }
    fprintf(file, "----------------------------\n");
    fclose(file);
}

void displayStats(char playerNames[][20], int wins[], int losses[], int ties[], int numPlayers) {
    printf("\nCurrent Stats:\n");
    for (int i = 0; i < numPlayers; i++) {
        printf("%s - Wins: %d | Losses: %d | Ties: %d\n", playerNames[i], wins[i], losses[i], ties[i]);
    }
    printf("\n");
}

void playBlackjack() {
    char playerNames[MAX_PLAYERS][20];
    int playerWins[MAX_PLAYERS] = {0}, playerLosses[MAX_PLAYERS] = {0}, totalTies[MAX_PLAYERS] = {0};
    int hands[MAX_PLAYERS][MAX_CARDS] = {0}, handSize[MAX_PLAYERS] = {0};
    int dealerHand[MAX_CARDS] = {0}, dealerHandSize = 0;
    int deck[52], deckIndex = 0;
    int numPlayers, numRounds;

    srand(time(NULL));

    clearScreen();
    printf("Enter number of players (1-%d): ", MAX_PLAYERS);
    scanf("%d", &numPlayers);
    while (numPlayers < 1 || numPlayers > MAX_PLAYERS) {
        printf("Invalid number of players. Try again: ");
        scanf("%d", &numPlayers);
    }

    printf("Enter number of rounds (1-%d): ", MAX_ROUNDS);
    scanf("%d", &numRounds);
    while (numRounds < 1 || numRounds > MAX_ROUNDS) {
        printf("Invalid number of rounds. Try again: ");
        scanf("%d", &numRounds);
    }

    for (int i = 0; i < numPlayers; i++) {
        printf("Enter name of player %d: ", i + 1);
        scanf("%19s", playerNames[i]);
    }

    for (int round = 0; round < numRounds; round++) {
        clearScreen();
        printf("\n--- ROUND %d ---\n", round + 1);
        initializeDeck(deck);
        shuffleDeck(deck);
        deckIndex = 0;
        resetHands(hands, handSize, dealerHand, &dealerHandSize);

        dealInitialCards(hands, handSize, deck, &deckIndex, numPlayers);
        dealerHand[0] = deck[deckIndex++];
        dealerHand[1] = deck[deckIndex++];
        dealerHandSize = 2;

        printf("Dealer shows: ");
        printHand(&dealerHand[0], 1);
        printf("\n");

        int dealerBlackjack = isBlackjack(dealerHand, dealerHandSize);
        int playersWithBlackjack[MAX_PLAYERS] = {0};

        for (int i = 0; i < numPlayers; i++) {
            printPlayerStatus(playerNames[i], hands[i], handSize[i]);
            if (isBlackjack(hands[i], handSize[i])) {
                playersWithBlackjack[i] = 1;
                printf(" -> Blackjack!\n");
            }
        }

        if (dealerBlackjack) {
            printf("Dealer has Blackjack!\n");
            for (int i = 0; i < numPlayers; i++) {
                if (playersWithBlackjack[i]) {
                    printf("%s ties with dealer.\n", playerNames[i]);
                    totalTies[i]++;
                } else {
                    printf("%s loses to dealer.\n", playerNames[i]);
                    playerLosses[i]++;
                }
            }
            displayStats(playerNames, playerWins, playerLosses, totalTies, numPlayers);
            printf("Press Enter to continue...");
            getchar(); getchar();
            continue;
        } else {
            for (int i = 0; i < numPlayers; i++) {
                if (playersWithBlackjack[i]) {
                    printf("%s wins with Blackjack!\n", playerNames[i]);
                    playerWins[i]++;
                }
            }
        }


        for (int i = 0; i < numPlayers; i++) {
            if (playersWithBlackjack[i]) continue;
            while (1) {
                clearScreen();
                printf("\n--- ROUND %d ---\n", round + 1);
                printf("--- %s's Turn ---\n", playerNames[i]);
                printPlayerStatus(playerNames[i], hands[i], handSize[i]);
                if (isBusted(hands[i], handSize[i])) {
                    printf("%s busted!\n", playerNames[i]);
                    playerLosses[i]++;
                    break;
                }
                char choice;
                int validInput = 0;
                while (!validInput) {
                    printf("Hit or Stand? (h/s): ");
                    scanf(" %c", &choice);
                    if (choice == 'h' || choice == 'H') {
                        validInput = 1;
                        if (handSize[i] < MAX_CARDS) {
                            hands[i][handSize[i]++] = deck[deckIndex++];
                        } else {
                            printf("Cannot draw more cards (max %d reached).\n", MAX_CARDS);
                            break;
                        }
                    } else if (choice == 's' || choice == 'S') {
                        validInput = 1;
                        break;
                    } else {
                        printf("Invalid input. Please enter 'h' or 's'.\n");
                    }
                }
                if (choice == 's' || choice == 'S') break;
            }


            displayStats(playerNames, playerWins, playerLosses, totalTies, numPlayers);
            printf("Press Enter to continue to next player...");
            getchar(); getchar();
        }

        dealerDraw(dealerHand, &dealerHandSize, deck, &deckIndex);
        int dealerValue = calculateHandValue(dealerHand, dealerHandSize);
        printf("Dealer's hand: ");
        printHand(dealerHand, dealerHandSize);
        printf(" (Total: %d)\n", dealerValue);

        for (int i = 0; i < numPlayers; i++) {
            if (playersWithBlackjack[i]) continue;
            int playerValue = calculateHandValue(hands[i], handSize[i]);
            if (playerValue > 21) {

            } else if (dealerValue > 21 || playerValue > dealerValue) {
                printf("%s wins!\n", playerNames[i]);
                playerWins[i]++;
            } else if (playerValue < dealerValue) {
                printf("%s loses.\n", playerNames[i]);
                playerLosses[i]++;
            } else {
                printf("%s ties with dealer.\n", playerNames[i]);
                totalTies[i]++;
            }
        }

        displayStats(playerNames, playerWins, playerLosses, totalTies, numPlayers);
        printf("Press Enter to continue...");
        getchar(); getchar();
    }

    clearScreen();
    printf("\nFINAL SCORES\n");
    displayStats(playerNames, playerWins, playerLosses, totalTies, numPlayers);
    saveStatsToFile(playerNames, playerWins, playerLosses, totalTies, numPlayers);
}

int main() {
    int menuChoice;
    while (1) {
        clearScreen();
        printf("\nWELCOME TO BLACKJACK\n");
        printf("\n1. Play Blackjack\n2. Exit\nChoose an option: ");
        scanf("%d", &menuChoice);
        switch (menuChoice) {
            case 1:
                playBlackjack();
                break;
            case 2:
                printf("Exiting game. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please enter 1 or 2.\n");
                break;
        }
    }
}