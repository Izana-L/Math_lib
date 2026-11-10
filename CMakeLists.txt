cmake_minimum_required(VERSION 3.21)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# VERSION la usa el archivo MathConfigVersion.cmake que se instala (find_package(Math 0.1)).
project(Math VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compile_commands.json para clangd (Zed, VS Code...). Solo lo generan los generadores Ninja/Makefile.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Math_tests y Math_isolation solo se compilan cuando Math es el proyecto principal. Si otro proyecto
# lo usa de librería (add_subdirectory, FetchContent) no se compilan, salvo con -DMATH_BUILD_TESTS=ON.
option(MATH_BUILD_TESTS "Compilar los tests de Math y Math_isolation" ${PROJECT_IS_TOP_LEVEL})
# Lo mismo con las reglas de install: si otro proyecto incluye Math con add_subdirectory, su
# "cmake --install" no instala Math salvo que lo pida con -DMATH_INSTALL=ON.
option(MATH_INSTALL "Generar las reglas de install de Math" ${PROJECT_IS_TOP_LEVEL})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Math_warnings)
# CMAKE_INSTALL_INCLUDEDIR, CMAKE_INSTALL_LIBDIR...: hace falta antes de target_include_directories.
include(GNUInstallDirs)

set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/sources/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CODE_DIR}/headers/*.hpp)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/tests/Test_*.cpp)

message(STATUS "CODE_DIR = ${CODE_DIR}")
message(STATUS "SOURCES  = ${SOURCES}")

# glm: el único sitio con su ruta y sus macros. Solo lo enlazan Math y los tests, siempre PRIVATE.
# Las macros se definen aquí, no en un header, para que todas las unidades de traducción que ven
# glm tengan la misma configuración. code/sources/Glm_config.hpp comprueba que han llegado.
#   GLM_FORCE_DEPTH_ZERO_TO_ONE: profundidad 0..1 de Vulkan (CONVENTIONS.md §4).
#   GLM_FORCE_QUAT_DATA_XYZW:    el constructor de glm::quat pasa a ser (x, y, z, w) (§6).
#   GLM_ENABLE_EXPERIMENTAL:     permite los headers gtx.
# Isolation.cpp mira estas macros para detectar si Math_glm se propaga: si cambias la lista, cambia ese test.
add_library(Math_glm INTERFACE)
target_include_directories(Math_glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
target_compile_definitions(Math_glm INTERFACE
  GLM_FORCE_DEPTH_ZERO_TO_ONE
  GLM_FORCE_QUAT_DATA_XYZW
  GLM_ENABLE_EXPERIMENTAL)

add_library(Math STATIC ${SOURCES} ${HEADERS})
# Mismo nombre que da find_package: quien use Math escribe Math::Math tanto si lo incluye con
# add_subdirectory/FetchContent como si lo instala.
add_library(Math::Math ALIAS Math)
# BUILD_INTERFACE: en este build los headers están en code/headers. INSTALL_INTERFACE: una vez
# instalado, en <prefijo>/include. Una ruta absoluta del código fuente no se puede exportar.
target_include_directories(Math PUBLIC
  $<BUILD_INTERFACE:${CODE_DIR}/headers>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
# Los headers públicos usan C++20 (<concepts>, <numbers>, <=>): quien enlace Math también lo necesita.
target_compile_features(Math PUBLIC cxx_std_20)
# Math es STATIC, así que aun siendo PRIVATE CMake deja Math_glm en su interfaz de enlace
# ($<LINK_ONLY:Math_glm>). Con $<BUILD_INTERFACE:...> se queda dentro de este build: no sale
# en un install(EXPORT) ni le llega a quien use Math con find_package.
target_link_libraries(Math PRIVATE $<BUILD_INTERFACE:Math_glm>)
# Con Debug y Release instalados en el mismo prefijo, la librería Debug se llama Mathd y no pisa
# a la Release. find_package elige la que corresponde a la configuración de quien la usa.
set_target_properties(Math PROPERTIES DEBUG_POSTFIX d)
Math_set_warnings(Math)

if(MATH_BUILD_TESTS)
  enable_testing()

  # Un ejecutable por cada code/tests/Test_*.cpp, registrado en ctest con su nombre.
  # Aquí glm sí está permitido, como oráculo. code/sources está en el include path para que los
  # tests usen los headers privados Glm_config.hpp y Glm_convert.hpp.
  foreach(test_source IN LISTS TEST_SOURCES)
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_include_directories(${test_name} PRIVATE ${CODE_DIR}/sources ${CODE_DIR}/tests)
    target_link_libraries(${test_name} PRIVATE Math Math_glm)
    Math_set_warnings(${test_name})
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "unit" TIMEOUT 30)
  endforeach()

  # Comprobaciones de aislamiento que no son código C++ (CONVENTIONS.md §13).
  add_test(NAME Math_no_glm_in_public_headers
           COMMAND ${CMAKE_COMMAND} -DHEADERS_DIR=${CODE_DIR}/headers -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check-no-glm.cmake)
  add_test(NAME Math_umbrella_complete
           COMMAND ${CMAKE_COMMAND} -DHEADERS_DIR=${CODE_DIR}/headers -DUMBRELLA=Math/Math.hpp -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check-umbrella.cmake)
  set_tests_properties(Math_no_glm_in_public_headers Math_umbrella_complete PROPERTIES LABELS "isolation")

  # Test de aislamiento: solo se compila, no se enlaza ni se ejecuta. Enlaza Math igual que
  # un proyecto que lo use de librería, sin Math_glm. Si falla, falla "cmake --build".
  add_library(Math_isolation OBJECT ${CODE_DIR}/Math_isolation/Isolation.cpp)
  target_link_libraries(Math_isolation PRIVATE Math)
  Math_set_warnings(Math_isolation)
endif()

if(MATH_INSTALL)
  include(CMakePackageConfigHelpers)

  # La librería, y el export set MathTargets que describe el target para find_package.
  install(TARGETS Math
          EXPORT MathTargets
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  # Solo los headers públicos. Los privados (code/sources/*.hpp) y glm no se instalan nunca.
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
  # SameMinorVersion: mientras la versión sea 0.x, un cambio de minor puede romper la API.
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
    COMPATIBILITY SameMinorVersion)

  install(FILES
          ${CMAKE_CURRENT_BINARY_DIR}/MathConfig.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/MathConfigVersion.cmake
          DESTINATION ${MATH_CMAKE_INSTALL_DIR})
endif()

# clangd (Zed) lee la copia de compile_commands.json de la raíz. cmake/configure-preset.cmake la
# crea al configurar, pero "cmake --build" también reconfigura (al cambiar un CMakeLists.txt o al
# añadir un .cpp) y la copia se quedaba vieja: clangd no encontraba Isolation.cpp, le aplicaba los
# flags de Math.cpp (que tienen glm) y marcaba el #error. Así se actualiza también en cada build.
if(PROJECT_IS_TOP_LEVEL AND CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMENT "Copiando compile_commands.json a la raiz (clangd)"
    VERBATIM)
  add_custom_target(Math_compile_commands ALL DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
