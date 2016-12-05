#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ANSI_COLOR_RED    "" //"\x1b[31m"
#define ANSI_COLOR_YELLOW "" // "\x1b[33m"
#define ANSI_COLOR_BLUE   "" //"\x1b[34m"
#define ANSI_COLOR_RESET  "" //"\x1b[0m"

int *board;
int sizeX, sizeY;
char strings[99999] = "Ergebnis: ";

bool getFieldVal(int posX, int posY);
void setFieldVal(int posX, int posY, bool val);
bool checkFieldVal(int posX, int posY, int numb);
bool checkFieldValNumb(int posX, int posY, int fieldNumber, int numb);
bool simpleBackTracking(int posX, int posY, int numb);

void printBoard() {
    
    for (int i = 0; i < sizeY; i++) {

        for (int j = 0; j < sizeX; j++) {
            
            if (*(board + i*sizeY + j) == 1) {
                printf(ANSI_COLOR_YELLOW     "@ "     ANSI_COLOR_RESET);
            } else {
                         if (i%2==0) {
                j++;
                if (j % 2 == 0) {
                    printf(ANSI_COLOR_RED     "x "     ANSI_COLOR_RESET);
                } else {
                    printf(ANSI_COLOR_BLUE     "x "     ANSI_COLOR_RESET);
                }
                j--;
            } else {
                if (j % 2 == 0) {
                    printf(ANSI_COLOR_RED     "x "     ANSI_COLOR_RESET);
                } else {
                    printf(ANSI_COLOR_BLUE     "x "     ANSI_COLOR_RESET);
                }
            }   
            }
        }
        printf("\n");
    }
    
}

// Gibt nur den Wert von einer bestimmten Position aus.
bool getFieldVal(int posX, int posY) {
    if (posX < sizeX && posY < sizeY && posX >= 0 && posY >= 0) {
        return *(board + posY*sizeY + posX);
    } else {
        return true;   
    }
}

void setFieldVal(int posX, int posY, bool val) {
    // board = 0x00000  + 1*sizeY (10) + 3 ~ 0x00013
    *(board + posY*sizeY + posX) = val;
}

// If Destionation X and Y aren't visited, execute simpleBackTracking to possible move to that location.
// Numb is used to keep track of ?
bool checkFieldVal(int posX, int posY, int numb) {
    if (!getFieldVal(posX, posY)) {
        return simpleBackTracking(posX, posY, numb + 1);
    } else {
        return false;
    }
}

// Possible moves will be checked depending on the fieldNumber.
bool checkFieldValNumb(int posX, int posY, int fieldNumber, int numb) {
    switch (fieldNumber) {
        case 0:
            return checkFieldVal(posX + 2, posY + 1, numb);
            break;
        case 1:
            return checkFieldVal(posX + 2, posY - 1, numb);
            break;
        case 2:
            return checkFieldVal(posX - 2, posY + 1, numb);
            break;
        case 3:
            return checkFieldVal(posX - 2, posY - 1, numb);
            break;
        case 4:
            return checkFieldVal(posX + 1, posY + 2, numb);
            break;
        case 5:
            return checkFieldVal(posX + 1, posY - 2, numb);
            break;
        case 6:
            return checkFieldVal(posX - 1, posY + 2, numb);
            break;
        case 7:
            return checkFieldVal(posX - 1, posY - 2, numb);
            break;
    }
    printf("ERROR\n");
    return 0;
}

// ?
bool simpleBackTracking(int posX, int posY, int numb) {
    setFieldVal(posX, posY, true);
    //printf("%d %d %d\n",posX, posY, numb);
    
    if (numb == sizeX * sizeY) {
                    
        char buf[5];
        sprintf(buf, "(%d,%d) ", posX+1, posY+1);
        strcat(strings, buf);
        
        return 1;
    }
    
    // Versucht alle möglichen Züge von aktuellem Feld auszuführen.
    for (int i = 0; i <= 7; i++) {
        if (checkFieldValNumb(posX, posY, i, numb)) {
            
            // Add coordinates to the steps string.
            char buf[5];
            sprintf(buf, "(%d,%d) ", posX+1, posY+1);
            strcat(strings, buf);
            return 1;
        }
    }
    
    setFieldVal(posX, posY, false);
    return 0;
}


int main() {
    
    int initialX, initialY;
    
    sizeX = 7;
    sizeY = 7;
    initialX = 0;
    initialY = 0;

    printf("Board Size (x): ");
    scanf("%d", &sizeX);
    
    printf("Board Size (y): ");
    scanf("%d", &sizeY);
    
    printf("Initial X Position: ");
    scanf("%d", &initialX);
    
    printf("Initial Y Position: ");
    scanf("%d", &initialY);




    printf("Board Size: %dx%d, Initial X: %d Initial Y: %d\n", sizeX, sizeY, initialX, initialY);
    
    board = (int *)calloc(sizeX * sizeY, sizeof(int));
    
    *(board + initialY*sizeY + initialX) = 1;
    //printBoard(board, sizeX, sizeY);
    
    printf("%d\n", simpleBackTracking(initialX, initialY, 1));
    printBoard();
    printf("%d", getFieldVal(7,7));
    printf("\n%s\n", strings);


    // WOW
    return 0;
}
