#include "ScarletEnginePch.h"
#include "IOManager.h"

#include <fstream>
#include <string>

namespace Scarlet
{

void IOManager::LoadTextFile(const Filepath& filepath, std::string& result)
{
    SCARLET_ASSERT(filepath.DoesPathExist() && "Trying to load file that doesn't exist on disk.");

    std::ifstream file(filepath.GetAbsolutePath(), std::ios::in);

    if (file.fail())
    {
        SCARLET_WARN("Failed to open file: {}", filepath.GetAbsolutePath());
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