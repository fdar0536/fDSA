set(fdsa_public_headers
    include/internal/defines.h
    include/internal/ptrlinkedlist.h
    include/internal/ptrmap.h
    include/internal/ptrvector.h
    include/internal/vector.h
    include/fdsa.h
)

set(fdsa_priv_headers
    fdsa/fdsa.h
    fdsa/ptrlinkedlist.h
    fdsa/ptrmap.h
    fdsa/ptrvector.h
    fdsa/vector.h

    ${CMAKE_BINARY_DIR}/config.h
)

set(fdsa_src
    fdsa/fdsa.c
    fdsa/init.c
    fdsa/ptrlinkedlist.c
    fdsa/ptrmap.c
    fdsa/ptrvector.c
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

#install
set_target_properties(fDSA
    PROPERTIES

    VERSION ${PROJECT_VERSION}

    PUBLIC_HEADER
    "${CMAKE_SOURCE_DIR}/include/fdsa.h"

    PRIVATE_HEADER
    "${CMAKE_SOURCE_DIR}/include/internal/defines.h;\
${CMAKE_SOURCE_DIR}/include/internal/ptrlinkedlist.h;\
${CMAKE_SOURCE_DIR}/include/internal/ptrmap.h;\
${CMAKE_SOURCE_DIR}/include/internal/ptrvector.h;\
${CMAKE_SOURCE_DIR}/include/internal/vector.h"
)

install(
    TARGETS fDSA
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    EXPORT fDSATargets
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    LIBRARY
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
    COMPONENT
    runtime

    ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT
    devel

    PUBLIC_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/fdsa
    COMPONENT
    devel

    PRIVATE_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/fdsa/internal
    COMPONENT
    devel
)

install(
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake
    NAMESPACE ${CMAKE_PROJECT_NAME}::
    COMPONENT devel
)
