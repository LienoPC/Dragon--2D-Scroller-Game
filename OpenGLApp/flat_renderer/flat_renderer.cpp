#include "flat_renderer.h"

FlatRenderer::FlatRenderer(Shader shader) {
    this->shader = shader;
}

void FlatRenderer::DrawFlatRectangle(glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {

	
    // configure VAO/VBO
    unsigned int VBO, VAO, EBO;

    // definisco i vertici del rettangolo
    glm::vec2 a = position;
    glm::vec2 b = glm::vec2(a.x + size.x, a.y);
    glm::vec2 c = glm::vec2(a.x + size.x, a.y - size.y);
    glm::vec2 d = glm::vec2(a.x, a.y - size.y);

    //divido in due triangoli il rettangolo
    float vertices[] = {
        // pos      
        c.x, c.y, 
        b.x, b.y, 
        a.x, a.y,
        d.x, d.y
    };
    unsigned int indices[] = {
        0, 1, 3, // first Triangle
        1, 2, 3 // second Triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    //VBO Bind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //EBO Bind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);

    /*
     model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    this->shader.SetMatrix4("model", model);
    
    */
    
   
    this->shader.SetVector3f("flatColor", color);

    glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);

    //releasing buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
