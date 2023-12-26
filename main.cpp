#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shapefil.h>
#include <vector>

#include "shader.h"

// ��������
void processInput(GLFWwindow* window);
// �ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  // ��̬�����ӿڴ�С�ʹ��ڴ�С��ͬ

// ����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Ҫ�ر߽�����
double XMAX, YMAX, XMIN, YMIN;
std::vector<int> size;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // �汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ����ģʽ

    // ��������
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // ע��ص�����

    // ����GLAD��OpenGL�ĺ���ָ�룩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ������ɫ��
    Shader ourShader("shader.vert", "shader.frag");

    // ����SHP����
    const char* pShapeFile = "Road\\RoadPolygon.shp";
    SHPHandle hShp = SHPOpen(pShapeFile, "r");
    int nShapeType, nVertices;  // ����, �ڵ���
    int nEntities = 0;          // Ҫ�ظ���
    double* minB = new double[4];
    double* maxB = new double[4];   // ��Χ
    SHPGetInfo(hShp, &nEntities, &nShapeType, minB, maxB);
    std::cout << "Xmin, Ymin: " << minB[0] << ", " << minB[1] << std::endl;
    std::cout << "Xmax, Ymax: " << maxB[0] << ", " << maxB[1] << std::endl;
    XMAX = maxB[0];
    XMIN = minB[0];
    YMAX = maxB[1];
    YMIN = minB[1];
    printf("ShapeType:%d\n", nShapeType);   // ��ӡҪ������
    printf("Entities:%d\n", nEntities);     // ��ӡҪ�ظ���

    // VBO VAO
    //unsigned int* VBOs = new unsigned int[nEntities];
    //unsigned int* VAOs = new unsigned int[nEntities];
    unsigned int* VBOs = new unsigned int[nEntities];
    unsigned int* VAOs = new unsigned int[nEntities];
    glGenVertexArrays(nEntities,VAOs); // �����������
    glGenBuffers(nEntities, VBOs);      // ���㻺�����

    // ����ÿ��Ҫ��
    std::vector<double> vertices;
    for (int i = 0; i < nEntities; i++)
    {
        int iShape = i;
        SHPObject* obj = SHPReadObject(hShp, iShape);
        
        int parts = obj->nParts;        // ��ɲ��ָ���
        int verts = obj->nVertices;     // �ڵ����

        // �������ÿ���ڵ�
        for (size_t i = 0; i < verts; i++)
        {
            // ��һ��x, y���� [-1, 1]
            // ʹ���Ϊ��׼���豸���꣨NDC��
            double x = 2 * (obj->padfX[i] - XMIN) / (XMAX - XMIN) - 1;
            double y = 2 * (obj->padfY[i] - YMIN) / (YMAX - YMIN) - 1;
            double z = 0.0;

            std::cout << "x: " << x << ", y: " << y << std::endl;
            // �洢��vector��
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        size.push_back(vertices.size() / 3);

        glBindVertexArray(VAOs[i]);             // �Ȱ�VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]); // �ٰ�VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &vertices[0], GL_STATIC_DRAW);  // ע������
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);                 // ����VAO��ʽ
        glEnableVertexAttribArray(0);
    }
    SHPClose(hShp);

    // �����
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ����
        processInput(window);

        // ��Ⱦ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ����������
        ourShader.use();    // ���ǰ����ɫ��
        
        // ���Ҫ�ػ���
        for (int i = 0; i < size.size(); i++) 
        {
            // glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_LINE_STRIP, 0, size[i]);
        }
        
        // ��鲢�����¼�����������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(size.size(), VAOs);
    glDeleteBuffers(size.size(), VBOs);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}