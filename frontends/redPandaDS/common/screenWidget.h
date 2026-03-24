#ifndef RED_PANDA_DS_UI_SCREEN_WIDGET_H
#define RED_PANDA_DS_UI_SCREEN_WIDGET_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <vector>

namespace RedPandaDS {
namespace UI {

class ScreenWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    ScreenWidget(QWidget* parent = nullptr);
    /**
     * @brief Set a new pixel frame buffer to render. Triggers a redraw.
     *
     * @param newPixels Pixel data to render.
     */
    void setPixels(const std::vector<uint32_t>& newPixels);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    const int widthPixels, heightPixels;
    std::vector<uint32_t> pixels;
    GLuint textureId;

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    void initQuad();

    QOpenGLShaderProgram shader;
    void initShaders();
};

}  // namespace UI
}  // namespace RedPandaDS

#endif
