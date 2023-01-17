#ifdef OPENGL_ENABLED

#ifndef GLWINDOW_H
#define GLWINDOW_H

class GLFWwindow;

class GlWindow {
public:
    GlWindow(int width, int height, const char *name);
    ~GlWindow();

    GlWindow(const GlWindow &other) = delete;
    GlWindow & operator = (const GlWindow &other) = delete;

    GlWindow(GlWindow &&other) noexcept;
    GlWindow & operator = (GlWindow &&other) noexcept;

    [[nodiscard]] GLFWwindow *window() const;

private:
    GLFWwindow *glfw_window;
};

#endif // GLWINDOW_H

#endif // OPENGL_ENABLED