#include "glpowersupply.h"
#include "GLFW/glfw3.h"

GlPowerSupply::GlPowerSupply(GLFWwindow *window) :
    window(window) {

}

bool GlPowerSupply::on() {
    return !glfwWindowShouldClose(window);
}
