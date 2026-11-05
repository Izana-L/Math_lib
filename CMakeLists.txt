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

# Math_tests y Math_isolation solo se compilan cuando Math es el proyecto principal. Si otro proyecto
# lo usa de librería (add_subdirectory, FetchContent) no se compilan, salvo con -DMATH_BUILD_TESTS=ON.
option(MATH_BUILD_TESTS "Compilar Math_tests y Math_isolation" ${PROJECT_IS_TOP_LEVEL})

set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/sources/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CODE_DIR}/headers/*.hpp)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CODE_DIR}/tests/*.cpp)

message(STATUS "CODE_DIR = ${CODE_DIR}")
message(STATUS "SOURCES  = ${SOURCES}")

# glm: el único sitio con su ruta y sus macros. Solo lo enlazan Math y Math_tests, siempre PRIVATE.
# Isolation.cpp usa GLM_ENABLE_EXPERIMENTAL para detectar si Math_glm se propaga: si lo quitas, cambia ese test.
add_library(Math_glm INTERFACE)
target_include_directories(Math_glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
target_compile_definitions(Math_glm INTERFACE GLM_ENABLE_EXPERIMENTAL)

add_library(Math STATIC ${SOURCES} ${HEADERS})
target_include_directories(Math PUBLIC ${CODE_DIR}/headers)
# Los headers públicos usan C++20 (<concepts>, <numbers>, <=>): quien enlace Math también lo necesita.
target_compile_features(Math PUBLIC cxx_std_20)
# Math es STATIC, así que aun siendo PRIVATE CMake deja Math_glm en su interfaz de enlace
# ($<LINK_ONLY:Math_glm>). Con $<BUILD_INTERFACE:...> se queda dentro de este build: no sale
# en un install(EXPORT) ni le llega a quien use Math con find_package.
target_link_libraries(Math PRIVATE $<BUILD_INTERFACE:Math_glm>)

if(MATH_BUILD_TESTS)
  # Tests: aquí glm sí está permitido, como oráculo.
  add_executable(Math_tests ${TEST_SOURCES})
  target_link_libraries(Math_tests PRIVATE Math Math_glm)

  # Test de aislamiento: solo se compila, no se enlaza ni se ejecuta. Enlaza Math igual que
  # un proyecto que lo use de librería, sin Math_glm.
  add_library(Math_isolation OBJECT ${CODE_DIR}/Math_isolation/Isolation.cpp)
  target_link_libraries(Math_isolation PRIVATE Math)
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
# TODO: Add tests and install targets if needed.
