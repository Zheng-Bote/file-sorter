#!/usr/bin/bash
set -e

VERSION="1.2.0"

#nvm use --lts
# npm install -g @cyclonedx/cdxgen

/home/zb_bamboo/Downloads/atom/bin/atom -J-Xmx16g usages -o ./docs/app.atom --slice-outfile ./docs/atom_v${VERSION}.json -l cpp ./src
cdxgen -o ./docs/sbom-cyclonedx_v${VERSION}.json -t cpp --usages-slices-file ./docs/atom_v${VERSION}.json --author "ZHENG Robert" --deep -r ./src


## SPDX
# sudo apt install pipx
# pipx ensurepath
# pipx install reuse
reuse spdx -o docs/sbom-spdx_v${VERSION}.json

shasum -a 256 ./docs/sbom-cyclonedx_v${VERSION}.json > ./docs/sbom-cyclonedx_v${VERSION}.json.sha256
shasum -a 256 ./docs/sbom-spdx_v${VERSION}.json > ./docs/sbom-spdx_v${VERSION}.json.sha256
