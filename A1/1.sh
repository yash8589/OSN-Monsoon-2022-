#bin/bash

gcc filegen.c -o filegen
./filegen > input.txt

gcc 1.c -o 1 -lm
./1 input.txt