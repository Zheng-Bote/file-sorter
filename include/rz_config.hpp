/**
 * @file config.hpp.in
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief configuration
 * @version 2.0.0
 * @date 2025-11-15
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */
#pragma once

#include <cstdint>
#include <string>

static const std::string PROJECT_NAME = "FileSorter";
static const std::string PROG_LONGNAME = "FileSorter for Download-Folder";
static const std::string PROJECT_DESCRIPTION = "Sorts your downloads automatically.";

static const std::string PROJECT_EXECUTABLE = "FileSorter";

static const std::string PROJECT_VERSION = "1.1.0";
static const std::int32_t PROJECT_VERSION_MAJOR{1};
static const std::int32_t PROJECT_VERSION_MINOR{1};
static const std::int32_t PROJECT_VERSION_PATCH{0};

static const std::string PROJECT_HOMEPAGE_URL = "https://github.com/Zheng-Bote/file-sorter";
static const std::string PROG_AUTHOR = "ZHENG Bote";
static const std::string PROG_CREATED = "2025";
static const std::string PROG_ORGANIZATION_NAME = "ZHENG Robert";
static const std::string PROG_ORGANIZATION_DOMAIN = "net.hase-zheng";

static const std::string CMAKE_CXX_STANDARD = "c++23";
static const std::string CMAKE_CXX_COMPILER = "MSVC 19.44.35220.0";
static const std::string CMAKE_QT_VERSION = "6.10.1";
