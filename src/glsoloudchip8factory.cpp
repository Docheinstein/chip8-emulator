#include "glsoloudchip8factory.h"
#include "soloudspeaker.h"

GlSoLoudChip8Factory::GlSoLoudChip8Factory(GLFWwindow *window) :
    GlChip8Factory(window) {

}

std::unique_ptr <Speaker> GlSoLoudChip8Factory::make_speaker() const {
    return std::make_unique<SoLoudSpeaker>();
}


