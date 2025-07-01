#!/bin/bash

set -xe

# g++ main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
g++ human2human.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
# g++ human2random.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

./a.out

rm a.out

exit
