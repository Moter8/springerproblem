#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
    #define ANSI_COLOR_GREEN    ""
    #define ANSI_COLOR_RED    ""
    #define ANSI_COLOR_RESET  ""
    
    #define PIECE_ODD "x "
    #define PIECE_EVEN "o "
    #define PIECE_ACTIVE "@ "
#else
    #define ANSI_COLOR_GREEN    "\x1b[32m"
    #define ANSI_COLOR_RED    "\x1b[31m"
    #define ANSI_COLOR_RESET  "\x1b[0m"
    
    #define PIECE_ODD "\x1b[31mx \x1b[0m"
    #define PIECE_EVEN "\x1b[34mo \x1b[0m"
    #define PIECE_ACTIVE "\x1b[33m@ \x1b[0m"
#endif

// Beschreibung der Aufgabe: https://drive.google.com/file/d/0BzRp-cLiZDUJcWNUWDdVN3F3SjQ/view?usp=sharing

// TODO: 2 verschiedene Outputs anhand der Steps, refactor checkFieldValNumb to use coord structure
// MY_TYPE a = { .flag = true, .value = 123, .stuff = 0.456 };

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

bool getFieldVal(int posX, int posY);
void setFieldVal(int posX, int posY, bool val);
bool checkFieldVal(int posX, int posY, int numb);
bool checkFieldValNumb(int posX, int posY, int fieldNumber, int numb);
bool simpleBackTracking(int posX, int posY, int numb);
int promptForDigits(char prompt[]);
int promptForDigitsLimit(char prompt[], int upperLimit);
bool isBoardCompleted();
bool startWarnsdorfBackTracking(int initialX, int initialY);
bool warnsdorfBackTracking(int posX, int posY, int numb);
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
struct coord getFieldByNumber(int initialX, int initialY, int fieldNumber) {
    struct coord result = {.x = initialX, .y = initialY};
    switch (fieldNumber) {
        //Neue Reihenfolge schneller am Rand, langsamer in der Mitte
        //durchschnittlich kein Unterschied
        /*
        case 0:
            result.x -= 1;
            result.y += 2;
            break;
        case 1:
            result.x += 1;
            result.y -= 2;
            break;
        case 2:
            result.x += 2;
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
            result.x -= 1;
            result.y -= 2;
            break;
        case 6:
            result.x += 2;
            result.y -= 1;
            break;
        case 7:
            result.x -= 2;
            result.y += 1;
            break;
            */
            
            //Alte Reihenfolge schneller in der Mitte, langsamer am Rand
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
bool startWarnsdorfBackTracking(int initialX, int initialY) {
    effectivity = 0;
    return warnsdorfBackTracking(initialX, initialY, 0);
}

// Recursive algorithm for backTracking
bool warnsdorfBackTracking(int posX, int posY, int numb) {
    setFieldVal(posX, posY, true);
    
    if (monitoring) {
        effectivity++;
    }
    
    if (numb == (sizeX * sizeY - 1)) {
        
        addStepToSteps(posX, posY, numb);
        return 1;
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
        struct coord buffer = getFieldByNumber(posX, posY, i);
        
        if (!getFieldVal(buffer.x, buffer.y)) {
            followingSteps[i].possibleSteps = countPossibleSteps(buffer.x, buffer.y);
            followingSteps[i].position = buffer;
        }
    }
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (followingSteps[j].possibleSteps == i) {
                if (warnsdorfBackTracking(followingSteps[j].position.x, followingSteps[j].position.y, numb + 1)) {
                    addStepToSteps(posX, posY, numb);
                    return 1;
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
        buffer = getFieldByNumber(initialX, initialY, i);
        if(!getFieldVal(buffer.x, buffer.y)) {
            result++;
        }
    }
    return result;
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
        struct coord temp = getFieldByNumber(posX, posY, i);
        if (!getFieldVal(temp.x, temp.y) && simpleBackTracking(temp.x, temp.y, numb + 1)) {
            addStepToSteps(posX, posY, numb);
            return 1;
        }
    }
    
    setFieldVal(posX, posY, false);
    return 0;
}

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

struct coord setupInput() {
    // One less to go from 1-indexed to 0-indexed counting
    struct coord initial = {.x = -1, .y = -1};
    
    sizeX = promptForDigits("Board Size (x)");
    sizeY = promptForDigits("Board Size (y)");
    initial.x += promptForDigitsLimit("Initial X Position", sizeX);
    initial.y += promptForDigitsLimit("Initial Y Position", sizeY);
    
    return initial;
}

// Allocate and 0-initialize board and steps arrays
void resetStepsABoard() {
    board = (int *)calloc(sizeX * sizeY, sizeof(int));
    steps = (struct coord *)calloc(sizeX * sizeY, sizeof(struct coord));
}

double calcEffectivity() {
    monitoring = true;
    double result = 0;
    for (int x = 0; x < sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            effectivity = 0;
            warnsdorfBackTracking(x, y, 0);
            result += effectivity;
        }
    }
    result = result / (sizeX * sizeY);;
    monitoring = false;
    return result;
}

int main() {
    struct coord initial = setupInput();
    
    resetStepsABoard();
    
    setFieldVal(initial.x, initial.y, true);
    
    printf("\nStartfeld:\n");
    printBoard();
    
    // Starts main algorithm
    if(startWarnsdorfBackTracking(initial.x, initial.y)) {
        printf( ANSI_COLOR_GREEN "\nA solution has been found!\n" ANSI_COLOR_RESET);
    } else {
        printf("No solution could be found!\n");
    }
    
    
    printf("\nErgebnisfeld:\n");
    printBoard();
    printSteps();
    
    //printf("Effektivität: %.2f \n", calcEffectivity(sizeY, sizeX));
    
    return 0;
}
