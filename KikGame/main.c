#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

char input = 'O'; // Definiujemy elementy tablicy
char player = 'X';

int arraySize; // minimalny rozmiar tablicy to 3 x 3

int returnArray(char storage[arraySize][arraySize]) { // Funkcja wyśiwetlania tablicy (na żądanie)
    for (int y = 0; y < arraySize; y++) {
        for (int z = 0; z < arraySize; z++) {
            printf("| %c |", storage[y][z]);
        }
        printf("\n");
        printf("\n");
    }
    return 0;
}

void addToArray(char storage[arraySize][arraySize], char input, int x, int y) { // Funkcja dodawania do tablicy
    storage[x][y] = input;
}

void initialState(char storage[arraySize][arraySize]) { // Stan początkowy (czysta tablica)
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            addToArray(storage, ' ', i, j);
        }
    }
}

int randomA(int max) { // Tworzenie liczby losowej w zakresie tablicy
    return rand() % max;
}

void checkFreeSpace(int x, int y, char storage[arraySize][arraySize]) { // Sprawdzenie czy miejsce wylosowane przez komputer jest dostępne, jeśli nie to wywołaj siebie (tę funkcję jeszcze raz)
    if (storage[x][y] == ' ') {
        addToArray(storage, input, x, y);
    } else {
        int A = randomA(arraySize);
        int B = randomA(arraySize);
        checkFreeSpace(A, B, storage);
    }
}

int flipCoin(void) { //Wybor kto zaczyna pierwszy
    return randomA(2);
}

void playerMove(char storage[arraySize][arraySize]) { // Zapytanie gracza o podanie współrzędnych i sprawdzenie czy są puste
    printf("Select your row:");
    int row, column;
    scanf("%i", &row);
    row -= 1;
    printf("Select your column:");
    scanf("%i", &column);
    column -= 1;
    if (row < 0 || column < 0 || row > arraySize || column > arraySize) {
        printf("You've entered value out of range, try again.\n");
        playerMove(storage);
    } else {
        char check = storage[row][column];
        if (check == ' ') {
            addToArray(storage, player, row , column);
        } else {
            printf("Don't cheat :)\n");
            playerMove(storage);
        }
    }
}

int checkResult(char storage[arraySize][arraySize]) { // Sprawdzenie czy wogóle a jeśli tak to kto wygrał
    int output = 2;
    for (int i = 0; i < arraySize - 2; i++) {
        for (int j = 0; j < arraySize - 2; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i+1][j+1] && storage[i][j] == storage[i+2][j+2]) {
                    if (storage[i][j] == 'O') {
                        output = 0;
                    } else if (storage[i][j] == 'X') {
                        output = 1;
                    }
                }
            }
        }
    }
    
    for (int i = arraySize - 1; i > 1; i--) {
        for (int j = 0; j < arraySize - 2; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i-1][j+1] && storage[i][j] == storage[i-2][j+2]) {
                    if (storage[i][j] == 'O') {
                        output = 0;
                    } else if (storage[i][j] == 'X') {
                        output = 1;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < arraySize - 2; i++) {
        for (int j = 0; j < arraySize; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i+1][j] && storage[i][j] == storage[i+2][j]) {
                    if (storage[i][j] == 'O') {
                        output = 0;
                    } else if (storage[i][j] == 'X') {
                        output = 1;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize - 2; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i][j+1] && storage[i][j] == storage[i][j+2]) {
                    if (storage[i][j] == 'O') {
                        output = 0;
                    } else if (storage[i][j] == 'X') {
                        output = 1;
                    }
                }
            }
        }
    }
    return output;
}

int checkFinal(char storage[arraySize][arraySize]) { // tu sprawdzamy czy wszystkie pola sa zapelnione (np w przypadku gdy jest remis)
    int output = 2;
    for (int y = 0; y < arraySize; y++) {
        for (int z = 0; z < arraySize; z++) {
            if (storage[y][z] == ' ') {
                output = 0;
                goto out;
            }
        }
    }
out:
    return output;
}

