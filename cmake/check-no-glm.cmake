# Comprobación de aislamiento (CONVENTIONS.md §13, punto 1): ningún header público
# contiene la palabra "glm", ni en código ni en comentarios, en mayúsculas o minúsculas.
# Se ejecuta con: cmake -DHEADERS_DIR=<carpeta> -P check-no-glm.cmake  (lo hace ctest)
cmake_minimum_required(VERSION 3.21)

if(NOT HEADERS_DIR OR NOT IS_DIRECTORY "${HEADERS_DIR}")
  message(FATAL_ERROR "check-no-glm: HEADERS_DIR no es una carpeta: '${HEADERS_DIR}'")
endif()

file(GLOB_RECURSE headers "${HEADERS_DIR}/*")
set(found "")
foreach(header IN LISTS headers)
  # file(STRINGS) no tiene opción de ignorar mayúsculas: se le pasa la clase de cada letra.
  file(STRINGS "${header}" lines REGEX "[Gg][Ll][Mm]")
  foreach(line IN LISTS lines)
    file(RELATIVE_PATH relative "${HEADERS_DIR}" "${header}")
    string(STRIP "${line}" line)
    string(APPEND found "  ${relative}: ${line}\n")
  endforeach()
endforeach()

if(found)
  message(FATAL_ERROR "Los headers publicos mencionan glm:\n${found}")
endif()
message(STATUS "check-no-glm: ningun header publico menciona glm")
