cmake_minimum_required(VERSION 3.20)

project(cathedral)
set(CMAKE_CXX_CLANG_FORMAT "clang-format -style=clang-format.json -fallback-style=Google")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependencies
include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static library" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable testing" FORCE)
set(CMARK_LIB_FUZZER OFF CACHE BOOL "Disable fuzzing" FORCE)

FetchContent_Declare(
    cmark
    GIT_REPOSITORY https://github.com/commonmark/cmark.git
    GIT_TAG 0.31.1
)

FetchContent_GetProperties(cmark)
if(NOT cmark_POPULATED)
    FetchContent_Populate(cmark)
    
    # We need to process the main CMakeLists.txt to get the function definitions
    # but we'll override the add_subdirectory commands
    set(PREV_BUILD_TESTING ${BUILD_TESTING})
    set(BUILD_TESTING OFF)
    add_subdirectory(${cmark_SOURCE_DIR} ${cmark_BINARY_DIR} EXCLUDE_FROM_ALL)
    set(BUILD_TESTING ${PREV_BUILD_TESTING})
endif()
# End Dependencies

# Build
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE C_SRC_FILES src/*.c)

# target_link_libraries(cathedral PRIVATE dl) # For when we add back sqlite
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # #######################
    # DEBUG         #
    # #######################
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -fsanitize=address,leak,undefined -fno-omit-frame-pointer -fasynchronous-unwind-tables -no-pie -ggdb3 -Wall -flto=auto")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address,leak,undefined -fno-omit-frame-pointer -no-pie -Wall -fno-omit-frame-pointer")

    # #######################
    # RELEASE       #
    # #######################
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIE -D_FORTIFY_SOURCE=2 -fexceptions -Wall -O2 -flto=auto -funroll-loops")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_LINKER_FLAGS_RELEASE} -fPIE  -s")

# -static -static-libgcc -static-libstdc++
elseif(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /GL")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
endif()

add_executable(cathedral ${SRC_FILES} ${C_SRC_FILES})

target_link_libraries(cathedral PRIVATE ${CMAKE_CXX_STANDARD_LIBRARIES} pthread cmark)

install(TARGETS cathedral RUNTIME DESTINATION bin)
