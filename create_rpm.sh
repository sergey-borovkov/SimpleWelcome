#!/bin/sh

NAME=rosa-launcher-qtquick
VER=0.3.3
DIR_NAME=$NAME-$VER

mkdir $DIR_NAME
cp -rf assets rosa-service-runner src CMakeLists.txt $DIR_NAME
tar -czf $DIR_NAME.tar.gz $DIR_NAME
rm -rf $DIR_NAME
mv -f $DIR_NAME.tar.gz ~/rpmbuild/SOURCES
rpm -ba rosa-launcher.spec