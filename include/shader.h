#ifndef BEG_SHADER_H
#define BEG_SHADER_H

#include <bmath.h>

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>

namespace BEG {

class Shader {
private:
    unsigned int mProgramId{}; /* GL shader program ID */

    /* compile a shader program from source and return it's ID */
    static unsigned int compileShader(GLenum type, const std::string& src);
public:
    enum class ShaderErrorType {
        FileDoesNotExistError,
        FileReadError,

        VertexCompilationError,
        FragmentCompilationError,
        ProgramLinkingError,

        InvalidUniformType
    };

    struct ShaderError {
        ShaderErrorType type;
        std::string info{};
    };

    static std::string errorTypeToString(ShaderErrorType err);

    /* --- */

    /* load a vertex and fragment shader from their paths and pass their source into the Shader constructor */
    static Shader fromPaths(const std::string& vertPath, const std::string& fragPath);

    Shader(unsigned int id = 0) : mProgramId{ id } {}
    Shader(const Shader& shader) : mProgramId{ shader.mProgramId } {}
    Shader(const std::string& vertSrc, const std::string& fragSrc);
    ~Shader();

    Shader& operator=(Shader&& shader);

    template <typename T>
    void setUniform(const std::string& name, const T& value);

    template <typename T>
    void setArrayUniform(const std::string& name, const std::string& suffix, const size_t index, const T& value) {
        size_t maxSize{ name.size() + suffix.size() + 10 };
        char* buffer{ new char[maxSize] };

        if (suffix.size() > 0) {
            std::snprintf(buffer, maxSize, "%s[%ld].%s", name.c_str(), index, suffix.c_str());
        } else {
            std::snprintf(buffer, maxSize, "%s[%ld]", name.c_str(), index);
        }

        setUniform(buffer, value);
        delete buffer;
    }

    void use();
};

}

#endif