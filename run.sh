#!/bin/bash
gcc -E -x c -P -I lib "$@" | ./compiler
