#pragma once
#include <Motor/App/ALayer.hpp>
#include "Simulation/Simulation.hpp"
#include <optional>


namespace IVJ {
    class Infeccion : public CE::GameLayer {
    public:
        Infeccion();
        ~Infeccion() override = default;


        void OnInit() override;
        void OnInputs(float dt, std::optional<sf::Event>& eventos) override;
        void OnUpdate(float dt) override;
        void OnRender(float dt) override;
    private:
        Simulation sim;
        float elapsed = 0.f;
        // layout
        float gridX = 400.f;
        float gridY = 120.f;
        float cellSize = 28.f; // tuned for 15x15
        sf::Font font_juego;
        sf::Text normalT, infectadoT, inmuneT, recuperacionT, muertoT, tiempo, fps, mensajeFinal;
        bool juegoTerminado = false;
        sf::Text fpsText;
        sf::Text elapsedText;
        sf::Text statsTxt;
    };
}