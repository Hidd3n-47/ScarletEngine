#pragma once

#include <ScarletEngine/Core/Defines.h>

#include <string>
#include <filesystem>

namespace Scarlet
{

enum class FilepathDirectory : uint8
{
    ENGINE,

    PROJECT
};

/**
 * @class Filepath: A class that can be used to create paths for files relative to the project folder.
 * @note \ref Filepath represents a file and hence must have an extension. A \ref Filepath is not a directory.
 */
class ENGINE_API Filepath
{
public:
    Filepath()  = default;
    ~Filepath() = default;

    explicit Filepath(const FilepathDirectory directory, const char* path)        : mRelativePath{ path }, mDirectory{ directory } { /* Empty. */ }
    explicit Filepath(const FilepathDirectory directory, const std::string& path) : mRelativePath{ path }, mDirectory{ directory } { /* Empty. */ }
    explicit Filepath(const FilepathDirectory directory, const std::filesystem::path& path) : mRelativePath{ path }, mDirectory{ directory } { /* Empty. */ }

    Filepath(const Filepath&)            = default;
    Filepath(Filepath&&)                 = default;
    Filepath& operator=(const Filepath&) = default;
    Filepath& operator=(Filepath&&)      = default;

    /**
     * @brief Get the file name of the \ref Filepath. Note, the file name includes extensions.
     * @return The filename of the \ref Filepath.
     */
    [[nodiscard]] inline std::string GetFileName() const { return mRelativePath.filename().string(); }

    /**
     * Get the absolute (full) filepath of the file on disk.
     * @return The absolute (full) filepath of the file on disk.
     */
    [[nodiscard]] inline std::string GetAbsolutePath() const
    {
        return ((mDirectory == FilepathDirectory::ENGINE ?
                 mScarletEngineDirectory : mScarletProjectDirectory) / mRelativePath).string();
    }

    /**
     * @brief Get the relative path from the root directory (determined from \ref FilepathDirectory).
     * @return Get the relative path from the root directory (determined from \ref FilepathDirectory).
     */
    [[nodiscard]] inline std::string GetRelativePath() const { return mRelativePath.string(); }

    /**
     * @brief Set the relative path of the \ref Filepath from the root directory (determined from \ref FilepathDirectory).
     * @param relativePath The relative of the filepath from the root directory (determined from \ref FilepathDirectory).
     */
    inline void SetRelativePath(const std::filesystem::path& relativePath)
    {
        SCARLET_ASSERT(relativePath.is_relative() && "Cannot set the relative equal to a non-relative path.");

        mRelativePath = relativePath;
    }

    /**
     * Get the extension of the file.
     * @return The extension of the file.
     */
    [[nodiscard]] inline std::string GetExtension() const { return mRelativePath.extension().string(); }
    /**
     * Check if the path exists on disk and is valid.
     * @return \c true if the path exists on disk, \c false otherwise.
     */
    [[nodiscard]] inline bool DoesPathExist() const { return std::filesystem::exists(GetAbsolutePath()); }

    [[nodiscard]] inline bool operator==(const Filepath& other) const { return mRelativePath == other.mRelativePath; }

    inline Filepath operator/(const std::string& path) const { return *this / std::filesystem::path{ path }; }

    inline Filepath operator/(const std::filesystem::path& path) const
    {
        Filepath newPath{};
        newPath.mDirectory    = mDirectory;
        newPath.mRelativePath = mRelativePath / path;

        return newPath;
    }

    inline static const std::filesystem::path& GetProjectDirectory() { return mScarletProjectDirectory; }
    inline static void SetEngineDirectory (const std::filesystem::path& engineDirectory ) { mScarletEngineDirectory = engineDirectory; }
    inline static void SetProjectDirectory(const std::filesystem::path& projectDirectory) { SCARLET_INFO("Project path set to: {}", projectDirectory.string()); mScarletProjectDirectory = projectDirectory; }

private:
    std::filesystem::path mRelativePath{ };
    FilepathDirectory     mDirectory   { FilepathDirectory::ENGINE };

    static std::filesystem::path mScarletEngineDirectory;
    static std::filesystem::path mScarletProjectDirectory;
};

} // Namespace Scarlet.

template <>
struct std::hash<Scarlet::Filepath>
{
    [[nodiscard]] inline size_t operator()(const Scarlet::Filepath& path) const noexcept
    {
        return std::hash<std::string>()(path.GetAbsolutePath());
    }
};