#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>


// ˽�г�Ա����ʵ�֣�����ָ�����͵���ɫ��
unsigned int Shader::compileShader(unsigned int type, const std::string& shaderSource) {
    unsigned int id = glCreateShader(type);
    const char* src = shaderSource.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Shader compilation failed\n";
        std::cout << "Shader type: " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "\n";
        std::cout << shaderSource << std::endl;
    }

    return id;
}

// ���캯��ʵ�֣����ء����벢���Ӷ����Ƭ����ɫ��
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vsFile(vertexPath), fsFile(fragmentPath);
    std::stringstream vstream, fstream;

    vstream << vsFile.rdbuf();
    fstream << fsFile.rdbuf();
    vertexCode = vstream.str();
    fragmentCode = fstream.str();

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    ID = program;

    // ɾ����ɫ������
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::Shader(const std::string& vertexPath, const std::string& GeometryPath, const std::string& fragmentPath)
{
    std::string vertexCode, geometryCode,fragmentCode;
    std::ifstream vsFile(vertexPath), gsFile(GeometryPath),fsFile(fragmentPath);
    std::stringstream vstream,gstream, fstream;

    vstream << vsFile.rdbuf();
    gstream << gsFile.rdbuf();
    fstream << fsFile.rdbuf();
    vertexCode = vstream.str();
    geometryCode = gstream.str();
    fragmentCode = fstream.str();

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryCode);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    ID = program;

    // ɾ����ɫ������
    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
}

// ��������ʵ��
Shader::~Shader() {
    glDeleteProgram(ID);
}

// ��Ա����ʵ��
void Shader::use() const{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 vec4) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE, glm::value_ptr(vec4));
}

void Shader::setVec3(const std::string& name, glm::vec3 vec3) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec3));
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::setMat3(const std::string& name, glm::mat3 mat3) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1,GL_TRUE, glm::value_ptr(mat3));
}

void Shader::setVec2(const std::string& name, glm::vec2 vec2) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1,glm::value_ptr(vec2));
}

void Shader::Unbind() const
{
    glUseProgram(0);
}
