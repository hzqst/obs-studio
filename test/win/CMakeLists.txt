project(win-test)

add_executable(win-test WIN32)

target_sources(win-test PRIVATE test.cpp)

target_link_libraries(win-test PRIVATE OBS::libobs)
target_link_libraries(win-test PRIVATE OBS::libobs-d3d11)
target_link_libraries(win-test PRIVATE OBS::libobs-winrt)
target_link_libraries(win-test PRIVATE OBS::test-input)

set_target_properties(win-test PROPERTIES FOLDER "tests and examples")