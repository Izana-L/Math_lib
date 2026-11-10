
cmake_minimum_required(VERSION 3.21)

if(NOT EXISTS "${HEADERS_DIR}/${UMBRELLA}")
  message(FATAL_ERROR "check-umbrella: no existe ${HEADERS_DIR}/${UMBRELLA}")
endif()

file(STRINGS "${HEADERS_DIR}/${UMBRELLA}" includes REGEX
     "^[ \t]*#[ \t]*include[ \t]*<[^>]+>")
set(included "")
foreach(line IN LISTS includes)
  string(REGEX REPLACE "^[ \t]*#[ \t]*include[ \t]*<([^>]+)>.*$"
         "\\1" path "${line}")
  list(APPEND included "${path}")
endforeach()

file(GLOB_RECURSE headers RELATIVE "${HEADERS_DIR}" "${HEADERS_DIR}/*.hpp")
set(missing "")
foreach(header IN LISTS headers)
  if(NOT header STREQUAL UMBRELLA AND NOT header IN_LIST included)
    string(APPEND missing "  #include <${header}>\n")
  endif()
endforeach()

if(missing)
  message(FATAL_ERROR "A ${UMBRELLA} le faltan estos includes:\n${missing}")
endif()
message(STATUS "check-umbrella: ${UMBRELLA} incluye todos los headers publicos")
