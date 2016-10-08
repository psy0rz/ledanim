#!/bin/bash
set -e


#1. we only have 1Mb memory for our public_html stuff, thats why we need to gzip it.
#2. because of the 32 character filename limit we also hash the filename. (yeah what a nice mess isnt it ;)
#3. because the esp doesnt handle parallel requests very well we try to combine all javascript into one .js file


#combine javascript files
rm -rf /tmp/combined 2>/dev/null || true
cp -r ../web/public_html /tmp/combined

CWD=`pwd`
pushd /tmp/combined
JS_FILES=`cat index.html |grep 'script src='|sed 's/.*src="//'|sed 's/".*//'|grep -v combined.js`
$CWD/compressjs.sh $JS_FILES combined.js
rm $JS_FILES
sed -i 's/.*script src=.*//' index.html
popd

#hash and compress all files
rm data/*.gz &>/dev/null || true
SRC_DIR="/tmp/combined"

for SRC in `find "$SRC_DIR" -type f`; do
    REL_SRC=`echo "$SRC" | sed "s@$SRC_DIR@@"`
    echo "Processing $REL_SRC"
    HASHED_DST=data/`echo -n "$REL_SRC"|md5sum|cut -c1-26`.gz
    if [ -e $HASHED_DST ]; then
        echo "hash collision!"
        exit 1
    fi

    gzip -9 < "$SRC" > $HASHED_DST
    echo "gzipped to: $HASHED_DST"
done


if [ "$1" == "" ]; then
	echo "Specify environement if you want to auto-upload, for example: $0 esp12e"
	exit 0
fi

~/.atom/packages/platformio-ide/penv/bin/platformio run --target uploadfs --environment $1
