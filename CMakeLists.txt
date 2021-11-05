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
set(SOURCES source/ParManager.cpp source/TGemViewerField.cpp source/TGemViewerDrift.cpp
  source/FieldMapBuilder.cpp)

add_executable(tgem tgem.cpp ${SOURCES})
add_executable(view_tgem view_tgem.cpp ${SOURCES})

target_link_libraries(tgem Garfield::Garfield ${ROOT_LIBRARIES})
target_link_libraries(view_tgem Garfield::Garfield ${ROOT_LIBRARIES})

target_include_directories(tgem PRIVATE ${PROJECT_SOURCE_DIR}/include/)
target_include_directories(view_tgem PRIVATE ${PROJECT_SOURCE_DIR}/include/)


# ---Copy all files locally to the build directory-------------------------------
foreach(_file
  gen_fieldmap.py gen_scripts.py params.txt 
  pysource/parManager.py pysource/geometry.py pysource/startinfo.py pysource/dielectrics.py)
  configure_file(${_file} ${CMAKE_CURRENT_BINARY_DIR}/${_file} COPYONLY)
endforeach()
