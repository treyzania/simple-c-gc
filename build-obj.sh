#!/bin/bash

FILE=obj.c
PKGS=glib-2.0

gcc $FILE `pkg-config --libs --cflags $PKGS`
