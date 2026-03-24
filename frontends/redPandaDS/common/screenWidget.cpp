#include "screenWidget.h"

#include "core/core.h"

namespace RedPandaDS {
namespace UI {
// ==================================================================================================
ScreenWidget::ScreenWidget(QWidget* parent)
    : widthPixels(Core::DS_LCD_WIDTH), heightPixels(Core::DS_LCD_HEIGHT) {
    pixels.resize(widthPixels * heightPixels);
}
// ==================================================================================================
void ScreenWidget::setPixels(const std::vector<uint32_t>& newPixels) {
    assert(newPixels.size() == widthPixels * heightPixels);
    // Copy the pixel buffer and redraw.
    pixels = newPixels;
    update();
}
// ==================================================================================================
void ScreenWidget::initializeGL() {
    initializeOpenGLFunctions();

    // Enable vsync.
    context()->format().setSwapInterval(1);

    // Create OpenGL objects.
    initShaders();
    initQuad();

    // Generate a texture for pixel buffer
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Nearest filter.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Create a 2D image texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPixels, heightPixels, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 pixels.data());
}
// ==================================================================================================
void ScreenWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}
// ==================================================================================================
void ScreenWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    shader.bind();
    glBindVertexArray(VAO);

    // Update texture with the current pixel buffer.
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, widthPixels, heightPixels, GL_RGBA, GL_UNSIGNED_BYTE,
                    pixels.data());
    // Draw the quad to the screen.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
// ==================================================================================================
void ScreenWidget::initQuad() {
    // Create a single texture quad (two triangles) which will always be rendered.
    float vertices[] = {// PosX PosY  TexX  TexY
                        -1.0f, -1.0f, 0.0f, 0.0f,

                        1.0f,  -1.0f, 1.0f, 0.0f,

                        1.0f,  1.0f,  1.0f, 1.0f,

                        -1.0f, 1.0f,  0.0f, 1.0f};
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    // Create vertex array / buffer and index buffer. Geometry is unchanging.

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex layout.
    // Element 1 -> 2 floats for position.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // Element 2 -> 2 floats for texture id.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
// ==================================================================================================
void ScreenWidget::initShaders() {
    // Create a simple vertex and fragment shader for rendering textured vertices.
    shader.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                   R"(#version 330 core
        layout(location = 0) in vec2 pos;
        layout(location = 1) in vec2 tex;
        out vec2 TexCoords;
        void main() {
            TexCoords = tex;
            gl_Position = vec4(pos, 0.0, 1.0);
        })");

    shader.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                   R"(#version 330 core
        in vec2 TexCoords;
        out vec4 FragColor;
        uniform sampler2D screenTexture;
        void main() {
            FragColor = texture(screenTexture, TexCoords);
        })");
    shader.link();
    shader.bind();
    shader.setUniformValue("screenTexture",
                           0);  // Force the shader to only render one type of texture.
}
// ==================================================================================================
}  // namespace UI
}  // namespace RedPandaDS
