add_executable(testPtrVector
    main.c
)

add_dependencies(testPtrVector fDSA)
target_link_libraries(testPtrVector PRIVATE fDSA)
target_include_directories(testPtrVector
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

add_test(fDSAPtrVector testPtrVector)
