#!/usr/bin/bash
# =============================================================================
# SPDX-FileComment: Script for creating a CycloneDX SBOM
# SPDX-FileType: SOURCE
# SPDX-FileContributor: ZHENG Robert
# SPDX-FileCopyrightText: 2026 ZHENG Robert
# SPDX-License-Identifier: MIT
#
# @file create_cyclonedx_bom.sh
# @brief Script to generate SBOM reports in CycloneDX and SPDX formats.
# @version <1.2.1>
# @date <2026-03-04>
#
# @author ZHENG Robert (robert@hase-zheng.net)
# @copyright Copyright (c) 2026 ZHENG Robert
#
# @/Users/zb_bamboo/Documents/DEV/CPP/file-sorter/license MIT License
# =============================================================================
set -e

VERSION="1.2.1"

#nvm use --lts
# npm install -g @cyclonedx/cdxgen

/home/zb_bamboo/Downloads/atom/bin/atom -J-Xmx16g usages -o ./docs/app.atom --slice-outfile ./docs/atom_v${VERSION}.json -l cpp ./src
cdxgen -o ./docs/sbom-cyclonedx_v${VERSION}.json -t cpp --usages-slices-file ./docs/atom_v${VERSION}.json --author "ZHENG Robert" --deep -r ./src


## SPDX
# sudo apt install pipx
# pipx ensurepath
# pipx install reuse
reuse spdx -o docs/sbom-spdx_v${VERSION}.spdx

shasum -a 256 ./docs/sbom-cyclonedx_v${VERSION}.json > ./docs/sbom-cyclonedx_v${VERSION}.json.sha256
shasum -a 256 ./docs/sbom-spdx_v${VERSION}.spdx > ./docs/sbom-spdx_v${VERSION}.spdx.sha256
