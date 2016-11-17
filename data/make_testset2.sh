#!/bin/bash

shopt -s globstar

function mkOneFreq
{
    freq=$1
    inPrefix=$2
    outPrefix=$3
    ext=$4
    
    for src in ${inPrefix}/**/*.${ext}; do
        dst=`realpath --relative-to="${inPrefix}" "$src"`
        dst=${dst//\/${ext}\//\/}
        dst=${dst/\.${ext}/.ogg}
        dst="${outPrefix}/$dst"
        echo "$src -> $dst"

        mkdir -p `dirname "$dst"`
        sox --norm=-1 $src $dst rate $freq silence 1 0.5 0.1% -1 0.5 0.1% remix - pad 0.5 0.5
        
        #exit 1;
        
    done
}

mkOneFreq 8000 ./voxforge/work/uncompressed ./testset2/voxforge wav
mkOneFreq 8000 ./bigpodcast/work ./testset2/bigpodcast mp3
mkOneFreq 8000 ./majak/work ./testset2/majak mp3
mkOneFreq 8000 ./podfm/work ./testset2/podfm mp3
mkOneFreq 8000 ./svoboda/work ./testset2/svoboda mp3


exit 0;

