cmake_minimum_required(VERSION 3.16)
project(lin_algebra_c C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(la STATIC src/la.c)
target_link_libraries(la m)

find_package(PkgConfig)
if(PkgConfig_FOUND)
  pkg_check_modules(SDL2 sdl2)
  if(SDL2_FOUND)
    add_library(viz_sdl STATIC src/viz_sdl.c)
    target_include_directories(viz_sdl PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(viz_sdl ${SDL2_LIBRARIES} m)
    target_compile_definitions(viz_sdl PUBLIC USE_SDL=1)
  endif()
endif()

# lessons/*/main.c 를 자동으로 찾아 각 폴더명으로 실행파일 생성
file(GLOB_RECURSE LESSON_MAINS RELATIVE ${CMAKE_SOURCE_DIR} "lessons/*/main.c")
foreach(MAIN_FILE ${LESSON_MAINS})
  get_filename_component(LESSON_DIR ${MAIN_FILE} DIRECTORY)  # lessons/lec01_vectors
  string(REPLACE "lessons/" "" LESSON_NAME ${LESSON_DIR})    # lec01_vectors
  add_executable(${LESSON_NAME} ${MAIN_FILE})
  target_link_libraries(${LESSON_NAME} la)
  if(TARGET viz_sdl)  # SDL2가 잡혔으면 시각화 유틸도 자동 링크
    target_link_libraries(${LESSON_NAME} viz_sdl)
  endif()
endforeach()
