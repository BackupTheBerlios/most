#!/bin/sh

mkdir -p             ../../../../api/doc/api

mkdir -p             ../../../../api/sys/inc/doc
mkdir -p             ../../../../api/usr/inc/doc
mkdir -p             ../../../../api/bsp/mdc/sys/inc/doc
mkdir -p             ../../../../api/bsp/sam/sys/inc/doc
mkdir -p             ../../../../api/bsp/la2/sys/inc/doc
mkdir -p             ../../../../api/bsp/pc/sys/inc/doc

cp ../../doc/api/*.html                 ../../../../api/doc/api
cp ../../doc/api/*.css                  ../../../../api/doc/api
cp -r ../../doc/api/images              ../../../../api/doc/api

cp -r ../../sys/inc/doc/html            ../../../../api/sys/inc/doc
cp -r ../../usr/inc/doc/html            ../../../../api/usr/inc/doc
cp -r ../../bsp/mdc/sys/inc/doc/html    ../../../../api/bsp/mdc/sys/inc/doc
cp -r ../../bsp/sam/sys/inc/doc/html    ../../../../api/bsp/sam/sys/inc/doc
cp -r ../../bsp/la2/sys/inc/doc/html    ../../../../api/bsp/la2/sys/inc/doc
cp -r ../../bsp/pc/sys/inc/doc/html     ../../../../api/bsp/pc/sys/inc/doc

