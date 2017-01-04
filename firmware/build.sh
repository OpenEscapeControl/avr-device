#!/bin/bash

function Ntoname()
{
    name=$( (echo obase=16; echo $1) | bc | perl -ne 'print lc')
    if (( ${#name} < 2 ))
    then
        name="0$name"
    fi
    echo $name
}


if [ "X$2" == "X" ]
then
    echo "Usage: $0 device_start device_end"
    exit 0
fi

function build_one()
{
    if [ "X$1" == "X" ]
    then
        echo "Usage: $0 device_n"
        exit 0
    fi
    N=$1
    filename="d$(Ntoname $N).hex"
    echo "Device id: $N, $filename"
    echo "int device_n = $N;" > ../project/device_n.cpp
    make -C ../project clean
    rm -rf ../project/build*
    make -C ../project
    cp ../project/build/project.hex $filename
}

device_end=$2
device=$1

while (( $device <= $device_end ))
do
    build_one $device
    let device=device+1
done

md5sum *.hex
