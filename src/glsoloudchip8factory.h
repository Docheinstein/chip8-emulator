#ifndef GLSOLOUDCHIP8FACTORY_H
#define GLSOLOUDCHIP8FACTORY_H

#include "glchip8factory.h"

class GlSoLoudChip8Factory : public GlChip8Factory {
public:
    explicit GlSoLoudChip8Factory(GLFWwindow *window);
    [[nodiscard]] std::unique_ptr<Speaker> make_speaker() const override;
};

#endif // GLSOLOUDCHIP8FACTORY_H