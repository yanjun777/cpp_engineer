#!/bin/bash

#等号不能有空格
#变量名大小写敏感
SRC_FILE="core2_vec_ref.cpp"
OUT_FILE="core2_vec_ref"


echo "Compiling $SRC_FILE ..."

g++ -std=c++11 ""$SRC_FILE -o $OUT_FILE 

# []必须有空格 
if [ $? -eq 0 ]; then 
    echo "Compilation successful. Running $OUT_FILE ..."
    ./$OUT_FILE
else 
    echo "Compilation failed."
fi
