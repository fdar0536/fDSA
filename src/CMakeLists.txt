cmake_minimum_required(VERSION 3.14)

project(fDSA
    LANGUAGES C CXX
    VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

include(GNUInstallDirs)

configure_file(config.h.in config.h @ONLY)
configure_file(
    cmake/fdsa-config-version.cmake.in
    fDSAConfigVersion.cmake
    @ONLY
)
configure_file(
    cmake/fdsa-config.cmake.in
    fDSAConfig.cmake @ONLY
)

include(fdsa/CMakeLists.txt)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/fDSAConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/fDSAConfig.cmake

    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
)

option(BUILD_TESTING_CASES "Build testing cases" ON)

if (BUILD_TESTING_CASES)
    include(fdsa/test/CMakeLists.txt)
endif(BUILD_TESTING_CASES)
