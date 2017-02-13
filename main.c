#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
    #define ANSI_COLOR_GREEN  ""
    #define ANSI_COLOR_RED    ""
    #define ANSI_COLOR_RESET  ""

    #define PIECE_ODD    "x "
    #define PIECE_EVEN   "o "
    #define PIECE_ACTIVE "@ "
#else
    #define ANSI_COLOR_GREEN  "\x1b[32m"
    #define ANSI_COLOR_RED    "\x1b[31m"
    #define ANSI_COLOR_RESET  "\x1b[0m"

    #define PIECE_ODD     "\x1b[31mx \x1b[0m"
    #define PIECE_EVEN    "\x1b[34mo \x1b[0m"
    #define PIECE_ACTIVE  "\x1b[33m@ \x1b[0m"
#endif

// Beschreibung der Aufgabe: https://drive.google.com/file/d/0BzRp-cLiZDUJcWNUWDdVN3F3SjQ/view?usp=sharingIsCaring

// TODO: Code a menu so the user can choose which Algorithm he wants to run.
// Alternative: offer this as command-line option.

struct coord {
    int x;
    int y;
};

struct extCoord {
    struct coord position;
    int possibleSteps;
};

int effectivity = 0;
int *board;
int sizeX, sizeY;
struct coord *steps;
bool monitoring = false;

void resetBoardAndSteps();
bool getFieldVal(int posX, int posY);
void setFieldVal(int posX, int posY, bool val);
bool checkFieldVal(int posX, int posY, int numb);
bool checkFieldValNumb(int posX, int posY, int fieldNumber, int numb);
//bool simpleBackTracking(int posX, int posY, int numb);
int promptForDigits(char prompt[]);
int promptForDigitsLimit(char prompt[], int upperLimit);
bool isBoardCompleted();
bool startWarnsdorfBackTracking(int initialX, int initialY);
bool warnsdorfBackTracking(int posX, int posY, int finalX, int finalY, int numb, int maxTries, int modifier);
int countPossibleSteps(int initialX, int initialY);

void printBoard() {
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (getFieldVal(x, y)) {
                printf(PIECE_ACTIVE);
            } else if((y + x)%2) {
                printf(PIECE_EVEN);
            } else {
                printf(PIECE_ODD);
            }
            //printf(getFieldVal(x, y) ? PIECE_ACTIVE : (y + x)%2 == 0 ? PIECE_EVEN : PIECE_ODD);
        }
        printf("\n");
    }
}

void printSteps() {
    int stepsAmount = sizeX*sizeY;

    for(int i = 0; i < stepsAmount; i++) {
        printf("(%d,%d) ", steps[i].x + 1, steps[i].y + 1);
    }

    printf("\n");
}


// Gibt nur den Wert von einer bestimmten Position aus.
bool getFieldVal(int posX, int posY) {
    if (posX < sizeX && posY < sizeY && posX >= 0 && posY >= 0) {
        return *(board + posY*sizeX + posX);
    } else {
        return true;
    }
}

void setFieldVal(int posX, int posY, bool val) {
    // http://i.imgur.com/KCztDMN.png
    *(board + posY*sizeX + posX) = val;
}

