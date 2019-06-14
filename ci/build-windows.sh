set -e

rm -rf build
mkdir build
cd build

curl -O https://dilborceisv8p.cloudfront.net/bsf_2019.06.07_win64.zip
unzip bsf_2019.06.07_win64.zip -d bsf

vcpkg update
vcpkg install libsquish physfs --triplet x64-windows

cmake -Dbsf_INSTALL_DIR=`pwd`/bsf \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DREGOTH_USE_SYSTEM_BSF=On \
      -DSKIP_AUTOMATE_VCPKG=On \
      -DCMAKE_TOOLCHAIN_FILE="${VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake" \
      -A x64 \
      ../..

cmake --build . --config Release --parallel

cd ..
