#include "interfaces/Material.h"

void Material::initialize(const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks, float q,
    const std::vector<std::string>& textures)
{
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->q = q;
    this->textures = textures;
}