int checkMe(int x, int y, char storage[arraySize][arraySize]) { // Ta funkcja sprawdza czy jest mozliwosc zablokowania gracza
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x >= arraySize) {
        x = arraySize - 1;
    }
    if (y >= arraySize) {
        y = arraySize - 1;
    }
    int ret = 0;
    char check = storage[x][y];
    if (check == ' ') {
        ret = 1;
        addToArray(storage, input, x, y);
    } else {
        ret = 0;
    }
    return ret;
}

void func(char storage[arraySize][arraySize]) { // Ta funkcja sprawdza czy istnieje ryzyko wygrania przez gracza a nastepnie wywoluje funkcje powyzej aby go zablowac jak sie da
    int add = 1;
    for (int i = 0; i < arraySize - 1; i++) {
        for (int j = 0; j < arraySize - 1; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i+1][j+1]) {
                    if (storage[i][j] == 'X') {
                        if (checkMe(i-1, j-1, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else if (checkMe(i+2, j+2, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else { add = 1;}
                    }
                }
            }
        }
    }
    for (int i = arraySize - 1; i > 0; i--) {
        for (int j = 0; j < arraySize - 1; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i-1][j+1]) {
                    if (storage[i][j] == 'X') {
                        if (checkMe(i+1, j-1, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else if (checkMe(i-2, j+2, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else { add = 1;}
                    }
                }
            }
        }
    }
    for (int i = 0; i < arraySize - 1; i++) {
        for (int j = 0; j < arraySize; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i+1][j]) {
                    if (storage[i][j] == 'X') {
                        if (checkMe(i-1, j, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else if (checkMe(i+2, j, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else { add = 1;}
                    }
                }
            }
        }
    }
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize - 1; j++) {
            if (storage[i][j] != ' ') {
                if (storage[i][j] == storage[i][j+1]) {
                    if (storage[i][j] == 'X') {
                        if (checkMe(i, j-1, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else if (checkMe(i, j+2, storage) == 1) {
                            add = 0;
                            goto foundit;
                        } else { add = 1;}
                        
                    }
                }
            }
        }
    }
foundit:
    if (add == 1) {
        int A = randomA(arraySize);
        int B = randomA(arraySize);
        checkFreeSpace(A, B, storage);
    }
}

int main(int argc, const char * argv[]) {
    char run = 'Y';     // Te zmienne potrzebne sa w przypadku checi kontynuowania gry
    int flip;     // Pomocnicza zmienna do wywołania funkcji losowej aby wybrać kto zaczyna
    srand(time(NULL));
    do {
        printf("Let's start. Insert number of rows/columns. Anything less than 3 or more than 10 won't be accepted: ");
        scanf(" %i", &arraySize);
    } while (arraySize < 3 || arraySize > 10);
    flip = flipCoin();
    char storage[arraySize][arraySize];
    initialState(storage);
    printf("%c", returnArray(storage));
    do {
        while(checkFinal(storage) == 0) {
            if (flip == 0) {
                printf("My move\n");
                printf("\n");
                func(storage);
            } else {
                playerMove(storage);
            }
            printf("%c", returnArray(storage));
            if (checkResult(storage) != 2) {goto endline;}
            if (checkFinal(storage) == 2) {goto endline;}
            if (flip == 1) {
                printf("My move\n");
                printf("\n");
                func(storage);
            } else {
                playerMove(storage);
            }
            printf("%c", returnArray(storage));
            if (checkResult(storage) != 2) {goto endline;}
            if (checkFinal(storage) == 2) {goto endline;}
        }
    endline:
        if (checkResult(storage) == 1) {
            printf("Lucky You ;)\n");
        } else if ((checkResult(storage) == 0)) {
            printf("I won !\n");
        }
        printf("Do you want to play again (Y/N) ?:");
        scanf(" %c", &run);
        if (run == 'Y') {
            initialState(storage);
            printf("%c", returnArray(storage));
            flip = flipCoin();
        } else if (run == 'N') {
        } else {
            printf("Didn't get your answer, say again?\n");
        }
    } while(run == 'Y');
    return 0;
}
