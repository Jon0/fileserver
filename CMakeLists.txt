cmake_minimum_required (VERSION 2.6)
project (PM)


# std threads
find_package(Threads REQUIRED)


# build core library
set(CORE_LIBRARY_NAME "core")
add_subdirectory(core)


# build main library
set(MAIN_LIBRARY_NAME "fileserver")
add_subdirectory(fileserver)
include_directories("${PROJECT_SOURCE_DIR}/")


# build modules
add_subdirectory(modules)

#build executable
add_subdirectory(bin)
