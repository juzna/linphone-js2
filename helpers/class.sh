#!/bin/bash

X=$1
cat sampleAPI.h | sed s/sampleAPI/${X}API/g > ${X}API.h
cat sampleAPI.cpp | sed s/sampleAPI/${X}API/g > ${X}API.cpp
