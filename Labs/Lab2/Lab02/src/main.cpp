// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

#include "utils.h"


 // float vertices[] = {
 //    // positions         
 //     0.5f, -0.5f, 0.0f, 
 //    -0.5f, -0.5f, 0.0f, 
 //     0.0f,  0.5f, 0.0f, 
 //    };  

    static const GLfloat vertices[] = {
    -0.25f,-0.25f,-0.25f, // triangle 1 : begin
    -0.25f,-0.25f, 0.25f,
    -0.25f, 0.25f, 0.25f, // triangle 1 : end
    0.25f, 0.25f,-0.25f, // triangle 2 : begin
    -0.25f,-0.25f,-0.25f,
    -0.25f, 0.25f,-0.25f, // triangle 2 : end
    0.25f,-0.25f, 0.25f,//3 begin
    -0.25f,-0.25f,-0.25f,
    0.25f,-0.25f,-0.25f,//3 end
    0.25f, 0.25f,-0.25f,//4 begin
    0.25f,-0.25f,-0.25f,
    -0.25f,-0.25f,-0.25f,//4 end
    -0.25f,-0.25f,-0.25f,//5 begin
    -0.25f, 0.25f, 0.25f,
    -0.25f, 0.25f,-0.25f,//5 end
    0.25f,-0.25f, 0.25f,//6 begin
    -0.25f,-0.25f, 0.25f,
    -0.25f,-0.25f,-0.25f,//6 end
    -0.25f, 0.25f, 0.25f,//7 begin
    -0.25f,-0.25f, 0.25f,
    0.25f,-0.25f, 0.25f,//7 end
    0.25f, 0.25f, 0.25f,//8 begin
    0.25f,-0.25f,-0.25f,
    0.25f, 0.25f,-0.25f,//8 end
    0.25f,-0.25f,-0.25f,//9 begin
    0.25f, 0.25f, 0.25f,
    0.25f,-0.25f, 0.25f,//9 end
    0.25f, 0.25f, 0.25f,//10 begin
    0.25f, 0.25f,-0.25f,
    -0.25f, 0.25f,-0.25f,//10 end
    0.25f, 0.25f, 0.25f,//11 begin
    -0.25f, 0.25f,-0.25f,
    -0.25f, 0.25f, 0.25f,//11 end
    0.25f, 0.25f, 0.25f,//12 begin
    -0.25f, 0.25f, 0.25f,
    0.25f,-0.25f, 0.25f//12 end
};


static const GLfloat colors[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};
    static const GLfloat colors2[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

int width = 640, height=640;
int main(int, char**)
{
    // Setup window

    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x  << " " << vec.y  << " " << vec.z << std::endl;


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);


    GLfloat matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f,// third column
     0.0f, 0.0f, 0.0f, 1.0f
    };

    float transX = 0.0f;
    float transZ = 0.0f;
    float scale = 1.0f;
    float angle = 0.0f;

    int mat_loc = glGetUniformLocation(shaderProgram, "m_matrix");
    glm::mat4 rotate_mat = glm::rotate(glm::make_mat4(matrix), angle, glm::vec3(1,0,0));
    glUniformMatrix4fv(mat_loc, 1, GL_FALSE, (&rotate_mat[0][0]));

    get_mat(matrix);

    GLuint points_vbo;
    glGenBuffers( 1, &points_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    GLuint colours_vbo;
    glGenBuffers( 1, &colours_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );

    GLuint VAO;
    glGenVertexArrays( 1, &VAO );
    glBindVertexArray( VAO );
    glBindBuffer( GL_ARRAY_BUFFER, points_vbo );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glBindBuffer( GL_ARRAY_BUFFER, colours_vbo );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 0 );
    glEnableVertexAttribArray( 1 );
    

    
    glEnable( GL_CULL_FACE ); // cull face
    glCullFace( GL_BACK );      // cull back face
    glFrontFace( GL_CW );           // GL_CCW for counter clock-wise
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);

        showOptionsDialog(shaderProgram, transX, transZ, angle, scale, matrix);

        {
            static float f = 0.0f;
            static int counter = 0;

            // ImGui::Begin("Hello, world!");                          

            // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            // ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 12*3);


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

