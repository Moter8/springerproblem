#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
    #define ANSI_COLOR_GREEN  ""
    #define ANSI_COLOR_RED    ""
    #define ANSI_COLOR_RESET  ""
#else
    #define ANSI_COLOR_GREEN  "\x1b[32m"
    #define ANSI_COLOR_RED    "\x1b[31m"
    #define ANSI_COLOR_RESET  "\x1b[0m"
#endif

// Beschreibung der Aufgabe: https://drive.google.com/file/d/0BzRp-cLiZDUJcWNUWDdVN3F3SjQ/view?usp=sharing

typedef struct {
    int x;
    int y;
} coord;

typedef struct {
    coord position;
    int possibleSteps;
} extCoord;

bool *board;
int sizeX, sizeY;
int *steps;

const coord invalid = { .x = -1, .y = -1};

int lengthInt(int i) {
    int result = 0;
    while (i != 0 ) {
        result++;
        i /= 10;
    }
    return result;
}

void printSteps() {
    int stepsAmount = sizeX*sizeY;
    int magnitude = lengthInt(stepsAmount);
    
    char printfFormat[100] = "%0";
    char magnitudeString[10];
    sprintf(magnitudeString, "%d", magnitude);
    
    strcat(printfFormat, magnitudeString);
    strcat(printfFormat, "d  ");
    
    // --> printfFormat = "%0Xd\n" -->X is the number of digits that the largest step contains
    for(int x = 0; x < sizeX; x++) {
        for(int y = 0; y < sizeY; y++) {
            printf(printfFormat, *(steps + y*sizeX + x));
        }
        printf("\n");
        printf("\n");
    }

    printf("\n");
}


// Gibt nur den Wert von einer bestimmten Position aus.
bool getFieldVal(coord pos) {
    if (pos.x < sizeX && pos.y < sizeY && pos.x >= 0 && pos.y >= 0) {
        return *(board + pos.y*sizeX + pos.x);
    } else {
        return true;
    }
}

void setFieldVal(coord pos, bool val) {
    *(board + pos.y*sizeX + pos.x) = val;
}

coord getFieldByNumber(coord pos, int fieldNumber) {
    switch (fieldNumber) {
        case 0:
            pos.x += 2;
            pos.y += 1;
            break;
        case 1:
            pos.x += 2;
            pos.y -= 1;
            break;
        case 2:
            pos.x -= 2;
            pos.y += 1;
            break;
        case 3:
            pos.x -= 2;
            pos.y -= 1;
            break;
        case 4:
            pos.x += 1;
            pos.y += 2;
            break;
        case 5:
            pos.x += 1;
            pos.y -= 2;
            break;
        case 6:
            pos.x -= 1;
            pos.y += 2;
            break;
        case 7:
            pos.x -= 1;
            pos.y -= 2;
            break;
    }
    return pos;
}

void addStepToSteps(coord pos, int step) {
    // Steps can not be larger than the possible amount of steps.
    if (step < sizeX * sizeY) {
        *(steps + pos.y*sizeX + pos.x) = step;
        //steps[step] = pos;
    } else {
        printf( ANSI_COLOR_RED "ERROR: Access to Steps out of bounds (Tried to access index %d but steps is only %d large)\n" ANSI_COLOR_RESET, step, sizeX*sizeY);
    }
}

int countPossibleSteps(coord initial) {
    int result = 0;
    coord buffer;
    for (int i = 0; i < 8; i++) {
        buffer = getFieldByNumber(initial, i);
        if(!getFieldVal(buffer)) {
            result++;
        }
    }
    return result;
}

/*
 * Recursive algorithm for the Knights path
 */
bool warnsdorfBackTracking(coord pos, coord final, int numb) {
    setFieldVal(pos, true);

    if (numb == (sizeX * sizeY - 1)) {
    	for (int i = 0; i < 8; i++) {
    		coord buffer = getFieldByNumber(pos, i);
    		if ((buffer.x == final.x && buffer.y == final.y) || final.x == -1 || final.y == -1) {
    	        addStepToSteps(pos, numb);
    	        return true;
    		}
    	}
    	setFieldVal(pos, false);
    	return false;
    }

    extCoord followingSteps[8] =
    {
        {.possibleSteps = -1}, {.possibleSteps = -1},
        {.possibleSteps = -1}, {.possibleSteps = -1},
        {.possibleSteps = -1}, {.possibleSteps = -1},
        {.possibleSteps = -1}, {.possibleSteps = -1}
    };

    // Versucht alle möglichen Züge vom aktuellem Feld auszuführen.
    for (int i = 0; i < 8; i++) {
        coord buffer = getFieldByNumber(pos, i);

        if (!getFieldVal(buffer)) {
            followingSteps[i].possibleSteps = countPossibleSteps(buffer);
            followingSteps[i].position = buffer;
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (followingSteps[j].possibleSteps == i) {
                if (warnsdorfBackTracking(followingSteps[j].position, final, numb + 1)) {
                    addStepToSteps(pos, numb);
                    return true;
                }
            }
        }
    }

    setFieldVal(pos, false);
    return false;
}

