cmake_minimum_required(VERSION 3.8)

# Using /std:c++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(MeasureXercesXqilla)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(FindXQilla)
include(AddXercesC)
include(AddXQilla)

add_subdirectory(ReleaseDomDocument)
add_subdirectory(MeasureXmlPerformance)

# Display configuration summary
message(STATUS "")
message(STATUS "General configuration summary")
message(STATUS "--------------------------------")
message(STATUS "")
message(STATUS "  C compiler:                ${CMAKE_C_COMPILER}")
message(STATUS "  C++ compiler:              ${CMAKE_CXX_COMPILER}")
message(STATUS "  C flags:                   ${CMAKE_C_FLAGS}")
message(STATUS "  C++ flags:                 ${CMAKE_CXX_FLAGS}")
message(STATUS "--------------------------------")
message(STATUS "  XercesC root folder:       ${XercesC_ROOT}")
message(STATUS "  XercesC library:           ${XercesC_LIBRARIES}")
message(STATUS "  XercesC header folder:     ${XercesC_INCLUDE_DIRS}")
message(STATUS "  Xerces binary:             ${XercesC_BIN}")
message(STATUS "--------------------------------")
message(STATUS "  XQilla root folder:        ${XQilla_ROOT}")
message(STATUS "  XQilla library:            ${XQilla_LIBRARIES}")
message(STATUS "  XQilla header folder:      ${XQilla_INCLUDE_DIRS}")
message(STATUS "  XQilla binary:             ${XQilla_BIN}")
