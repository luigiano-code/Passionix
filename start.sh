#!/bin/bash
gcc *.c -o passionix $(pkg-config --cflags gtk4 --libs json-glib-1.0 libadwaita-1) -lcrypto
./passionix
