# Springerproblem

Springerproblem is actually just the name for [Knight's Tour](https://en.wikipedia.org/wiki/Knight's_tour) in german.

I encourage you to read the wikipedia article about this problem.

Axel Conrad wrote a [page in german](http://www.axel-conrad.de/springer/springer.html) explaining the different algorithms involved.

The main way to solve this problem quickly is following [Warnsdorf's Rule](https://en.wikipedia.org/wiki/Knight's_tour#Warnsdorf's_rule), in order to not simply try every possibility, i.e. backtracking.

## Building

Compile with optimization, using the C99 standard and displaying all warnings:

`gcc main.c -o out -O3 --std=c99 -lm -Wall -Werror`
