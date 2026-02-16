#include "ScarletEnginePch.h"
#include "Shader.h"

#include <glew/glew.h>

#include <ScarletMath/Math.h>

#include "Core/IOManager.h"

namespace Scarlet
{

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    mId = glCreateProgram();

    std::string vertexShaderCode;
    IOManager::LoadTextFile(vertexShaderFilePath, vertexShaderCode);

    std::string fragmentShaderCode;
    IOManager::LoadTextFile(fragmentShaderFilePath, fragmentShaderCode);

    CreateShaders(vertexShaderCode, fragmentShaderCode);
}

Shader::~Shader()
{
    glDeleteProgram(mId);
}

void Shader::Bind() const
{
    glUseProgram(mId);
}
void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::CreateShaders(const std::string& vertexSourceCode, const std::string& fragmentSourceCode) const
{
    const uint32 vertexShaderId   = CompileShader(GL_VERTEX_SHADER, vertexSourceCode);
    const uint32 fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentSourceCode);

    SCARLET_ASSERT(vertexShaderId   != 0 && "Failed to compile vertex shader.");
    SCARLET_ASSERT(fragmentShaderId != 0 && "Failed to compile fragment shader.");

    glAttachShader(mId, vertexShaderId);
    glAttachShader(mId, fragmentShaderId);

    glLinkProgram(mId);

    int result;
    glGetProgramiv(mId, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &length);

        char message[512];
        glGetShaderInfoLog(mId, length, &length, message);

        SCARLET_WARN("Failed to link the shader to the program: {}", message);
        glDeleteShader(mId);
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}


uint32 Shader::CompileShader(const uint32 type, const std::string& source)
{
    const uint32 id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, 0);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char message[512];
        glGetShaderInfoLog(id, length, &length, message);

        SCARLET_WARN("Failed to compile Shader at: {}.\nError: {}", source, message);
        glDeleteShader(id);

        return 0;
    }

    return id;
}

template <>
void Shader::UploadUniform(const char* name, const Math::Mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

template <>
void Shader::UploadUniform(const char* name, const int& value)
{
    glUniform1i(GetUniformLocation(name), value);
}

int Shader::GetUniformLocation(const char* name)
{
    if (mUniformLocationCache.contains(name))
    {
        return mUniformLocationCache[name];
    }

    const int location = glGetUniformLocation(mId, name);

    if (location == -1)
    {
        SCARLET_WARN("Shader warning: Uniform '{}' does not exist.", name);
        return -1;
    }

    mUniformLocationCache[name] = location;

    return location;
}

} // Namespace Scarlet.
