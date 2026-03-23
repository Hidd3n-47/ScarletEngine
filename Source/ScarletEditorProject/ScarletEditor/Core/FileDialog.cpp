#include "ScarletEditorPch.h"
#include "FileDialog.h"

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>

namespace Scarlet
{

std::string FileDialog::OpenFile(const char* filter)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { };

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize  = sizeof(OPENFILENAME);
    ofn.hwndOwner    = glfwGetWin32Window(static_cast<GLFWwindow*>(Engine::Instance().GetMainWindow()->GetNativeWindow()));
    ofn.lpstrFile    = szFile;
    ofn.nMaxFile     = sizeof(szFile);
    ofn.lpstrFilter  = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return std::string{};
}

std::string FileDialog::OpenFolder()
{
    BROWSEINFOA browseInfo = { };
    browseInfo.hwndOwner   = glfwGetWin32Window(static_cast<GLFWwindow*>(Engine::Instance().GetMainWindow()->GetNativeWindow()));
    browseInfo.lpszTitle   = "Select a folder";
    browseInfo.ulFlags     = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pathList = SHBrowseForFolderA(&browseInfo);

    char path[MAX_PATH];
    if (SHGetPathFromIDListA(pathList, path))
    {
        return std::string{ path };
    }

    return std::string{};
}

std::string FileDialog::SaveAsFile(const char* filter)
{
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { };

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize  = sizeof(OPENFILENAME);
    ofn.hwndOwner    = glfwGetWin32Window(static_cast<GLFWwindow*>(Engine::Instance().GetMainWindow()->GetNativeWindow()));
    ofn.lpstrFile    = szFile;
    ofn.nMaxFile     = sizeof(szFile);
    ofn.lpstrFilter  = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn) == TRUE)
    {
        return ofn.lpstrFile;
    }

    return std::string{};
}

} // Namespace Scarlet.