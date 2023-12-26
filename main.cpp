#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shapefil.h>
#include <vector>

#include "shader.h"

// 函数声明
void processInput(GLFWwindow* window);
// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  // 动态调整视口大小和窗口大小相同

// 参数
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 要素边界坐标
double XMAX, YMAX, XMIN, YMIN;
std::vector<int> size;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 核心模式

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 注册回调函数

    // 加载GLAD（OpenGL的函数指针）
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 创建着色器
    Shader ourShader("shader.vert", "shader.frag");

    // 加载SHP数据
    const char* pShapeFile = "Road\\RoadPolygon.shp";
    SHPHandle hShp = SHPOpen(pShapeFile, "r");
    int nShapeType, nVertices;  // 类型, 节点数
    int nEntities = 0;          // 要素个数
    double* minB = new double[4];
    double* maxB = new double[4];   // 范围
    SHPGetInfo(hShp, &nEntities, &nShapeType, minB, maxB);
    std::cout << "Xmin, Ymin: " << minB[0] << ", " << minB[1] << std::endl;
    std::cout << "Xmax, Ymax: " << maxB[0] << ", " << maxB[1] << std::endl;
    XMAX = maxB[0];
    XMIN = minB[0];
    YMAX = maxB[1];
    YMIN = minB[1];
    printf("ShapeType:%d\n", nShapeType);   // 打印要素类型
    printf("Entities:%d\n", nEntities);     // 打印要素个数

    // VBO VAO
    //unsigned int* VBOs = new unsigned int[nEntities];
    //unsigned int* VAOs = new unsigned int[nEntities];
    unsigned int* VBOs = new unsigned int[nEntities];
    unsigned int* VAOs = new unsigned int[nEntities];
    glGenVertexArrays(nEntities,VAOs); // 顶点数组对象
    glGenBuffers(nEntities, VBOs);      // 顶点缓冲对象

    // 遍历每个要素
    std::vector<double> vertices;
    for (int i = 0; i < nEntities; i++)
    {
        int iShape = i;
        SHPObject* obj = SHPReadObject(hShp, iShape);
        
        int parts = obj->nParts;        // 组成部分个数
        int verts = obj->nVertices;     // 节点个数

        // 逐个遍历每个节点
        for (size_t i = 0; i < verts; i++)
        {
            // 归一化x, y坐标 [-1, 1]
            // 使其成为标准化设备坐标（NDC）
            double x = 2 * (obj->padfX[i] - XMIN) / (XMAX - XMIN) - 1;
            double y = 2 * (obj->padfY[i] - YMIN) / (YMAX - YMIN) - 1;
            double z = 0.0;

            std::cout << "x: " << x << ", y: " << y << std::endl;
            // 存储在vector中
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        size.push_back(vertices.size() / 3);

        glBindVertexArray(VAOs[i]);             // 先绑定VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]); // 再绑定VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &vertices[0], GL_STATIC_DRAW);  // 注入数据
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);                 // 整理VAO格式
        glEnableVertexAttribArray(0);
    }
    SHPClose(hShp);

    // 解除绑定
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 绘制三角形
        ourShader.use();    // 激活当前的着色器
        
        // 逐个要素绘制
        for (int i = 0; i < size.size(); i++) 
        {
            // glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_LINE_STRIP, 0, size[i]);
        }
        
        // 检查并调用事件，交换缓冲
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