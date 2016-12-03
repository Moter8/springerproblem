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
char strings[99999] = "Anfang: ";

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
    // currentX+x >= 0 && currentX+x <= sizeX-1 && currentY+y >= 0 && currentY+y <= sizeY-1
    if (posX < sizeX && posY < sizeY && posX >= 0 && posY >= 0) {
        int pos = *(board + posY*sizeY + posX);
        return *(board + posY*sizeY + posX);
    } else {
        return true;   
    }
}

void setFieldVal(int posX, int posY, bool val) {
    *(board + posY*sizeY + posX) = val;
}

bool checkFieldVal(int posX, int posY, int numb) {
    if (!getFieldVal(posX, posY)) {
        return simpleBackTracking(posX, posY, numb + 1);
    } else {
        return 0;
    }
}

// Je nachdem welche Nummer man mitgibt, 
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
}

bool simpleBackTracking(int posX, int posY, int numb) {
    setFieldVal(posX, posY, true);
    //printf("%d %d %d\n",posX, posY, numb);
    
    if (numb == sizeX * sizeY) {
                    
        char buf[5];
        sprintf(buf, "(%d,%d) ", posX+1, posY+1);
        strcat(strings, buf);
        
        return 1;
    }
    
    for (int i = 0; i <= 7; i++) {
        if (checkFieldValNumb(posX, posY, i, numb)) {
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
    
/*
    printf("Board Size (x): ");
    scanf("%d", &sizeX);
    
    printf("Board Size (y): ");
    scanf("%d", &sizeY);
    
    printf("Initial X Position: ");
    scanf("%d", &initialX);
    
    printf("Initial Y Position: ");
    scanf("%d", &initialY);
*/

    sizeX = 7;
    sizeY = 7;
    initialX = 3;
    initialY = 1;

    printf("Board Size: %dx%d, Initial X: %d Initial Y: %d\n", sizeX, sizeY, initialX, initialY);
    
    board = (int *)malloc(sizeX * sizeY * sizeof(int));
    
    *(board + initialY*sizeY + initialX) = 1;
    //printBoard(board, sizeX, sizeY);
    
    printf("%d\n", simpleBackTracking(initialX, initialY, 1));
    printBoard(board, sizeX, sizeY);
    printf("%d",getFieldVal(7,7));
    printf("\n%s", strings);


    // WOW
    return 0;
}
