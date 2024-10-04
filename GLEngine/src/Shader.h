﻿#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    virtual ~Shader();
    std::string vertexString;
    std::string fragmentString;
    const char *vertexSource;
    const char *fragmentSource;
    unsigned int ID; // shader program ID
    void use();

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec2(const std::string &name, float x, float y) const;

    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;

    void SetVec4(const std::string &name, const glm::vec4 &value) const;
    void SetVec4(const std::string &name, float x, float y, float z, float w) const;

    void SetMat2(const std::string &name, const glm::mat2 &mat) const;

    void SetMat3(const std::string &name, const glm::mat3 &mat) const;

    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

    void SetUniform3f(const std::string &name, float x, float y, float z) const;

    void SetUniform3f(const std::string &name, const glm::vec3 &param) const;

    void SetUniform1f(const std::string &name, float value) const;

    void SetUniform1i(const std::string &name, int value) const;

    enum Slot
    {
        DIFFUSE,
        SPECULAR
    };

private:
    void checkCompileErrors(unsigned int ID, std::string type);
};
