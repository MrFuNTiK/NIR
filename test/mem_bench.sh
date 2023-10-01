#!/bin/bash

# Run command: ./mem_bench.sh > ../_result/bin/mem_res.txt 2>&1

BINARY_DIR=${CWD}../_result/bin
TEST_NAME=tests
RES_FILE=${BINARY_DIR}/mem_res_44100.txt

win_sizes=(256 512 1024 2048 4096 8192)
tde_meths=(GCC GPS GPS_GRZ)

for size in ${win_sizes[*]}
do
    for meth in ${tde_meths[*]}
    do
        echo window size: ${size}       tde meth: ${meth}
        echo
        valgrind ${BINARY_DIR}/${TEST_NAME} --win-size ${size} --tde-method ${meth}
        echo
        echo ==================================================================
        echo
    done
done
