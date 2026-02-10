#include "ScarletEnginePch.h"
#include "IOManager.h"

#include <fstream>
#include <string>

namespace Scarlet
{

void IOManager::LoadTextFile(const char* filePath, std::string& result)
{
    std::ifstream file(filePath, std::ios::in);

    if (file.fail())
    {
        SCARLET_WARN("Failed to open file: {}", filePath);
        return;
    }

    std::string line;

    while (std::getline(file, line, '\n'))
    {
        result += line + "\n";
    }

    file.close();
}


} // Namespace Scarlet.