cmake_minimum_required(VERSION 3.0.0)
project(new_project VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include)
file(GLOB TARGET_SRC "./src/*.cpp" )

#add_executable(main ${TARGET_SRC})

add_library(adpcm STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/adpcm.cpp
)
target_include_directories(adpcm PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
