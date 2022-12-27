//Assignment 02: Trandformations, viewing and projection

#include "utils.h"

int width = 640, height=640;
#define DIVISIONS 100
#define PI 3.14

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);



    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram, width , height);
    
    int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    if(vVertex_attrib == -1) {
        std::cout << "Could not bind location: vVertex\n" ;
        exit(0);
    }else{
        std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    }

    // int vNormal_attrib = glGetAttribLocation(shaderProgram, "vertex_norm");
    // if(vNormal_attrib == -1) {
    //     std::cout << "Could not bind location: vertex_norm\n" ;
    //     exit(0);
    // }else{
    //     std::cout << "aNormal found at location " << vNormal_attrib << std::endl;
    // }



    int nTriangles = 0;
     std::vector<float> x,y,z, arr;
    std::vector<float> tri;

    int num_points = 0;
    int tri_points = 0;

    int j=0, i=0;
    float xval, yval, zval;
    for(float u=(M_PI*2) / 100.0,  j=0 ; j <100; j++, u+=(M_PI*2) / 100.0) 
        for(float v=(M_PI*2) / 100.0,  i=0 ; i <100; i++, v+=(M_PI*2) / 100.0){
            xval = 0.5 * sinf(v) * sinf(u);
            yval = 0.5 * sinf(v) * cosf(u);
            zval = 0.5 * cosf(u);

            x.push_back(xval);
            y.push_back(yval);
            z.push_back(zval);
            arr.push_back(xval);
            arr.push_back(yval);
            arr.push_back(zval);
            
    }
    // std::cout <<"j value :" << j << std::endl;
    num_points = x.size();
    float *val = arr.data();

    
    for(int  i=0; i< 100-1; i++)
        for(int j=0; j< 100-1 ; j++){
            tri_points+=1;
            tri.push_back(x[(i+1)*100 + j]);
            tri.push_back(y[(i+1)*100 + j]);
            tri.push_back(z[(i+1)*100 + j]);
            tri_points+=1;
            tri.push_back(x[i*100 + j]);
            tri.push_back(y[i*100 + j]);
            tri.push_back(z[i*100 + j]);
            tri_points+=1;
            tri.push_back(x[i*100 + (j+1)]);
            tri.push_back(y[i*100 + (j+1)]);
            tri.push_back(z[i*100 + (j+1)]);

            tri_points+=1;
            tri.push_back(x[(i+1)*100 + (j+1)]);
            tri.push_back(y[(i+1)*100 + (j+1)]);
            tri.push_back(z[(i+1)*100 + (j+1)]);
            tri_points+=1;
            tri.push_back(x[i*100 + (j+1)]);
            tri.push_back(y[i*100 + (j+1)]);
            tri.push_back(z[i*100 + (j+1)]);
            tri_points+=1;
            tri.push_back(x[(i+1)*100 + j]);
            tri.push_back(y[(i+1)*100 + j]);
            tri.push_back(z[(i+1)*100 + j]);

            
        }

        GLfloat expanded_vertices[tri_points*3] ;
        for(int k=0; k< tri_points*3 ; k++){
            expanded_vertices[k] = tri[k];
        }

  	static const GLfloat vertices[] = {
     0.5f,  0.5f,  0.5f, // triangle 1 : begin
    -0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f, // triangle 1 : end
     0.5f,  0.5f,  0.5f, // triangle 2 : begin
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f, // triangle 2 : end
     0.5f,  0.5f,  0.5f,//3 begin
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,//3 end

    -0.5f, -0.5f,  0.5f,//4 begin
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,//4 end
    -0.5f, -0.5f,  0.5f,//5 begin
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,//5 end
    -0.5f, -0.5f,  0.5f,//6 begin
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,//6 end

    -0.5f,  0.5f, -0.5f,//7 begin
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,//7 end
    -0.5f,  0.5f, -0.5f,//8 begin
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,//8 end
    -0.5f,  0.5f, -0.5f,//9 begin
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,//9 end

     0.5f, -0.5f, -0.5f,//10 begin
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,//10 end
     0.5f, -0.5f, -0.5f,//11 begin
    -0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,//11 end
     0.5f, -0.5f, -0.5f,//12 begin
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,//12 end*/
};

    static const GLfloat normals[] = 
    {
        0.0f,  0.0f,  1.0f, // triangle 1 : begin
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f, // triangle 1 : end
        0.0f,  1.0f,  0.0f, // triangle 2 : begin
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f, // triangle 2 : end
        1.0f,  0.0f,  0.0f,//3 begin
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,//3 end

        0.0f,  0.0f,  1.0f, // triangle 1 : begin
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f, // triangle 1 : end
        0.0f,  -1.0f,  0.0f, // triangle 2 : begin
        0.0f,  -1.0f,  0.0f,
        0.0f,  -1.0f,  0.0f, // triangle 2 : end
        -1.0f, 0.0f,  0.0f,//6 begin
        -1.0f, 0.0f,  0.0f,
        -1.0f, 0.0f,  0.0f,//6 end

        0.0f,  1.0f, 0.0f,//7 begin
        0.0f,  1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,//7 end
        0.0f,  0.0f,  -1.0f, // triangle 1 : begin
        0.0f,  0.0f,  -1.0f,
        0.0f,  0.0f,  -1.0f, // triangle 1 : end
        -1.0f,  0.0f,  0.0f, // triangle 2 : begin
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f, // triangle 2 : end
        
        1.0f,  0.0f,  0.0f,//3 begin
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,//3 end
        0.0f,  0.0f,  -1.0f, // triangle 1 : begin
        0.0f,  0.0f,  -1.0f,
        0.0f,  0.0f,  -1.0f, // triangle 1 : end
        0.0f,  -1.0f, 0.0f,//7 begin
        0.0f,  -1.0f, 0.0f,
        0.0f,  -1.0f, 0.0f,//7 end
    };
    
    glUseProgram(shaderProgram);
    GLuint sphere_VAO;
    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //else tri_points
    // glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(static_cast<uint>(vVertex_attrib));
    glVertexAttribPointer(static_cast<uint>(vVertex_attrib), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLuint normal_VBO;
    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW); //else tri_points
    // glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

   float angle = 0.0f;
   GLfloat matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f,// third column
     0.0f, 0.0f, 0.0f, 1.0f
    };



    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);
        showOptionsDialog(shaderProgram, angle, matrix);

        if (io.KeyCtrl){
            std::cout << "ctrl pressed" << std::endl;
        }
        int key = 'A';
        if(io.KeysDown[key] && io.KeyShift){
            std::cout << "a " << std::endl;
        }

        if( ImGui::IsKeyPressed('O')) {
            std::cout << "o" << std::endl;
        }

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Information");                          
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(sphere_VAO); 

        glDrawArrays(GL_TRIANGLES, 0, 36); //tri_points);//else tri_points   

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

