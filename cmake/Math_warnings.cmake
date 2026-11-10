# Warnings del proyecto Math. Se aplican solo a los targets de este repositorio y siempre
# como PRIVATE: no se imponen a quien use Math de librería.
#
#   Math_set_warnings(<target>)
#
# Con -DMATH_WARNINGS_AS_ERRORS=ON los warnings son errores (útil en CI).

option(MATH_WARNINGS_AS_ERRORS "Tratar los warnings de Math como errores" OFF)

function(Math_set_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE
      /W4                 # nivel alto
      /permissive-        # conformidad estricta con el estándar
      /utf-8              # los fuentes son UTF-8 (comentarios con tildes)
      /Zc:__cplusplus     # __cplusplus con el valor real
      /Zc:preprocessor    # preprocesador conforme
      /w14242 /w14254     # conversiones con pérdida que /W4 no avisa (parecido a -Wconversion)
      /w14826             # conversión con extensión de signo
      $<$<BOOL:${MATH_WARNINGS_AS_ERRORS}>:/WX>)
  else() # GCC y Clang
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic
      -Wconversion -Wsign-conversion  # conversiones con pérdida (float <-> int, signed <-> unsigned)
      -Wdouble-promotion              # float que se promociona a double sin querer
      -Wshadow                        # variables que tapan a otras
      -Wold-style-cast -Wcast-align -Wcast-qual
      -Wnon-virtual-dtor -Woverloaded-virtual
      -Wnull-dereference -Wformat=2 -Wimplicit-fallthrough
      -Wundef                         # #if con una macro sin definir
      $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast>
      $<$<BOOL:${MATH_WARNINGS_AS_ERRORS}>:-Werror>)
  endif()
endfunction()
