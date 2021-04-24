cmake_minimum_required(VERSION 3.14)

project(fDSA
    LANGUAGES C CXX
    VERSION 0.0.1
)

if (UNIX)
    add_compile_options(-Wall -Werror -Wextra -fvisibility=hidden)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-Weffc++)
    endif ()

    add_link_options(-lm)
    if (NOT APPLE)
        add_link_options(-Wl,--unresolved-symbols=report-all)
    endif (NOT APPLE)
else () # Windows
    if (MINGW)
        add_compile_options(-Wall -Werror -Wextra -fvisibility=hidden)

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(-Weffc++)
        endif ()

        add_link_options(-Wl,--unresolved-symbols=report-all -lm)
    else () # msvc or icc
        add_compile_options(/W4 /WX /wd4819 /wd4996)
    endif (MINGW)
endif (UNIX) # end if (UNIX)

enable_testing()

include(fdsa/CMakeLists.txt)

option(BUILD_TESTING_CASES "Build testing cases" ON)

configure_file(config.h.in config.h @ONLY)

if (BUILD_TESTING_CASES)
    include(fdsa/test/CMakeLists.txt)
endif(BUILD_TESTING_CASES)
