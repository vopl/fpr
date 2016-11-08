base=http://www.repository.voxforge1.org/downloads/Russian/Trunk/Audio/Original/48kHz_16bit

mkdir -p work/tgz
mkdir -p work/uncompressed

echo base=${base} > work/items.sh

wget -O work/index.html ${base}/

cat work/index.html | \
    grep "a href=" | \
    grep tgz | \
    awk -F\" '{print "wget -P work/tgz -c ${base}/"$6" || exit 1"; print "tar -C work/uncompressed -xzf work/tgz/"$6" || exit 1"}' >> work/items.sh

chmod +x work/items.sh || exit 1

./work/items.sh || exit 1

echo all done
