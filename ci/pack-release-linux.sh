set -e

echo Current Working Directory:
echo `pwd`

INTERNAL_DIR=artifacts

rm -rf ./$INTERNAL_DIR

mkdir $INTERNAL_DIR

cp ../README.md $INTERNAL_DIR/
cp ../LICENSE $INTERNAL_DIR/
cp -r ../content $INTERNAL_DIR/

cp -r build/bin/* $INTERNAL_DIR/
cp -r build/bsf/bin/Data $INTERNAL_DIR/
cp    build/bsf/lib/*.so* $INTERNAL_DIR/
cp    build/bsf/lib/bsf*/*.so* $INTERNAL_DIR/
