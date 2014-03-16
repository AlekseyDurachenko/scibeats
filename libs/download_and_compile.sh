#!/bin/bash

git clone https://github.com/AlekseyDurachenko/QtSampleEngine.git qtsampleengine
git clone https://github.com/AlekseyDurachenko/QtMiniSEED.git qtminiseed

wget https://seiscode.iris.washington.edu/attachments/download/348/libmseed-2.11.tar.gz
tar xvfz *.tar.gz
rm *.tar.gz
cd libmseed
make -j5
cd ..
