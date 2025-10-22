#pragma once
#include <Motor/App/ALayer.hpp>
#include "Figuras/Figuras.hpp"
#include <memory>

namespace IVJ {

    class Juego: public CE::GameLayer {
        public:
        explicit Juego(void);
        void OnInit(void);
        void OnInputs(float dt, std::optional<sf::Event>& eventos);
        void OnUpdate(float dt);
        void OnRender(float dt);


    private:
        std::vector<std::shared_ptr<Figuras>> figuras;
        std::vector<std::shared_ptr<CE::Objeto>> objetos;
        sf::Font font_juego;
        sf::Text texto;
        sf::Text muerto;
    };
}
