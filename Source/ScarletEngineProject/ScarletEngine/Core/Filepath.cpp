#include "ScarletEnginePch.h"
#include "Filepath.h"

namespace Scarlet
{

const std::filesystem::path Filepath::SCARLET_ENGINE_DIRECTORY = std::filesystem::path{ SCARLET_ENGINE_PATH  };
std::filesystem::path      Filepath::mScarletProjectDirectory  = std::filesystem::path{ "" };

} // Namespace Scarlet.