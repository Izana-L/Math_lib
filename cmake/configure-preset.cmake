# Uso: cmake -DPRESET=<nombre> -P cmake/configure-preset.cmake
#
# Configura el proyecto con un preset de CMakePresets.json y deja una copia de
# compile_commands.json en la raíz, que es donde lo busca clangd (Zed).
# Se hace con un script de CMake y no con "cmd1 && cmd2" para que funcione igual
# en PowerShell, cmd, bash...
cmake_minimum_required(VERSION 3.21)

if(NOT PRESET)
  message(FATAL_ERROR "Falta -DPRESET=<nombre de un preset de CMakePresets.json>")
endif()

get_filename_component(ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

get_filename_component(ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
# Debe coincidir con "binaryDir" de los presets (projects/out/build/${presetName}).
set(BUILD_DIR "${ROOT}/projects/out/build/${PRESET}")

# Todos los presets usan el generador Ninja.
find_program(NINJA NAMES ninja ninja-build samu)
if(NOT NINJA)
  message(FATAL_ERROR
    "No se encuentra ninja en el PATH. Instálalo:\n"
    "  Arch / CachyOS:        sudo pacman -S --needed cmake ninja gcc\n"
    "  Debian / Ubuntu / WSL: sudo apt install cmake ninja-build g++\n"
    "  Windows:               abre Zed desde una \"Developer PowerShell for VS\"")
endif()

# Una carpeta de build generada en otra máquina (p. ej. copiada desde WSL) guarda
# en CMakeCache.txt rutas que aquí pueden no existir (el proyecto, /usr/bin/ninja,
# el compilador...) y entonces no configura ni compila: se borra y se empieza de cero.
if(EXISTS "${BUILD_DIR}/CMakeCache.txt")
  file(STRINGS "${BUILD_DIR}/CMakeCache.txt" CACHED_PATHS
       REGEX "^CMAKE_(HOME_DIRECTORY|MAKE_PROGRAM|CXX_COMPILER):")
  foreach(entry IN LISTS CACHED_PATHS)
    string(REGEX REPLACE "^[^=]*=" "" path "${entry}")
    if(IS_ABSOLUTE "${path}" AND NOT EXISTS "${path}")
      message(STATUS "${path} no existe en esta máquina: se borra ${BUILD_DIR}")
      file(REMOVE_RECURSE "${BUILD_DIR}")
      break()
    endif()
  endforeach()
endif()

execute_process(COMMAND "${CMAKE_COMMAND}" --preset "${PRESET}"
                        WORKING_DIRECTORY "${ROOT}"
                        COMMAND_ERROR_IS_FATAL ANY)

set(COMPDB "${BUILD_DIR}/compile_commands.json")

set(COMPDB "${ROOT}/projects/out/build/${PRESET}/compile_commands.json")
if(NOT EXISTS "${COMPDB}")
  message(FATAL_ERROR "No existe ${COMPDB}. ¿Se usa un generador Ninja y CMAKE_EXPORT_COMPILE_COMMANDS=ON?")
endif()

file(COPY_FILE "${COMPDB}" "${ROOT}/compile_commands.json" ONLY_IF_DIFFERENT)
message(STATUS "compile_commands.json actualizado desde el preset '${PRESET}'")
