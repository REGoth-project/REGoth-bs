set -e

rm -rf build
mkdir build
cd build

wget https://dilborceisv8p.cloudfront.net/bsf_2019.06.06_linux.tar.gz

mkdir bsf
tar -xf bsf_2019.06.06_linux.tar.gz -C bsf

vcpkg update
vcpkg install libsquish physfs --triplet x64-linux

cmake -Dbsf_INSTALL_DIR=`pwd`/bsf \
      -DCMAKE_BUILD_TYPE=Release \
      -DREGOTH_USE_SYSTEM_BSF=On \
      -DSKIP_AUTOMATE_VCPKG=On \
      -DCMAKE_TOOLCHAIN_FILE="${VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake" \
      ../..

cmake --build . --config Release --parallel

cd ..
