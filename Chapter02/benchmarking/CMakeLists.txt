cmake_minimum_required(VERSION 3.8)

project(Chapter02-Essential_CPP_Techniques_Benchmarking)


file(
  GLOB
  CHAPTER_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  )

add_executable(${PROJECT_NAME} ${CHAPTER_SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE benchmark::benchmark)
