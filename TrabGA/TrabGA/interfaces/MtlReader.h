#pragma once
#include <string>

#include "Material.h"

using namespace std;

class MtlReader
{
public:
    Material material;

    void read(string filename);
};
