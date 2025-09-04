cmake_minimum_required(VERSION 3.16)
project(lin_algebra_c C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(la STATIC src/la.c)
target_link_libraries(la m)

find_package(PkgConfig REQUIRED)

# SDL2
pkg_check_modules(SDL2 REQUIRED sdl2)

# SDL2_ttf (텍스트 렌더링용)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# viz_sdl: SDL2(+TTF) 사용하는 렌더 유틸
add_library(viz_sdl STATIC src/viz_sdl.c)
target_include_directories(viz_sdl PUBLIC
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
)
target_link_libraries(viz_sdl PUBLIC
  ${SDL2_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  m
)
target_compile_definitions(viz_sdl PUBLIC USE_SDL=1)

# lessons/*/main.c 자동 실행파일
file(GLOB_RECURSE LESSON_MAINS RELATIVE ${CMAKE_SOURCE_DIR} "lessons/*/main.c")
foreach(MAIN_FILE ${LESSON_MAINS})
  get_filename_component(LESSON_DIR ${MAIN_FILE} DIRECTORY)  # lessons/lec01_vectors
  string(REPLACE "lessons/" "" LESSON_NAME ${LESSON_DIR})    # lec01_vectors
  add_executable(${LESSON_NAME} ${MAIN_FILE})
  target_link_libraries(${LESSON_NAME} la)
  target_link_libraries(${LESSON_NAME} viz_sdl)  # SDL 시각화 자동 링크
endforeach()
