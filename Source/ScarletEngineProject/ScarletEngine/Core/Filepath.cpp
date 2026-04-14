#include "ScarletEnginePch.h"
#include "Filepath.h"

namespace Scarlet
{

std::filesystem::path Filepath::mScarletEngineDirectory  = std::filesystem::path{ SCARLET_ENGINE_PATH  };
std::filesystem::path Filepath::mScarletProjectDirectory = std::filesystem::path{ "" };

} // Namespace Scarlet.