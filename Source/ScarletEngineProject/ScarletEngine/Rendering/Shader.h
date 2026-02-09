#pragma once

namespace Scarlet
{

/**
 * @class Shader A class representing a shader object. This is responsible for loading the source \nn
 * code and compiling into a usable format, as well as storing a unique identifier to bind to the pipeline.
 */
class Shader
{
public:
    /**
     * @brief Construct the shader object by creating the identifier. Load and compile the code for the vertex and fragment shaders.
     * @param vertexShaderFilePath The filepath to the vertex shader code.
     * @param fragmentShaderFilePath The filepath to the fragment shader code.
     */
    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    ~Shader();

    Shader(const Shader&)            = delete;
    Shader(Shader&&)                 = delete;
    Shader& operator=(Shader&&)      = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * @brief Bind the shader so the graphics pipeline uses this shader program.
     */
    void Bind() const;
    /**
     * @brief Unbind the currently bound shader program.
     */
    static void Unbind();
private:
    uint32 mId;

    /**
     * @brief Create the shaders from the source code and link to the shader program identifier.
     * @param vertexSourceCode The source code of the vertex shader.
     * @param fragmentSourceCode The source code of the fragment shader.
     */
    void CreateShaders(const std::string& vertexSourceCode, const std::string& fragmentSourceCode) const;

    /**
     * @brief A function used to compile shader source code.
     * @param type The type of shader: Vertex or Fragment.
     * @param source The source code of the shader.
     * @return The identifier for the compiled shader that can be linked to the shader program identifier.
     */
    static uint32 CompileShader(const uint32 type, const std::string& source);
};

} // Namespace Scarlet.
