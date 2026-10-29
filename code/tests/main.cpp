#include <iostream>
#include <glm/gtc/constants.hpp>
#include <Math/Constants.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector2.hpp>
#include <Math/Vector4.hpp>

int main()
{

    Math::Vector3 v3;
    Math::Vector2 v2;
    Math::Vector4 v4;

    // glm como oráculo: Math y glm tienen que dar el mismo valor.
    if (Math::Pi != glm::pi<float>())
    {
        std::cout << "Math_tests: FALLO, Math::Pi no coincide con glm::pi\n";
        return 1;
    }

    std::cout << "Math_tests: OK\n";
    return 0;
}