void createCubeObject(unsigned int &program, unsigned int &cube_VAO)
{
    glUseProgram(program);

    //Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }
    int vColor_attrib = glGetAttribLocation(program, "vColor");
    if(vColor_attrib == -1) {
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }

    //Cube data
    GLfloat cube_vertices[] = {10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10, //Front
                   10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10}; //Back
    GLushort cube_indices[] = {0, 2, 3, 0, 1, 2, //Front
                4, 7, 6, 4, 6, 5, //Back
                5, 2, 1, 5, 6, 2, //Left
                4, 3, 7, 4, 0, 3, //Right
                1, 0, 4, 1, 4, 5, //Top
                2, 7, 3, 2, 6, 7}; //Bottom
    GLfloat cube_colors[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1}; //Unique face colors

    //Generate VAO object
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    //Create VBOs for the VAO
    //Position information (data + format)
    int nVertices = 6*2*3; //(6 faces) * (2 triangles each) * (3 vertices each)
    GLfloat *expanded_vertices = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
        expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
        expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
    }
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_vertices;

    //Color - one for each face
    GLfloat *expanded_colors = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        int color_index = i / 6;
        expanded_colors[i*3] = cube_colors[color_index*3];
        expanded_colors[i*3+1] = cube_colors[color_index*3+1];
        expanded_colors[i*3+2] = cube_colors[color_index*3+2];
    }
    GLuint color_VBO;
    glGenBuffers(1, &color_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vColor_attrib);
    glVertexAttribPointer(vColor_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_colors;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}

