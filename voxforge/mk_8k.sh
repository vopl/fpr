#!/bin/bash


function addNoise
{
    mkdir -p `dirname "$2"`
    sox $1 -p synth ${3}noise vol $4 | sox --no-dither --norm -m $1 - $2
}


shopt -s globstar

for src in work/uncompressed/**/*.wav; do
    dst=`realpath --relative-to="work/uncompressed" "$src"`
    dst=${dst//\/wav\//\/}
    dst="work/8k/original/$dst"
    echo "$src -> $dst"

    mkdir -p `dirname "$dst"`
    sox --no-dither --norm $src -b 16 $dst rate 8000
    
    for np in 0.01 0.02 0.03 0.04 0.05 0.06 0.07 0.08 0.09 0.10; do
        addNoise $dst ${dst/original/add_white_${np}} white ${np}
        addNoise $dst ${dst/original/add_pink_${np}} pink ${np}
        addNoise $dst ${dst/original/add_brown_${np}} brown ${np}
    done
    
    #exit 1;
    
done


exit 0;

