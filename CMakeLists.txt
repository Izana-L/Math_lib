cmake_minimum_required(VERSION 3.21)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,
      $<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>
      ,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project(Math VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


option(MATH_BUILD_TESTS "Compilar los tests de Math y Math_isolation"
       ${PROJECT_IS_TOP_LEVEL})

option(MATH_INSTALL "Generar las reglas de install de Math"
       ${PROJECT_IS_TOP_LEVEL})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Math_warnings)

include(GNUInstallDirs)

set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/sources/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CODE_DIR}/headers/*.hpp)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/tests/Test_*.cpp)

message(STATUS "CODE_DIR = ${CODE_DIR}")
message(STATUS "SOURCES  = ${SOURCES}")


add_library(Math_glm INTERFACE)
target_include_directories(Math_glm SYSTEM INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
target_compile_definitions(Math_glm INTERFACE
  GLM_FORCE_DEPTH_ZERO_TO_ONE
  GLM_FORCE_QUAT_DATA_XYZW
  GLM_ENABLE_EXPERIMENTAL)

add_library(Math STATIC ${SOURCES} ${HEADERS})

add_library(Math::Math ALIAS Math)

target_include_directories(Math PUBLIC
  $<BUILD_INTERFACE:${CODE_DIR}/headers>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(Math PUBLIC cxx_std_20)

target_link_libraries(Math PRIVATE $<BUILD_INTERFACE:Math_glm>)

set_target_properties(Math PROPERTIES DEBUG_POSTFIX d)
Math_set_warnings(Math)

if(MATH_BUILD_TESTS)
  enable_testing()

  foreach(test_source IN LISTS TEST_SOURCES)
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_include_directories(${test_name} PRIVATE ${CODE_DIR}/sources $
                               {CODE_DIR}/tests)
    target_link_libraries(${test_name} PRIVATE Math Math_glm)
    Math_set_warnings(${test_name})
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "unit" TIMEOUT 30)
  endforeach()

  # Comprobaciones de aislamiento que no son código C++ (CONVENTIONS.md §13).
  add_test(NAME Math_no_glm_in_public_headers
           COMMAND ${CMAKE_COMMAND} -DHEADERS_DIR=${CODE_DIR}/headers -P
                   ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check-no-glm.cmake)
  add_test(NAME Math_umbrella_complete
           COMMAND ${CMAKE_COMMAND} -DHEADERS_DIR=${CODE_DIR}/headers
                    -DUMBRELLA=Math/Math.hpp -P ${CMAKE_CURRENT_SOURCE_DIR}
                    /cmake/check-umbrella.cmake)
  set_tests_properties(Math_no_glm_in_public_headers Math_umbrella_complete
                       PROPERTIES LABELS "isolation")

  add_library(Math_isolation OBJECT ${CODE_DIR}/Math_isolation/Isolation.cpp)
  target_link_libraries(Math_isolation PRIVATE Math)
  Math_set_warnings(Math_isolation)
endif()

if(MATH_INSTALL)
  include(CMakePackageConfigHelpers)


  install(TARGETS Math
          EXPORT MathTargets
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


  install(DIRECTORY ${CODE_DIR}/headers/
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          FILES_MATCHING PATTERN "*.hpp")

  set(MATH_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/Math)

  install(EXPORT MathTargets
          NAMESPACE Math::
          DESTINATION ${MATH_CMAKE_INSTALL_DIR})

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/MathConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake
    INSTALL_DESTINATION ${MATH_CMAKE_INSTALL_DIR})

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
    COMPATIBILITY SameMinorVersion)

  install(FILES
          ${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
          DESTINATION ${MATH_CMAKE_INSTALL_DIR})
endif()


if(PROJECT_IS_TOP_LEVEL AND CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}
    /compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json

    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMENT "Copiando compile_commands.json a la raiz (clangd)"
    VERBATIM)
  add_custom_target(Math_compile_commands ALL DEPENDS ${CMAKE_SOURCE_DIR}
                    /compile_commands.json)
endif()
