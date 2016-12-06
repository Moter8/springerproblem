#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ANSI_COLOR_RED    "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE   "\x1b[34m"
#define ANSI_COLOR_RESET  "\x1b[0m"

#define PIECE_ODD "\x1b[31mx \x1b[0m"
#define PIECE_EVEN "\x1b[34mo \x1b[0m"
#define PIECE_ACTIVE "\x1b[33m@ \x1b[0m"

// Beschreibung der Aufgabe: https://drive.google.com/file/d/0BzRp-cLiZDUJcWNUWDdVN3F3SjQ/view?usp=sharing

// TODO: 2 verschiedene Outputs anhand der Steps, refactor checkFieldValNumb to use coord structure
// MY_TYPE a = { .flag = true, .value = 123, .stuff = 0.456 };
// use coord everywhere instead of posX/posY? --> Nein
// rework PrintBoard method :]

int *board;
int sizeX, sizeY;

struct coord {
    int x;
    int y;
};

struct coord *steps;

bool getFieldVal(int posX, int posY);
void setFieldVal(int posX, int posY, bool val);
bool checkFieldVal(int posX, int posY, int numb);
bool checkFieldValNumb(int posX, int posY, int fieldNumber, int numb);
bool simpleBackTracking(int posX, int posY, int numb);
int promptForDigits(char prompt[]);
int promptForDigitsLimit(char prompt[], int upperLimit);
bool isBoardCompleted();

void printBoard() {
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            if (getFieldVal(i, j)) {
                printf(PIECE_ACTIVE);
            } else if((i + j)%2) {
                printf(PIECE_EVEN);
            } else {
                printf(PIECE_ODD);
            }
            //printf(getFieldVal(i, j) ? PIECE_ACTIVE : (i + j)%2 == 0 ? PIECE_EVEN : PIECE_ODD);
        }
        printf("\n");
    }
}
Rewrite printBoard(), 
bool isBoardCompleted() {
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
		    // not sure if (j,i) or (i, j)
			if (!getFieldVal(j, i)) {
				return false;
			}
		}
	}
	return true;
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

//ersetzt später checkFieldValNumb
struct coord getFieldByNumber(int initialX, int initialY, int fieldNumber) {
    struct coord result = {.x = initialX, .y = initialY};
    switch (fieldNumber) {
        case 0:
            result.x += 2;
            result.y += 1;
            break;
        case 1:
            result.x += 2;
            result.y -= 1;
            break;
        case 2:
            result.x -= 2;
            result.y += 1;
            break;
        case 3:
            result.x -= 2;
            result.y -= 1;
            break;
        case 4:
            result.x += 1;
            result.y += 2;
            break;
        case 5:
            result.x += 1;
            result.y -= 2;
            break;
        case 6:
            result.x -= 1;
            result.y += 2;
            break;
        case 7:
            result.x -= 1;
            result.y -= 2;
            break;
    }
    return result;
}

void addStepToSteps(int posX, int posY, int step) {
    // Steps can not be larger than the possible amount of steps.
    if (step < sizeX * sizeY) {
        steps[step].x = posX;
        steps[step].y = posY;
    } else {
        printf( ANSI_COLOR_RED "ERROR: Access to Steps out of bounds (Tried to access index %d but steps is only %d large)\n" ANSI_COLOR_RESET, step, sizeX*sizeY);
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
 
bool startSimpleBackTracking(int initialX, int initialY) {
    return simpleBackTracking(initialX, initialY, 0);
}

// Recursive algorithm for backTracking
bool simpleBackTracking(int posX, int posY, int numb) {
    setFieldVal(posX, posY, true);
    
    if (numb == (sizeX * sizeY - 1)) {
        
        addStepToSteps(posX, posY, numb);
        return 1;
    }
    
    // Versucht alle möglichen Züge vom aktuellem Feld auszuführen.
    for (int i = 0; i <= 7; i++) {
        if (checkFieldValNumb(posX, posY, i, numb)) {
            
            addStepToSteps(posX, posY, numb);
            return 1;
        }
    }
    
    setFieldVal(posX, posY, false);
    return 0;
}

//Flushes the input stream
void flushInputStream() {
    
}

// http://stackoverflow.com/a/27281028/3991578
void flushStdin(void) {
  int ch;
  while(((ch = getchar()) !='\n') && (ch != EOF));
}

// Prompt for a digit between 0 and the upper Limit variable
int promptForDigitsLimit(char prompt[], int upperLimit) {
    while(true) {
        int input;
        printf("%s: ", prompt);

        if (scanf("%d", &input) <= 0) {
            flushStdin();
            printf("Input must be a number.\n");
        } else if (input > upperLimit && upperLimit != -1) {
            printf("Number must be smaller than %d.\n", upperLimit + 1);
        } else if (input <= 0) {
            printf("Number must be greater than 0.\n");
        } else {
            return input;
        }
        flushInputStream();
    }
}

// Prompt for a digit larger than 0.
int promptForDigits(char prompt[]) {
    return promptForDigitsLimit(prompt, -1);
}

int main() {
    
    int initialX, initialY;
    
    sizeX = promptForDigits("Board Size (x)");
    sizeY = promptForDigits("Board Size (y)");
    initialX = promptForDigitsLimit("Initial X Position", sizeX);
    initialY = promptForDigitsLimit("Initial Y Position", sizeY);

    // One less to go from 1-indexed to 0-indexed counting
    initialX--;
    initialY--;
    
    // Allocate and 0-initialize board and steps arrays
    board = (int *)calloc(sizeX * sizeY, sizeof(int));
    steps = (struct coord *)calloc(sizeX * sizeY, sizeof(struct coord));
    
    setFieldVal(initialX, initialY, true);
    
    printf("\nStartfeld:\n");
    printBoard();

    // Setting the starting position to visited
    *(board + initialY * sizeY + initialX) = 1;
    
    // Starts main algorithm
    startSimpleBackTracking(initialX, initialY);
    
    
    if(isBoardCompleted()) {
        printf("A solution has been found!\n");
    } else {
        printf("No solution could be found!\n");
    }
    
    printf("\nErgebnisfeld:\n");
    printBoard();
    
    return 0;
}