set -e

rm -rf build
mkdir build
cd build

wget https://dilborceisv8p.cloudfront.net/bsf_2019.09.18_linux.tar.gz

mkdir bsf
tar -xf bsf_*.tar.gz -C bsf

cmake -Dbsf_INSTALL_DIR=`pwd`/bsf \
      -DCMAKE_BUILD_TYPE=Release \
      -DREGOTH_USE_SYSTEM_BSF=On \
      ../..

cmake --build . --config Release --parallel

cd ..
