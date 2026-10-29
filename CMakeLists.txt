cmake_minimum_required(VERSION 3.21)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(Math LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compile_commands.json para clangd (Zed, VS Code...). Solo lo generan los generadores Ninja/Makefile.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/sources/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CODE_DIR}/headers/*.hpp)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/tests/*.cpp)

# El test de aislamiento vive en tests/ pero no puede compilarse dentro de Math_tests, que tiene glm.
set(ISOLATION_SOURCE ${CODE_DIR}/Math_isolation/Isolation.cpp)
list(REMOVE_ITEM TEST_SOURCES ${ISOLATION_SOURCE})

message(STATUS "CODE_DIR = ${CODE_DIR}")
message(STATUS "SOURCES  = ${SOURCES}")

# glm: el único sitio con su ruta y sus macros. Solo lo enlazan Math y Math_tests, siempre PRIVATE.
add_library(Math_glm INTERFACE)
target_include_directories(Math_glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
target_compile_definitions(Math_glm INTERFACE GLM_ENABLE_EXPERIMENTAL)

add_library(Math STATIC ${SOURCES} ${HEADERS})
target_include_directories(Math PUBLIC ${CODE_DIR}/headers)
target_link_libraries(Math PRIVATE Math_glm)

# Tests: aquí glm sí está permitido, como oráculo.
add_executable(Math_tests ${TEST_SOURCES})
target_link_libraries(Math_tests PRIVATE Math Math_glm)

# Test de aislamiento: solo se compila, no se enlaza ni se ejecuta. No enlaza Math_glm.
add_library(Math_isolation OBJECT ${ISOLATION_SOURCE})
target_link_libraries(Math_isolation PRIVATE Math)
# TODO: Add tests and install targets if needed.
