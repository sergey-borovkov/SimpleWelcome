#!/bin/sh

NAME=rosa-launcher
VER=2.0.0
DIR_NAME=$NAME-$VER

#git archive -o $DIR_NAME.tar.gz HEAD
#rpm -ba $NAME.spec
#rm $DIR_NAME.tar.gz

mkdir $DIR_NAME
cp -rf rosa-starter po assets facebook-timeframe-plugin vkontakte-timeframe-plugin src CMakeLists.txt config.h.in $DIR_NAME
tar -czf $DIR_NAME.tar.gz $DIR_NAME
rm -rf $DIR_NAME
cp -f $DIR_NAME.tar.gz ~/rpmbuild/SOURCES
rpm -ba rosa-launcher.spec
