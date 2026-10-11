cmake_minimum_required(VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(Math LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/sources/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CODE_DIR}/headers/*.hpp)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/tests/*.cpp)

message(STATUS "CODE_DIR = ${CODE_DIR}")
message(STATUS "SOURCES  = ${SOURCES}")

add_library(Math STATIC ${SOURCES} ${HEADERS})

# GLM (header-only)
target_include_directories(Math SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
target_compile_definitions(Math PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_include_directories(Math PUBLIC ${CODE_DIR}/headers)


add_executable(Math_tests ${TEST_SOURCES})

target_link_libraries(Math_tests PRIVATE Math )
# TODO: Add tests and install targets if needed.