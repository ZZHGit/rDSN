<?php
require_once($argv[1]); // type.php
require_once($argv[2]); // program.php
$file_prefix = $argv[3];
$dsn_root = dirname(dirname(dirname(dirname(__DIR__))));
$dsn_root = str_replace('\\', '/', $dsn_root);
?>
cmake_minimum_required(VERSION 2.8.8)

set(DSN_ROOT "<?=$dsn_root?>")
#set(DSN_ROOT "$ENV{DSN_ROOT}")
if((DSN_ROOT STREQUAL "") OR (NOT EXISTS "${DSN_ROOT}/"))
    message(FATAL_ERROR "Please make sure that DSN_ROOT is defined and does exists.")
endif()

include("${DSN_ROOT}/bin/dsn.cmake")

set(MY_PROJ_NAME "<?=$_PROG->name?>")
project(${MY_PROJ_NAME} C CXX)

# Source files under CURRENT project directory will be automatically included.
# You can manually set MY_PROJ_SRC to include source files under other directories.
set(MY_PROJ_SRC "")

# Search mode for source files under CURRENT project directory?
# "GLOB_RECURSE" for recursive search
# "GLOB" for non-recursive search
set(MY_SRC_SEARCH_MODE "GLOB")

set(MY_PROJ_INC_PATH "")

set(MY_PROJ_LIBS "")

set(MY_PROJ_LIB_PATH "")

# Extra files that will be installed
set(MY_BINPLACES "${CMAKE_CURRENT_SOURCE_DIR}/config.ini;${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile")

set(MY_BOOST_PACKAGES "")

dsn_common_setup()
dsn_add_executable()

add_custom_target( docker 
    COMMAND docker build -t "${MY_PROJ_NAME}-image" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${MY_PROJ_NAME}")
add_dependencies( docker "${MY_PROJ_NAME}")
