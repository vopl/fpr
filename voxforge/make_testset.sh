#!/bin/bash

shopt -s globstar

function addNoise
{
    mkdir -p `dirname "$2"`
    sox $1 -p synth ${3}noise vol $4 | sox --no-dither --norm -m $1 - $2
}


function mkOneFreq
{
    freq=$1
    
    for src in work/uncompressed/**/*.wav; do
        dst=`realpath --relative-to="work/uncompressed" "$src"`
        dst=${dst//\/wav\//\/}
        dst="work/testset/${freq}/original/$dst"
        echo "$src -> $dst"

        mkdir -p `dirname "$dst"`
        sox --no-dither --norm $src -b 16 $dst rate $freq
        
        for np in 0.01 0.02 0.03 0.04 0.05 0.06 0.07 0.08 0.09 0.10 0.11 0.12 0.13 0.14 0.15 0.16 0.17 0.18 0.19 0.20; do
            addNoise $dst ${dst/original/white_${np}} white ${np}
            addNoise $dst ${dst/original/pink_${np}} pink ${np}
            addNoise $dst ${dst/original/brown_${np}} brown ${np}
        done
        
        #exit 1;
        
    done
}

mkOneFreq 6000
mkOneFreq 8000
mkOneFreq 11025
mkOneFreq 16000
mkOneFreq 22050


exit 0;

