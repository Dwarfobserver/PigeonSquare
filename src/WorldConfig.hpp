
#ifndef PIGEONSQUARE_WORLDCONFIG_HPP
#define PIGEONSQUARE_WORLDCONFIG_HPP

#include <string>


class Window;

struct WorldConfig {
    Window* pWindow;
    std::string breadTextureName;
    std::string breadExpiredTextureName;
};


#endif //PIGEONSQUARE_WORLDCONFIG_HPP