// http://stackoverflow.com/a/27281028/3991578
void flushStdIn() {
  int ch;
  while(((ch = getchar()) !='\n') && (ch != EOF));
}

// Prompt for a digit between 0 and the upper Limit variable
int promptForDigitsLimit(char prompt[], int upperLimit) {
    while(true) {
        int input;
        printf("%s: ", prompt);
        if (scanf("%d", &input) <= 0) {
            printf("Input must be a number.\n");
        } else if (input > upperLimit && upperLimit != -1) {
            printf("Number can't be greater than %d.\n", upperLimit);
        } else if (input <= 0) {
            printf("Number must be greater than 0.\n");
        } else {
            return input;
        }
        flushStdIn();
    }
}

// Prompt for a digit larger than 0.
int promptForDigits(char prompt[]) {
    return promptForDigitsLimit(prompt, -1);
}

void setupBoardSize() {
    sizeX = promptForDigits("Board Size (x)");
    sizeY = promptForDigits("Board Size (y)");
}

coord setupPosition(char option[]) {
    
    char stringXSuffix[] = " X Position";
    char stringYSuffix[] = " Y Position";
    char stringXPos[30] = "";
    char stringYPos[30] = "";
    strcat(stringXPos, option);
    strcat(stringXPos, stringXSuffix);
    strcat(stringYPos, option);
    strcat(stringYPos, stringYSuffix);
    
    coord setup = {
        // Subtract 1 from the given value to go from 1-indexed to 0-indexed.
        .x = promptForDigitsLimit(stringXPos, sizeX) - 1,
        .y = promptForDigitsLimit(stringYPos, sizeY) - 1
    };
    return setup;
}

// Allocate and 0-initialize board and steps arrays
void resetBoardAndSteps() {
    board = (bool *)calloc(sizeX * sizeY, sizeof(bool));
    steps = (int *)calloc(sizeX * sizeY, sizeof(int));
}

/*
 * Returns if there is a possible solution for the knights tour
 * on the current field size.
 * If found, the solution is saved in steps.
 */
bool startWarnsdorfBackTracking(coord initial) {
	printf("Mode: normal.\n");
    return warnsdorfBackTracking(initial, invalid, 0);
}

/*
 * Returns if there is a possible solution for a closed knights tour
 * on the current field size.
 * If found, the solution is saved in steps.
 */
bool startWarnsdorfBackTrackingClosed(coord initial) {
	printf("Mode: closed.\n");
    return warnsdorfBackTracking(initial, initial ,0);
}

/*
 * Returns if there is a possible solution for the knights tour,
 * on the current field size with a given final destination.
 * If found, the solution is saved in steps.
 */
bool startWarnsdorfBackTrackingDest(coord initial, coord final) {
	printf("Mode: given destination.\n");
    return warnsdorfBackTracking(initial, final, 0);
}

int main() {
    
    printf("1: Startfeld wird vom Programm gewählt.\n");
    printf("2: Startfeld wird vom Anwender frei gewählt.\n");
    printf("3: Startfeld wird vom Anwender frei gewählt, der Springer geht einen geschlossenen Pfad.\n");
    printf("4: Startfeld und Endfeld werden vom Anwender frei gewählt.\n");
    
    int option = promptForDigitsLimit("Wählen Sie bitte zwischen den Optionen 1, 2, 3 und 4 aus", 4);
    bool result = false;
    coord initial;
    
    setupBoardSize();
    resetBoardAndSteps();
    
    switch (option) {

        case 1: { // open with initial values pre-defined
        printf("Starting at Position (1,1).\n");
            coord initial = { .x = 0 , .y = 0 };
            result = startWarnsdorfBackTracking(initial);
            break;
        }
        case 2: { // open
            initial = setupPosition("Initial");
            result = startWarnsdorfBackTracking(initial);
            break;
        }
        case 3: { // closed
            initial = setupPosition("Initial");
            result = startWarnsdorfBackTrackingClosed(initial);
            break;
        }
        case 4: {
            initial = setupPosition("Initial");
            coord final = setupPosition("Final");
            result = startWarnsdorfBackTrackingDest(initial, final);
            break;
        }
    }

    if(result) {
        printf( ANSI_COLOR_GREEN "\nA solution has been found!\n" ANSI_COLOR_RESET);
        printf("\nSolution Steps:\n");
        printSteps();
    } else {
        printf("No solution could be found, please try other values!\n");
    }
    
    flushStdIn();
    printf("Press enter to exit the program.");
    getchar();
    return 0;
}
