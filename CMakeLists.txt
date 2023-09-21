cmake_minimum_required(VERSION 3.26)

#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/build-conan/conan_toolchain.cmake" CACHE STRING "")
#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../deps/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")


if(EXISTS ${CMAKE_TOOLCHAIN_FILE})
message(STATUS "MAIN CMAKE_TOOLCHAIN_FILE EXISTS:" ${CMAKE_TOOLCHAIN_FILE})
else()
message(STATUS "MAIN CMAKE_TOOLCHAIN_FILE NOT FOUND:" ${CMAKE_TOOLCHAIN_FILE})
endif()

project(demo-depbox-gmp-project LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ===================
# header only library
# -------------------
add_library(manydeps-cln-lib INTERFACE)
target_include_directories(manydeps-cln-lib INTERFACE include/)

add_executable(app_demo src/main.cpp)
target_link_libraries(app_demo PRIVATE manydeps-cln-lib) #external_folder)

#add_library(external_folder INTERFACE)
#target_include_directories(external_folder INTERFACE external/)

if(UNIX)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/vcpkg_installed/x64-linux")
else() # WINDOWS?
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/build/vcpkg_installed/x64-windows")
set(GMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/build/vcpkg_installed/x64-windows/include")
set(GMP_LIBRARIES "${CMAKE_SOURCE_DIR}/build/vcpkg_installed/x64-windows/lib/mpir.lib") 
endif()

# PkgConfig from vcpkg only working on Linux!
# On Windows, do it manually with FindGMP

if(UNIX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)
#
add_library(gmp::gmp ALIAS PkgConfig::gmp)
add_library(gmp::gmpxx ALIAS PkgConfig::gmpxx)
#
target_link_libraries(app_demo PRIVATE gmp::gmp)
#
else() # WINDOWS?
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
# invoke FindGMP.cmake
# find_package(GMP REQUIRED)
# message(STATUS "MAIN GMP FOUND!" ${GMP_FOUND})
#
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake/modules") # ???
set(VCPKG_WINDOWS "${CMAKE_SOURCE_DIR}/build/vcpkg_installed/x64-windows")
include_directories("${VCPKG_WINDOWS}/include")
link_directories("${VCPKG_WINDOWS}/lib")
#
set(MPIR_LIB_PATH "${VCPKG_WINDOWS}/lib")
target_link_libraries(app_demo PRIVATE "${MPIR_LIB_PATH}/mpir.lib") # "${MPIR_LIB_PATH}/mpirxx.lib"
endif()

# ====================================================
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/cln/cmake/modules")
# -----------------------------------------------------------------------------------------
# Override build/gmp-config.cmake from conan (if toolchain is activated) - prefix is first!
# Note that we need to follow CLN recipes, not conan... as they are slightly different!
# conan demands find_package(gmp ...) while CLN deals with find_package(GMP ...)
# So, conan works with gmp_INCLUDE_DIR and CLN requires GMP_INCLUDE_DIR
# -----------------------------------------------------------------------------------------
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/external/cln/cmake/modules")
# -----------------------------------------------------------
# invoke FindGMP.cmake from CLN
find_package(GMP REQUIRED)
# This is just a small test, to make sure GMP is included in application, at this point.
# target_link_libraries(app_demo PRIVATE gmp::gmp)  # DONE ALREADY!

# Allow GINAC to find CLN source files
set(CLN_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/cln)
# ====================================================

add_subdirectory(external/ginac)
# ------------------------------------
# build ginac into current application
# ------------------------------------
target_link_libraries(app_demo PRIVATE ginac)

#==========
#  TESTS
#----------
Include(FetchContent)
#
FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.3.1)
FetchContent_MakeAvailable(Catch2)
#
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
#
#add_executable(my_test tests/get_data.test.cpp)
#target_link_libraries(my_test PRIVATE brkpg_biblioteca Minha_pasta_external Catch2::Catch2WithMain)
#catch_discover_tests(my_test)


# this is heuristically generated, and may not be correct
add_executable(my_test tests/manydeps_ginac.test.cpp)
#find_package(Catch2 CONFIG REQUIRED)

target_link_libraries(my_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

if(UNIX)
target_link_libraries(my_test PRIVATE gmp::gmp gmp::gmpxx)
else() # WINDOWS?
target_link_libraries(my_test PRIVATE mpir)
endif()

catch_discover_tests(my_test)

