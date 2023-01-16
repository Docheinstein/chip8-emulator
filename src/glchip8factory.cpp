#include "glchip8factory.h"
#include "glpowersupply.h"
#include "gleventlooper.h"
#include "gldisplay.h"
#include "glkeypad.h"

std::unique_ptr<Display> GlChip8Factory::make_display(int scaling) const {
    return std::make_unique<GlDisplay>(window, scaling);
}

std::unique_ptr<Keypad> GlChip8Factory::make_keypad() const {
    return std::make_unique<GlKeypad>(window);
}

std::unique_ptr<PowerSupply> GlChip8Factory::make_power() const {
    return std::make_unique<GlPowerSupply>(window);
}

std::unique_ptr<EventLooper> GlChip8Factory::make_event_looper() const {
    return std::make_unique<GlEventLooper>();
}

GlChip8Factory::GlChip8Factory(GLFWwindow *window) :
    window(window) {

}

