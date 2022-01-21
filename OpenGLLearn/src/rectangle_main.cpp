#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


const char* vertexShaderRecSource =
"#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main() \n"
"{\n"
"	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";

const char* fragmentShaderRecSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";


int main() 
{



	//��ʼ��
	glfwInit();
	//���ú�OPENGL�汾��ģʽ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello!OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glad ���أ��������OpenGL�ĳ�ʼ��������OpenGL�ĺ���ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/**
	������ɫ��������Ҫ��̬���봫��GPU
	*/

	//���嶥����ɫ������Ȼ���ַ���GLSL���봫�룬Ȼ����б���
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderRecSource, NULL);
	glCompileShader(vertexShader);
	//����Ƿ����ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "vertexShaderSourceCompile Error��" << infoLog << std::endl;
	}

	/*
	Ƭ����ɫ������̬����
	*/

	//
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderRecSource, NULL);
	glCompileShader(fragmentShader);

	//����Ƿ����ɹ�

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "fragmentShaderSourceCompile Error��" << infoLog << std::endl;
	}

	/*
	���ӣ���ɫ��
	*/
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "��ɫ����������ʧ�ܣ�" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//���嶥������
	float vertices[] = {
		0.5f,0.5f,0.0f, //���Ͻ�
		0.5f,-0.5f,0.0f, //���½�
		-0.5f,-0.5f,0.0f, //���½�
		-0.5f,0.5f,0.0f  //���Ͻ�
	};

	unsigned int indices[] = {
		0,1,3,//��һ��������
		1,2,3 //�ڶ���������
	};



	unsigned int VBO, VAO,EBO;

	glGenBuffers(1,&EBO);
	//����һ��VAO��������
	glGenVertexArrays(1, &VAO);
	//����һ�����㻺�����
	glGenBuffers(1, &VBO);
	//��ʼ����
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//ָ�����߰󶨡���������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��CPU�����ݷ��͵�GPU�����һ���������Ǹ���GPU���ݲ���ı䡣
	//������ݻ�Ƶ���ı䣬��ʹ��GL_DYNAMIC_DRAW��GL_STREAM_DRAW,GPU�Ὣ���ݷ��ڸ���д����ڴ沿��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���ö���ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//�ȴ��رմ���
	while (!glfwWindowShouldClose(window))
	{

		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//ʹ���߿�ģʽ����
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

