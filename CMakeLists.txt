# CMakeList.txt: проект CMake для bnv-git-hook; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("bnv-git-hook")
set(CMAKE_CXX_STANDARD 17)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (bnv-git-hook "bnv-git-hook.cpp" "bnv-git-hook.h" "GitManager.cpp" "GitManager.h" "INIReader.cpp" "INIReader.h" "ini.c" "ini.h" "BnvHelper.cpp" "BnvHelper.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
