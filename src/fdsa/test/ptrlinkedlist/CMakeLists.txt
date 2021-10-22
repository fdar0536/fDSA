add_executable(testPtrLinkedList
    main.c
)

add_dependencies(testPtrLinkedList fDSA)
target_link_libraries(testPtrLinkedList PRIVATE fDSA)
target_include_directories(testPtrLinkedList
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

add_test(fDSAPtrLinkedList testPtrLinkedList)
