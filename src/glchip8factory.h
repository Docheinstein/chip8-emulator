#ifndef GLCHIP8FACTORY_H
#define GLCHIP8FACTORY_H

#include <GLFW/glfw3.h>
#include "chip8factory.h"

class GlChip8Factory : public Chip8Factory {
public:
    explicit GlChip8Factory(GLFWwindow *window);
    [[nodiscard]] std::unique_ptr<Display> make_display(int scaling) const override;
    [[nodiscard]] std::unique_ptr<Keypad> make_keypad() const override;
    [[nodiscard]] std::unique_ptr<PowerSupply> make_power() const override;
    [[nodiscard]] std::unique_ptr<EventLooper> make_event_looper() const override;
private:
    GLFWwindow *window;
};

#endif // GLCHIP8FACTORY_H