//ersetzt später checkFieldValNumb
struct coord getFieldByNumber(int initialX, int initialY, int fieldNumber, int modifier) {
    struct coord result = {.x = initialX, .y = initialY};
    fieldNumber = (fieldNumber + modifier)%8;
    switch (fieldNumber) {
        case (0):
            result.x += 2;
            result.y += 1;
            break;
        case (1):
            result.x += 2;
            result.y -= 1;
            break;
        case (2):
            result.x -= 2;
            result.y += 1;
            break;
        case (3):
            result.x -= 2;
            result.y -= 1;
            break;
        case (4):
            result.x += 1;
            result.y += 2;
            break;
        case (5):
            result.x += 1;
            result.y -= 2;
            break;
        case (6):
            result.x -= 1;
            result.y += 2;
            break;
        case (7):
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

int calcMaxTries() {
    return sizeX * sizeY;
}

/*
 * Returns if there is a possible solution for the knights tour
 * on the current field size.
 * If found, the solution is saved in steps.
 */
bool startWarnsdorfBackTracking(int initialX, int initialY) {
	printf("Mode: normal.\n");
    return warnsdorfBackTracking(initialX, initialY, -1, -1 ,0, calcMaxTries(), 0);
}

/*
 * Returns if there is a possible solution for a closed knights tour
 * on the current field size.
 * If found, the solution is saved in steps.
 */
bool startWarnsdorfBackTrackingClosed(int initialX, int initialY) {
	printf("Mode: closed.\n");
	int maxTries = calcMaxTries();
	while (true) {
    	for (int i = 0; i < 8; i++) {
    	    effectivity = 0;
    	    resetBoardAndSteps();
    	    if (warnsdorfBackTracking(initialX, initialY, initialX, initialY ,0, maxTries , i)) {
    	        printf("SOLUTION FOUND");
    	        return true;
    	}
	}
	maxTries *= 2;
	}
	printf("NOOOOOOOOOOOO");
    return 0;
}

/*
 * Returns if there is a possible solution for the knights tour,
 * on the current field size with a given final destination.
 * If found, the solution is saved in steps.
 */
bool startWarnsdorfBackTrackingDest(int initialX, int initialY, int finalX, int finalY) {
	printf("Mode: given Destination.\n");
    return warnsdorfBackTracking(initialX, initialY, finalX, finalY ,0, calcMaxTries(), 0);
}

/*
 * Recursive algorithm for the Knights path
 */
bool warnsdorfBackTracking(int posX, int posY, int finalX, int finalY, int numb, int maxTries, int modifier) {
    if (effectivity > maxTries) {
        return false;
    }
    setFieldVal(posX, posY, true);

    if (monitoring) {
        effectivity++;
    }

    if (numb == (sizeX * sizeY - 1)) {
    	for (int i = 0; i < 8; i++) {
    		struct coord buffer = getFieldByNumber(posX, posY, i,0);
    		if ((buffer.x == finalX && buffer.y == finalY) || finalX == -1 || finalY == -1) {
    	        addStepToSteps(posX, posY, numb);
    	        return 1;
    		}
    	}
    	setFieldVal(posX, posY, false);
    	return 0;
    }

    struct extCoord followingSteps[8] =
    {
        {.possibleSteps = -1}, {.possibleSteps = -1},
        {.possibleSteps = -1}, {.possibleSteps = -1},
        {.possibleSteps = -1}, {.possibleSteps = -1},
        {.possibleSteps = -1}, {.possibleSteps = -1}
    };

    // Versucht alle möglichen Züge vom aktuellem Feld auszuführen.
    for (int i = 0; i < 8; i++) {
        struct coord buffer = getFieldByNumber(posX, posY, i,modifier);

        if (!getFieldVal(buffer.x, buffer.y)) {
            followingSteps[i].possibleSteps = countPossibleSteps(buffer.x, buffer.y);
            followingSteps[i].position = buffer;
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (followingSteps[j].possibleSteps == i) {
                if (warnsdorfBackTracking(followingSteps[j].position.x, followingSteps[j].position.y, finalX, finalY, numb + 1, maxTries, modifier)) {
                    addStepToSteps(posX, posY, numb);
                    return 1;
                } else if (effectivity > maxTries) {
                    setFieldVal(posX, posY, false);
                    return false;
                }
                
            }
        }
    }

    setFieldVal(posX, posY, false);
    return 0;
}

int countPossibleSteps(int initialX, int initialY) {
    int result = 0;
    struct coord buffer;
    for (int i = 0; i < 8; i++) {
        buffer = getFieldByNumber(initialX, initialY, i, 0);
        if(!getFieldVal(buffer.x, buffer.y)) {
            result++;
        }
    }
    return result;
}

/*
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
        struct coord temp = getFieldByNumber(posX, posY, i);
        if (!getFieldVal(temp.x, temp.y) && simpleBackTracking(temp.x, temp.y, numb + 1)) {
            addStepToSteps(posX, posY, numb);
            return 1;
        }
    }

    setFieldVal(posX, posY, false);
    return 0;
}
*/
// http://stackoverflow.com/a/27281028/3991578
void flushStdIn(void) {
  int ch;
  while(((ch = getchar()) !='\n') && (ch != EOF));
}

// Prompt for a digit between 0 and the upper Limit variable
int promptForDigitsLimit(char prompt[], int upperLimit) {
    while(true) {
        int input;
        printf("%s: ", prompt);

        if (scanf("%d", &input) <= 0) {
            flushStdIn();
            printf("Input must be a number.\n");
        } else if (input > upperLimit && upperLimit != -1) {
            printf("Number can't be greater than %d.\n", upperLimit);
        } else if (input <= 0) {
            printf("Number must be greater than 0.\n");
        } else {
            return input;
        }
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

struct coord setupInitialPosition() {
    
    struct coord initial = {
        // Subtract 1 from the given value to go from 1-indexed to 0-indexed.
        .x = promptForDigitsLimit("Initial X Position", sizeX) - 1,
        .y = promptForDigitsLimit("Initial Y Position", sizeY) - 1
    };

    return initial;
}

// Allocate and 0-initialize board and steps arrays
void resetBoardAndSteps() {
    board = (int *)calloc(sizeX * sizeY, sizeof(int));
    steps = (struct coord *)calloc(sizeX * sizeY, sizeof(struct coord));
}

double calcEffectivity() {
    monitoring = true;

    double result = 0;
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            effectivity = 0;
            //warnsdorfBackTracking(x, y, 0);
            result += effectivity;
            printf("x:%d y:%d Effektiviät: %d \n", x,y,effectivity);
            resetBoardAndSteps();
        }
    }
    result = result / (sizeX * sizeY);;
    monitoring = false;
    return result;
}

int main() {
    
    printf("1: Startfeld wird vom Programm gewählt.\n");
    printf("2: Startfeld wird vom Anwender frei gewählt.\n");
    printf("3: Startfeld wird vom Anwender frei gewählt, der Springer geht einen geschlossenen Pfad.\n");
    
    int option = promptForDigitsLimit("Wählen Sie bitte zwischen den Optionen 1, 2 und 3 aus", 3);
    bool result = false;
    struct coord initial;
    
    setupBoardSize();
    resetBoardAndSteps();
    
    //Test code: 
    for(int x = 0; x < sizeX;x++) {
        for(int y = 0; y < sizeY;y++) {
            monitoring = true;
            effectivity = 0;
            startWarnsdorfBackTrackingClosed(x, y);
            resetBoardAndSteps();
            printf("x: %d y: %d effectivity: %d ", x, y, effectivity);
        }
    }
    
    monitoring = true;
    
    switch (option) {

        case 1: { // open with initial values pre-defined
            result = startWarnsdorfBackTracking(0, 0);
            break;
        }
        case 2: { // open
            initial = setupInitialPosition();
            result = startWarnsdorfBackTracking(initial.x, initial.y);
            break;
        }
        case 3: { // closed
            initial = setupInitialPosition();
            result = startWarnsdorfBackTrackingClosed(initial.x, initial.y);
            break;
        }
    }
/*
    setFieldVal(initial.x, initial.y, true); // just for the output, doesn't alter algorithm
    printf("\nStartfeld:\n");
    printBoard();
*/
    if(result) {
        printf( ANSI_COLOR_GREEN "\nA solution has been found!\n" ANSI_COLOR_RESET);
        printf("\nErgebnisfeld:\n");
        printBoard();
        printSteps();
    } else {
        printf("No solution could be found, please try other values!\n");
    }

    printf("Press enter to exit the program.");
    getchar();

    getchar();
    return 0;
}
