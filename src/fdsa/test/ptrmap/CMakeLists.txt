add_executable(testPtrMap
    main.c
)

add_dependencies(testPtrMap fDSA)
target_link_libraries(testPtrMap PRIVATE fDSA)
target_include_directories(testPtrMap
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

add_test(fDSAMap testPtrMap)
