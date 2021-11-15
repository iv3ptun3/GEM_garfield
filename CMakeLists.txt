# ########################################################################################
# Use the code in this section for standalone projects
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(tgem)
if(NOT TARGET Garfield::Garfield)
  find_package(Garfield REQUIRED)
endif()
find_package(ROOT REQUIRED)
# ########################################################################################

# ---Build executable------------------------------------------------------------
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} source/*.cpp)

add_executable(simple_track simple_track.cpp ${SOURCES})
add_executable(tgem tgem.cpp ${SOURCES})
add_executable(view_tgem view_tgem.cpp ${SOURCES})

target_link_libraries(simple_track Garfield::Garfield ${ROOT_LIBRARIES})
target_link_libraries(tgem Garfield::Garfield ${ROOT_LIBRARIES})
target_link_libraries(view_tgem Garfield::Garfield ${ROOT_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/include/SRIM/)

# ---Copy all files locally to the build directory-------------------------------
file(GLOB_RECURSE SRIM_DATA RELATIVE ${CMAKE_SOURCE_DIR} SRIM/*.txt)
foreach(_file
  gen_fieldmap.py gen_scripts.py params.txt 
  pysource/parManager.py pysource/geometry.py pysource/startinfo.py pysource/dielectrics.py ${SRIM_DATA})
  configure_file(${_file} ${CMAKE_CURRENT_BINARY_DIR}/${_file} COPYONLY)
endforeach()
