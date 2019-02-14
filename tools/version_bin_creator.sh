#!/bin/bash
set -e

echo "version bin creator called"
echo "writing version \"${1}\" to \"${2}\""

VERSION_BIN=$2
> ${VERSION_BIN}  # empty version file

VITA_VERSION=$1
VERSIONS=(${VITA_VERSION//./ })
VERSION_HEX=""

for i in 0 1; do
    VERSION_PART=${VERSIONS[i]}
    if [[ -z ${VERSION_PART} ]]; then
        VERSION_PART=0
    fi
    # sed is used to turn hex representation around (into little-endian as VITA's ARM will read it)
    VERSION_PART_HEX=$(printf "%.8x" "${VERSION_PART}" | sed -E 's/(..)(..)(..)(..)/\4\3\2\1/')
    VERSION_HEX="${VERSION_HEX}${VERSION_PART_HEX}"
    echo "version part ${i}: ${VERSION_PART} -> ${VERSION_PART_HEX}"
done
echo "echo 0x${VERSION_HEX} > \"${VERSION_BIN}\""
echo "0: ${VERSION_HEX}" | xxd -r -g0 > ${VERSION_BIN}
