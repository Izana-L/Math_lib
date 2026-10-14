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

execute_process(
  COMMAND "${CMAKE_COMMAND}" --preset "${PRESET}"
  WORKING_DIRECTORY "${ROOT}"
  COMMAND_ERROR_IS_FATAL ANY)

# Debe coincidir con "binaryDir" de los presets (projects/out/build/${presetName}).
set(COMPDB "${ROOT}/projects/out/build/${PRESET}/compile_commands.json")
if(NOT EXISTS "${COMPDB}")
  message(FATAL_ERROR "No existe ${COMPDB}. ¿Se usa un generador Ninja y CMAKE_EXPORT_COMPILE_COMMANDS=ON?")
endif()

file(COPY_FILE "${COMPDB}" "${ROOT}/compile_commands.json" ONLY_IF_DIFFERENT)
message(STATUS "compile_commands.json actualizado desde el preset '${PRESET}'")
