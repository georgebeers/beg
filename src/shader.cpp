#include <shader.h>

using namespace BEG;

std::string Shader::errorTypeToString(ShaderErrorType err) {
    switch (err) {
        case ShaderErrorType::FileDoesNotExistError: return std::string("FileDoesNotExistError");
        case ShaderErrorType::FileReadError: return std::string("FileReadError");
        case ShaderErrorType::VertexCompilationError: return std::string("VertexCompilationError");
        case ShaderErrorType::FragmentCompilationError: return std::string("FragmentCompilationError");
        case ShaderErrorType::ProgramLinkingError: return std::string("ProgramLinkingError");
        case ShaderErrorType::InvalidUniformType: return std::string("InvalidUniformTypeError");
        default: throw std::invalid_argument("invalid shader error");
    }
}

Shader Shader::fromPaths(const std::string& vertPath, const std::string& fragPath) {
    std::ifstream vertFile(vertPath), fragFile(fragPath);

    if (!vertFile || !fragFile) {
        throw Shader::ShaderErrorType::FileDoesNotExistError;
    }

    std::stringstream vertBuf, fragBuf;

    vertBuf << vertFile.rdbuf();
    fragBuf << fragFile.rdbuf();

    return { vertBuf.str(), fragBuf.str() }; 
}

unsigned int Shader::compileShader(GLenum type, const std::string& src) {
    unsigned int id{ glCreateShader(type) };

    const char* cSrc{ src.c_str() }; /* convert shader source to c-style string so GL can read it */

    glShaderSource(id, 1, &cSrc, NULL);

    glCompileShader(id);

    int success{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        int infoLogLength{};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

        char *buf = new char[infoLogLength];
        glGetShaderInfoLog(id, infoLogLength, NULL, buf);

        Shader::ShaderError err { type == GL_VERTEX_SHADER ? Shader::ShaderErrorType::VertexCompilationError
                                                           : Shader::ShaderErrorType::FragmentCompilationError, buf };

        delete buf;

        throw err;
    }

    return id;
}

Shader::Shader(const std::string& vertSrc, const std::string& fragSrc) : mProgramId{ glCreateProgram() } {
    unsigned int vertShader{ Shader::compileShader(GL_VERTEX_SHADER, vertSrc) };
    unsigned int fragShader{ Shader::compileShader(GL_FRAGMENT_SHADER, fragSrc) };

    glAttachShader(mProgramId, vertShader);
    glAttachShader(mProgramId, fragShader);

    glLinkProgram(mProgramId);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    int success{};
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        int infoLogLength{};
        glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char *buf = new char[infoLogLength];
        glGetProgramInfoLog(mProgramId, infoLogLength, NULL, buf);

        Shader::ShaderError err { Shader::ShaderErrorType::ProgramLinkingError, buf };

        delete buf;

        throw err;
    }
}

Shader::~Shader() {
    glDeleteProgram(mProgramId);
}

Shader& Shader::operator=(Shader &&shader) {
    this->mProgramId = shader.mProgramId;
    shader.mProgramId = 0;

    return *this;
}

template <>
void Shader::setUniform<int>(const std::string& name, const int& value) {
    glUniform1i(glGetUniformLocation(mProgramId, name.c_str()), value);
}

template <>
void Shader::setUniform<bool>(const std::string& name, const bool& value) {
    glUniform1i(glGetUniformLocation(mProgramId, name.c_str()), static_cast<int>(value));
}

template <>
void Shader::setUniform<float>(const std::string& name, const float& value) {
    glUniform1f(glGetUniformLocation(mProgramId, name.c_str()), value);
}

template <>
void Shader::setUniform<double>(const std::string& name, const double& value) {
    glUniform1d(glGetUniformLocation(mProgramId, name.c_str()), value);
}

template <>
void Shader::setUniform<Vector<2>>(const std::string& name, const Vector<2>& value) {
    glUniform2f(glGetUniformLocation(mProgramId, name.c_str()), value.x(), value.y());
}

template <>
void Shader::setUniform<Vector<3>>(const std::string& name, const Vector<3>& value) {
    glUniform3f(glGetUniformLocation(mProgramId, name.c_str()), value.x(), value.y(), value.z());
}

template <>
void Shader::setUniform<Vector<4>>(const std::string& name, const Vector<4>& value) {
    glUniform4f(glGetUniformLocation(mProgramId, name.c_str()), value.x(), value.y(), value.z(), value.w());
}

template <>
void Shader::setUniform<Matrix<2>>(const std::string& name, const Matrix<2>& value) {
    glUniformMatrix2fv(glGetUniformLocation(mProgramId, name.c_str()), 1, GL_TRUE, value.data().data());
}

template <>
void Shader::setUniform<Matrix<3>>(const std::string& name, const Matrix<3>& value) {
    glUniformMatrix3fv(glGetUniformLocation(mProgramId, name.c_str()), 1, GL_TRUE, value.data().data());
}

template <>
void Shader::setUniform<Matrix<4>>(const std::string& name, const Matrix<4>& value) {
    glUniformMatrix4fv(glGetUniformLocation(mProgramId, name.c_str()), 1, GL_TRUE, value.data().data());
}

void Shader::use() {
    glUseProgram(mProgramId);
}