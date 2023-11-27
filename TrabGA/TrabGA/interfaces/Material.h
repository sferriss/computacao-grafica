#pragma once
#include <string>
#include <vector>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

class Material
{
public:
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float q;
    std::vector <std::string> textures;

    Material() = default;

    void initialize(const glm::vec3& ka = glm::vec3(0.4, 0.4, 0.4), const glm::vec3& kd = glm::vec3(0.5, 0.5, 0.5), const glm::vec3& ks = glm::vec3(0.5, 0.5, 0.5), float q = 10.0f, const std::vector<std::string>& textures = std::vector<std::string>());
};
