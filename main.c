#include <stdio.h>

int main() {

    int input;
    
    printf("Type in the board size: ");
    scanf("%d", &input);

    printf("Board Size: %dx%d\n", input, input);

    return 0;
}