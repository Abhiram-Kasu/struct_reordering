foreach(suite IN ITEMS layout values type_identity)
    add_executable(${suite}_tests ${suite}_tests.cpp)
    target_link_libraries(${suite}_tests PRIVATE struct_reordering::struct_reordering)
    add_test(NAME struct_reordering.${suite} COMMAND ${suite}_tests)
endforeach()

target_sources(type_identity_tests PRIVATE type_identity_other.cpp)
