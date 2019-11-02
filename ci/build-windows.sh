set -e

rm -rf build
mkdir build
cd build

curl -O https://dilborceisv8p.cloudfront.net/bsf_2019.09.18_win64.zip
unzip bsf_*.zip -d bsf

cmake -Dbsf_INSTALL_DIR=`pwd`/bsf \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DREGOTH_USE_SYSTEM_BSF=On \
      -A x64 \
      ../..

cmake --build . --config Release --parallel

cd ..
