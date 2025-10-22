#include "Infeccion.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <Motor/Render/Render.hpp>
#include <Motor/GUI/GLogger.hpp>
#include <iostream>
#include "Motor/Utils/Vector2D.hpp"
namespace IVJ {


    Infeccion::Infeccion() : normalT(font_juego), infectadoT(font_juego), inmuneT(font_juego),
                             recuperacionT(font_juego), muertoT(font_juego), tiempo(font_juego), fps(font_juego),
                             mensajeFinal(font_juego), fpsText(font_juego), elapsedText(font_juego), statsTxt(font_juego),
                             sim(15, 15)
    {
    }


    void Infeccion::OnInit() {
        sim.init();
        fpsText.setFont(font_juego);
        fpsText.setCharacterSize(20);
        fpsText.setPosition({10.f, 10.f});
        fpsText.setFillColor(sf::Color::Black);
        fpsText.setStyle(sf::Text::Bold | sf::Text::Underlined);

        elapsedText.setFont(font_juego);
        elapsedText.setCharacterSize(20);
        elapsedText.setPosition({200.f, 10.f});
        elapsedText.setFillColor(sf::Color::Black);
        elapsedText.setStyle(sf::Text::Bold | sf::Text::Underlined);


        mensajeFinal.setFont(font_juego);
        mensajeFinal.setString("GAME OVER!!");
        mensajeFinal.setCharacterSize(100);
        mensajeFinal.setPosition({860, 540});
        mensajeFinal.setFillColor(sf::Color::Red);
        mensajeFinal.setStyle(sf::Text::Bold | sf::Text::Underlined);

    }


    void Infeccion::OnInputs(float dt, std::optional<sf::Event>& eventos) {
        if (!eventos) return;
        if (eventos->getIf<sf::Event::Closed>()) {
            termino = true;
            return;
        }
        if (eventos->getIf<sf::Event::MouseButtonPressed>()) {

            auto mousePos = sf::Mouse::getPosition(CE::Render::Get().GetVentana());
            sim.tryToggleAt(mousePos.x, mousePos.y, gridX, gridY, cellSize);
        }
    }


    void Infeccion::OnUpdate(float dt)
    {
        fpsText.setString(std::to_string((1.f / dt)) + " FPS");

        if (!juegoTerminado) {
            elapsed += dt;           // solo sumamos tiempo si no terminó
            sim.update(dt);
            elapsedText.setString("Elapsed: " + std::to_string(elapsed) + " s");//Texto en pantalla


            if (!sim.hasInfectious()) {
                juegoTerminado = true;
                std::cout << "No hay más células infectadas. Juego terminado.\n"; //para debugear si todo salio bien
            }
        }
    }



    void Infeccion::OnRender(float dt) {
        sim.render(gridX, gridY, cellSize);

        int normal,enfermo,recuperar,inmune,muerto;
        sim.countStates(normal,enfermo,recuperar,inmune,muerto);


        ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Normal: %d", normal);
        ImGui::Text("Enfermo: %d", enfermo);
        ImGui::Text("Recuperar: %d", recuperar);
        ImGui::Text("Inmune: %d", inmune);
        ImGui::Text("Muerto: %d", muerto);
        ImGui::End();

        std::string normaltxt = ("Normales: " + std::to_string(normal));
        std::string enfermotxt = ("Infectados: " + std::to_string(enfermo));
        std::string recuperartxt = ("Recuperados: " + std::to_string(recuperar));
        std::string inmunetxt = ("Inmunes: " + std::to_string(inmune));
        std::string muertotxt = ("Muertos: " + std::to_string(muerto));

        if (!font_juego.openFromFile(ASSETS "/fonts/BitcountGridSingle.ttf"))
        {
            std::cout << "Error al cargar font\n";
            exit(1);
        }
        normalT.setFont(font_juego);
        normalT.setString(normaltxt);
        normalT.setCharacterSize(18);
        normalT.setPosition({100, 50});
        normalT.setFillColor(sf::Color::Black);
        normalT.setStyle(sf::Text::Bold | sf::Text::Underlined);

        infectadoT.setFont(font_juego);
        infectadoT.setString(enfermotxt);
        infectadoT.setCharacterSize(18);
        infectadoT.setPosition({400, 50});
        infectadoT.setFillColor(sf::Color::Black);
        infectadoT.setStyle(sf::Text::Bold | sf::Text::Underlined);

        inmuneT.setFont(font_juego);
        inmuneT.setString(inmunetxt);
        inmuneT.setCharacterSize(18);
        inmuneT.setPosition({770,50});
        inmuneT.setFillColor(sf::Color::Black);
        inmuneT.setStyle(sf::Text::Bold | sf::Text::Underlined);

        recuperacionT.setFont(font_juego);
        recuperacionT.setString(recuperartxt);
        recuperacionT.setCharacterSize(18);
        recuperacionT.setPosition({1100, 50});
        recuperacionT.setFillColor(sf::Color::Black);
        recuperacionT.setStyle(sf::Text::Bold | sf::Text::Underlined);

        muertoT.setFont(font_juego);
        muertoT.setString(muertotxt);
        muertoT.setCharacterSize(18);
        muertoT.setPosition({1400, 50});
        muertoT.setFillColor(sf::Color::Black);
        muertoT.setStyle(sf::Text::Bold | sf::Text::Underlined);

        statsTxt.setFont(font_juego);
        statsTxt.setString("Stats>");
        statsTxt.setCharacterSize(20);
        statsTxt.setPosition({0, 50});
        statsTxt.setFillColor(sf::Color::Red);
        statsTxt.setStyle(sf::Text::Bold | sf::Text::Underlined);



        CE::Render::Get().AddToDraw(normalT);
        CE::Render::Get().AddToDraw(infectadoT);
        CE::Render::Get().AddToDraw(inmuneT);
        CE::Render::Get().AddToDraw(recuperacionT);
        CE::Render::Get().AddToDraw(muertoT);
        CE::Render::Get().AddToDraw(fpsText);
        CE::Render::Get().AddToDraw(elapsedText);
        CE::Render::Get().AddToDraw(statsTxt);
        ImGui::Begin("Time", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (!juegoTerminado) {
            ImGui::Text("elapsed: %.2f", elapsed);//usando log
            CE::Render::Get().AddToDraw(elapsedText);//usnado texto en pantalla

        } else {
            ImGui::Text("elapsed: %.2f", elapsed);
            CE::Render::Get().AddToDraw(mensajeFinal);//usando texto en oabtalla

        }

        ImGui::End();

    }
}