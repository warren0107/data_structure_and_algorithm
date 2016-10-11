#!/bin/bash

i=1
while true; do
echo "test:$i"
./avl 10000 | grep -nr "This is not a avl tree"
    if [ $? == 0 ];then
        break;
    fi
    let i=i+1
    sleep 1
done
