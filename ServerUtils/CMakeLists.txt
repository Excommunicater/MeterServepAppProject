cmake_minimum_required(VERSION 3.16.3)

set(thisLibrary serverUtils)

add_library(${thisLibrary} 
    dataUtils.c
    errorHandling.c
    messageUtils.c
    queueUtils.c
    serverUtils.c
)

