// Test de aislamiento de glm (CONVENTIONS.md §13).
// No se ejecuta: basta con que compile. Su target solo enlaza Math, igual que un
// proyecto que use Math de librería, y comprueba que a ese proyecto no le llega glm:
//
//   1. El target Math no le pasa Math_glm (ni la ruta de glm ni sus macros).
//   2. Ningún header público de Math incluye glm, ni directa ni indirectamente.
//
// No usa __has_include(<glm/glm.hpp>): eso también da positivo si glm está
// instalado en el sistema (p. ej. /usr/include/glm), aunque Math esté bien aislado.

// 1. GLM_ENABLE_EXPERIMENTAL solo lo define Math_glm, y llega siempre junto con su
//    ruta de include. Se mira antes de los #include para ver solo lo que llega por
//    la línea de comandos, es decir, lo que propaga el target Math.
//    Si solo lo marca el editor y compilar funciona, clangd no encuentra este archivo
//    en compile_commands.json y le aplica los flags de otro: compila para actualizarlo.
#if defined(GLM_ENABLE_EXPERIMENTAL) || defined(GLM_FORCE_DEPTH_ZERO_TO_ONE) || defined(GLM_FORCE_QUAT_DATA_XYZW)
#error "Math propaga Math_glm a quien lo enlaza: debe enlazarlo como PRIVATE. Si solo sale en el editor, compile_commands.json esta desactualizado."
#endif

#include <Math/Math.hpp>

// 2. Todos los headers de glm incluyen glm/detail/setup.hpp, que define GLM_SETUP_INCLUDED.
//    Si glm no está instalado en el sistema, el #include de glm ya falla antes de llegar aquí.
#ifdef GLM_SETUP_INCLUDED
#error "Un header publico de Math incluye glm: quien use Math de libreria tambien necesitaria glm."
#endif
