##################
# Target
get_filename_component(DIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" TARGET_MAIN ${DIRECTORY_NAME})
set(TARGET_MAIN ${TARGET_MAIN}_tests)

####################
# Sources & headers
aux_source_directory(. SRC_LIST)
file(GLOB HEADERS_LIST "*.h" "*.hpp")

find_package(TBB CONFIG REQUIRED)

add_executable(${TARGET_MAIN} ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${TARGET_MAIN} PRIVATE Catch2::Catch2WithMain TBB::tbb TBB::tbbmalloc)

catch_discover_tests(${TARGET_MAIN})