set(fdsa_public_headers
    include/internal/defines.h
    include/internal/ptrmap.h
    include/internal/ptrvector.h
    include/internal/vector.h
    include/fdsa.h
)

set(fdsa_priv_headers
    fdsa/fdsa.h
    fdsa/ptrmap.h
    fdsa/ptrvector.h
    fdsa/types.h
    fdsa/vector.h

    ${CMAKE_BINARY_DIR}/config.h
)

set(fdsa_src
    fdsa/fdsa.c
    fdsa/init.c
    fdsa/ptrmap.c
    fdsa/ptrvector.c
    fdsa/types.c
    fdsa/vector.c
)

add_library(fDSA
    SHARED

    ${fdsa_public_headers}
    ${fdsa_priv_headers}
    ${fdsa_src}
)

target_include_directories(fDSA
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)
