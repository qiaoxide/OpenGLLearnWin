#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//使用/激活程序
	void use();
	//uniform工具函数
	void setBool(const std::string &name,bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setColor(const std::string &name, float r, float g, float b);
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif

