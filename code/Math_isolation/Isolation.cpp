// Test de aislamiento de glm (CONVENTIONS.md §13).
// No se ejecuta: basta con que compile. Su target solo enlaza Math, así que no
// tiene la ruta de glm. Si un header público incluye glm, este archivo no compila.

#if __has_include(<glm/glm.hpp>)
#error "Math_isolation ve glm: o Math lo enlaza como PUBLIC en vez de PRIVATE, o glm esta instalado en el sistema. Asi el test no demuestra nada."
#endif

#include <Math/Angle.hpp>
#include <Math/Constants.hpp>
#include <Math/Hash.hpp>
#include <Math/Matrix3.hpp>
#include <Math/Matrix4.hpp>
#include <Math/Quaternion.hpp>
#include <Math/Scalar.hpp>
#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>
