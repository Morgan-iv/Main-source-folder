#!/bin/bash

solc $1 && echo "var compilerOutput=`solc --optimize --combined-json abi,bin,interface $1`" > "$1.js"
