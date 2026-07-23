#!/bin/bash
gcc *.c -o passionix $(pkg-config --cflags --libs json-glib-1.0) -lcrypto
./passionix
