#ifndef GLPOWERSUPPLY_H
#define GLPOWERSUPPLY_H

#include "powersupply.h"

class GLFWwindow;

class GlPowerSupply : public PowerSupply {
public:
    explicit GlPowerSupply(GLFWwindow* window);
    bool on() override;

private:
    GLFWwindow *window;
};

#endif // GLPOWERSUPPLY_H