#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "Camera.h"
#include "Model.h"
#include<map>
#include<sstream>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string>faces);

// settings
const unsigned int SCR_WIDTH = 3200;
const unsigned int SCR_HEIGHT = 2000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    /*glm::vec3 pointLightPositions[] = {
        glm::vec3(3.0f,  3.0f,  3.0f),
        glm::vec3(3.0f, -3.0f,  3.0f),
        glm::vec3(-3.0f,  3.0f,  3.0f),
        glm::vec3(-3.0f, -3.0f,  3.0f)
    };*/
    // ------------------------------------------------------------------
    //Shader ourShader("5.1.framebuffersV.shader", "5.1.framebuffersF.shader");
    //Shader screenShader("FrameBuffer_vr.shader", "FrameBuffer_frag.shader");
    //Shader skyboxShader("cubemapV.shader", "cubemapF.shader");
    //Shader redShader("8advanced_glsl.shader", "redFrag.shader");
    //Shader blueShader("8advanced_glsl.shader", "blueFrag.shader");
    //Shader yellowShader("8advanced_glsl.shader", "yellowFrag.shader");
    //Shader greenShader("8advanced_glsl.shader", "greenFrag.shader");
    //Shader gsShader("gsV.shader", "geometry.shader", "gsF.shader");
    Shader shader("9.4.instanceV.shader", "9.4.instanceF.shader");
    vector<std::string> faces{
        "asset/skybox/right.jpg",
        "asset/skybox/left.jpg",
        "asset/skybox/top.jpg",
        "asset/skybox/bottom.jpg",
        "asset/skybox/front.jpg",
        "asset/skybox/back.jpg"
    };
    Model nanosuit("asset\\nanosuit\\nanosuit.obj");
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    float cubeVertices[] = {
        // Back face
        // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float points[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
    };
    float quadVertices[] = {
        // 位置          // 颜色
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y+=2)
    {
        for (int x = -10; x < 10;  x += 2) {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }
    VertexArray VAOpoints;
    VAOpoints.Bind();
    VertexBuffer VBOpoints(points, sizeof(points));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    VertexArray VAOsky;
    VAOsky.Bind();
    VertexBuffer VBOsky(skyboxVertices, sizeof(skyboxVertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    VertexArray VAOcube;
    VAOcube.Bind();
    VertexBuffer VBOCube(cubeVertices,sizeof(cubeVertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    VertexArray VAOcubePosition;
    VAOcubePosition.Bind();
    VertexBuffer VBOCubePosition(cubeVertices, sizeof(cubeVertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    VertexArray VAOplane;
    VAOplane.Bind();
    VertexBuffer VBOplane(planeVertices, sizeof(planeVertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    VertexArray VAOQuad;
    VAOQuad.Bind();
    VertexBuffer VBOQuad(quadVertices, sizeof(quadVertices));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    VertexBuffer VBOinstance(&translations[0],sizeof(glm::vec2)*100);
    VBOinstance.Bind();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    VBOinstance.Unbind();
    glVertexAttribDivisor(2, 1);
    glBindVertexArray(0);
    unsigned int cubeTexture = loadTexture("container.jpg");
    unsigned int floorTexture = loadTexture("metal.png");
    unsigned int cubemapTexture = loadCubemap(faces);

    // build and compile shaders
    //// -------------------------
    //ourShader.use();
    //ourShader.setInt("skybox", 0);
    //screenShader.use();
    //screenShader.setInt("screenTexture", 0);
    //skyboxShader.use();
    //skyboxShader.setInt("skybox", 0);

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //setting uniform block
    ////bind all unifrom block in shader to binding index
    //unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(greenShader.ID, "Matrices");
    //unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(redShader.ID, "Matrices");
    //unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(yellowShader.ID, "Matrices");
    //unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(blueShader.ID, "Matrices");
    //unsigned int uniformBlockIndexOurShader = glGetUniformBlockIndex(ourShader.ID, "Matrices");

    //glUniformBlockBinding(greenShader.ID, uniformBlockIndexGreen, 0);
    //glUniformBlockBinding(redShader.ID, uniformBlockIndexRed, 0);
    //glUniformBlockBinding(yellowShader.ID, uniformBlockIndexYellow, 0);
    //glUniformBlockBinding(blueShader.ID, uniformBlockIndexBlue, 0);
    //glUniformBlockBinding(ourShader.ID, uniformBlockIndexOurShader, 0);
    //create a buffer and bind this buffer to binding index
    //unsigned int uboMatrices;
    //glGenBuffers(1, &uboMatrices);
    //glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    //glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    //glBindBuffer(GL_UNIFORM_BUFFER,0);
    ////bind this databuffer to binding index
    //glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);


    
    // load models
    // -----------


    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        // 
        // 
        //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        //glEnable(GL_DEPTH_TEST);    
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ////write in the databuffer which alreadly binding binding index
        /*glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);*/
        //glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        //glBindBuffer(GL_UNIFORM_BUFFER, 0);

        //glm::mat4 view = camera.GetViewMatrix();
        //glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        //glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        //glBindBuffer(GL_UNIFORM_BUFFER, 0);

        //ourShader.use();
        //glm::mat4 model = glm::mat4(1.0f);
        //VAOcube.Bind();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP,cubemapTexture);
        //model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        //ourShader.setMat4("model", model);
        //ourShader.setMat4("view", view);
        //ourShader.setMat4("projection", projection);
        //ourShader.setVec3("cameraPos", camera.Position);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(2.0f, 0.0f, -1.0f));
        //ourShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        ////write four cubes
        //VAOcubePosition.Bind();
        //redShader.use();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));  // 移动到左上角
        //redShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //greenShader.use();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));  // 移动到右上角
        //greenShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //blueShader.use();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));  // 移动到左下角
        //blueShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //yellowShader.use();
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));  // 移动到右下角
        //yellowShader.setMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //
        ////draw skybox
        //glDepthFunc(GL_LEQUAL);
        //skyboxShader.use();
        //view = glm::mat4(glm::mat3(view));
        //skyboxShader.setMat4("view", view);
        //skyboxShader.setMat4("projection", projection);
        //VAOsky.Bind();
        //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDepthFunc(GL_LESS);

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glDisable(GL_DEPTH_TEST);
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        //glClear(GL_COLOR_BUFFER_BIT);

       


        //screenShader.use();
        //VAOQuad.Bind();
        //glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        ////set texture sampler in fragment
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        shader.use();
        VAOQuad.Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        




        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

unsigned int loadTexture(const char* path)
{
    unsigned int texture1;
    glGenTextures(1, &texture1);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents==1)
        {
            format = GL_RED;
        }
        if (nrComponents==3)
        {
            format = GL_RGB;
        }
        if (nrComponents==4)
        {
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        stbi_image_free(data);
    }
    else
    {
        std::cout << "texture failed" << endl;
        stbi_image_free(data);
    }
    return texture1;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}