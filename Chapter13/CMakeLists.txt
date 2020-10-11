cmake_minimum_required (VERSION 3.8)

project (Chapter13-Asynchronous_Programming_With_Coroutines)

#if (MSVC)
add_definitions(-DBOOST_ASIO_DISABLE_CONCEPTS)
#endif()


file(GLOB CHAPTER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(FILTER CHAPTER_SRC_FILES EXCLUDE REGEX ".*asio_server.cpp")
message("${CHAPTER_SRC_FILES}")
add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})


# Build asio server as a standalone program
#add_executable(asio_server ./asio_server.cpp)
file(GLOB SERVER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/asio_server.cpp")
#list(APPEND SERVER_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/asio_server.cpp")
message("${SERVER_SRC_FILES}")
add_executable("asio_server" ${SERVER_SRC_FILES})

#add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})
target_link_libraries("asio_server" GTest::gtest)
target_link_libraries(${PROJECT_NAME} GTest::gtest)



