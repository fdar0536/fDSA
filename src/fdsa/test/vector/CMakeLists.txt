add_executable(testVector
    main.c
)

add_dependencies(testVector fDSA)
target_link_libraries(testVector PRIVATE fDSA)
target_include_directories(testVector
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

add_test(SubFXVector fDSA)
