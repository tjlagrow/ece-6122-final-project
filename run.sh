#!/bin/bash

echo "You need to make sure you have taken out the trash! UGH!"
rm -rf raytracer #TJs_25plus_point_image.png

echo "Starting the Tracing of Rays young one 0.o"
c++ -o raytracer -O3 -Wall raytracer.cpp

echo "DING! Time to take it out of the oven!"
echo "IM THINKING SO HAAAARRRD!!!!!!!"
time ./raytracer

echo "the .png is made, go open it :)"
open TJs_25plus_point_image.png