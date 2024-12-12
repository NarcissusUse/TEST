#pragma once

#include <glad/glad.h>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
class Shader
{
public:
    unsigned int ID;

    // 构造函数和析构函数声明
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexPath, const std::string& GeometryPath, const std::string& fragmentPath);
    ~Shader();

    // 成员函数声明
    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 vec4) const;
    void setVec3(const std::string& name, glm::vec3 vec3) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat3(const std::string& name, glm::mat3 mat3) const;
    void setVec2(const std::string& name, glm::vec2 vec2)const;
    void Unbind()const;

private:
    // 私有成员函数
    static unsigned int compileShader(unsigned int type, const std::string& shaderSource);
};
