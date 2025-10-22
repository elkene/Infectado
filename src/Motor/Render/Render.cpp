#include "Render.hpp"
#include <iostream>
namespace CE {
    Render& Render::Get() {
        if (!instancia)
            instancia = new Render();
        return *instancia;
    }
    void Render::Terminar() {
        delete instancia;
    }
    void Render::CrearVentana(const MotorConfig &motor_info) {
        sf::ContextSettings videosettings;
        videosettings.antiAliasingLevel=3;

        r_ventana = std::make_unique<sf::RenderWindow>(
            sf::VideoMode({motor_info.vW, motor_info.vH}, 32),
            sf::String(motor_info.titulo),
            sf::Style::Default,
            sf::State::Windowed,
            videosettings);

        r_ventana->setFramerateLimit(100);
        unsigned int tw, th;

#if    DEBUG
        tw = motor_info.vW-motor_info.vW*0.25f;
        th = motor_info.vH-motor_info.vH*0.25f;
#else
        tw = motor_info.vW;
        th = motor_info.vH;
#endif
        r_textura=std::make_unique<sf::RenderTexture>(
            sf::Vector2u{tw, th}, videosettings);
    }

    void Render::AutoResizeTextura(unsigned int w, unsigned int h) {
        auto cs = r_textura->getSize();
        unsigned cw = cs.x;
        unsigned ch = cs.y;
        if ( (cw==w && ch==h) || (w==0 || h==0)) return;
        sf::ContextSettings video;
        video.antiAliasingLevel=3;
        r_textura->clear(sf::Color::Black);
        if (!r_textura->resize(sf::Vector2u{w,h},video))
            exit(EXIT_FAILURE);
    }
    void Render::AddToDraw(const sf::Drawable &obj) {
        r_textura->draw(obj);
    }
    void Render::OnClearColor(const sf::Color& clearcolor) {
        r_textura->clear(clearcolor);
        r_ventana->clear(clearcolor);
    }

    void Render::OnDisplayTextura(void) {
        r_textura->display();
    }

    void Render::OnDisplayVentana(void) {
        r_ventana->display();
    }
    sf::RenderWindow& Render::GetVentana(void) {
        return *r_ventana;
    }
    sf::RenderTexture& Render::GetTextura(void) {
        return *r_textura;
    }

}