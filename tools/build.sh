#!/bin/bash

system=$1
image=$2

[ ! -f "$system" ] && echo "there is no system binary file there" && exit -1
dd if=$system seek=1 bs=512 count=200 of=$image 2>&1 >/dev/null
