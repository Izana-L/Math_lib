#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);

    glm::vec3 suma = a + b;
    float punto = glm::dot(a, b);
    glm::vec3 cruz = glm::cross(a, b);

    std::cout << "suma:  " << suma.x << ", " << suma.y << ", " << suma.z << "\n";
    std::cout << "dot:   " << punto << "\n";
    std::cout << "cross: " << cruz.x << ", " << cruz.y << ", " << cruz.z << "\n";

    glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    std::cout << "traslacion x: " << m[3][0] << "\n";
    return 0;
}