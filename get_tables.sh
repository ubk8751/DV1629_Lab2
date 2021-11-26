#!/bin/bash
for pagesize in 128 256 512 1024
do 
    
    printf "\n\nDoing page size = $pagesize \n"
    for npages in 1 2 4 8 16 32 64 128
    do
        ./optimal $npages $pagesize mult.mem
    done
done