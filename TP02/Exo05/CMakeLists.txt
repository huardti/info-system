cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

get_filename_component(ParentDir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(Seance ${ParentDir} NAME)
get_filename_component(Exercice ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(ProjectId ${Seance})
string(APPEND ProjectId "_" "${Exercice}")

string(REPLACE " " "_" ProjectId ${ProjectId})

file(GLOB files_src CONFIGURE_DEPENDS "*.h" "*.c")

add_executable(${ProjectId} ${files_src})