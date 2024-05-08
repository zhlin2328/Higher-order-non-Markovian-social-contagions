#!/bin/bash
. ${HOME}/.bashrc

cd $PBS_O_WORKDIR
TASK_ID=$(($PBS_VNODENUM))

./a.out ${TASK_ID} > output.${TASK_ID}.dat 2> output.${TASK_ID}.log
