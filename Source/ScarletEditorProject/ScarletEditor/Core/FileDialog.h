#pragma once

namespace Scarlet
{

class FileDialog
{
public:
    /**
     * @brief Open an "Open File" dialog. This will allow the user to pick a given file.
     * @param filter The filter for the type of files the file dialog should show.
     * @return The absolute filepath of the chosen file. Empty string if the operation was cancelled.
     */
    [[nodiscard]] static std::string OpenFile(const char* filter);
    /**
     * @brief Open a "Save" dialog. This will allow the user to pick a location to save the document.
     * @param filter The filter for the type of files shows in the dialog.
     * @return The absolute filepath of the chosen file (could be without the extension if the user typed it in). Empty string if the operation was cancelled.
     */
    [[nodiscard]] static std::string SaveAsFile(const char* filter);
};

} // Namespace Scarlet.