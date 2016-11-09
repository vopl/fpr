#!/bin/bash

base=./../voxforge/work/testset
fpr_args=
psql_args="-d test -h 127.0.0.1 -p 5432 -U postgres -q "









shopt -s globstar





##########################################################
srcs=(${base}/**/*.wav)
num=$(( ${#srcs[@]} -1 ))
echo "$num wav files found"
newseq=$( shuf -i 0-$num )
for i in $newseq; do
     srcs2+=(${srcs[$i]})
done


##########################################################
for src in ${srcs2[@]}; do
    name=`realpath --relative-to="${base}" "$src"`
    echo $name

    if [[ `echo "SELECT count(*) FROM track WHERE name='$name'" | psql -t ${psql_args}` =~ 1 ]]; then 
        echo already in db
        continue;
    fi



    vals=
    for line in $(./aid-fpr-cli ${fpr_args} --in-file=$src | tr ":" "," | tr -d " "); do
        vals="$vals,($line)"
        #echo $line
    done

    if [[ -z $vals ]]; then 
        echo empty
        continue;
    fi


    vals=${vals:1}

    if [[ `echo "SELECT count(*) FROM track WHERE name='$name'" | psql -t ${psql_args}` =~ 1 ]]; then 
        echo already in db
        continue;
    fi

    sql="BEGIN;\n
INSERT INTO track (name) VALUES('$name');\n
INSERT INTO fingerprint (track_id, pos, val) SELECT * FROM (select max(id) FROM track) as p1, (VALUES $vals) as p2;\n
COMMIT;"

    echo -e "$sql" | psql ${psql_args}
    echo "imported"

    #exit 0
done
