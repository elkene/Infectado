#include "Juego.hpp"
#include <iostream>
#include <fstream>
#include <Motor/Render/Render.hpp>
#include <SFML/Graphics.hpp>
#include "Figuras/Figuras.hpp"
#include <memory>
#include <filesystem>
#include <cmath>
#include "Motor/Utils/Vector2D.hpp"
#include "Sistemas/Sistemas.hpp"
#include "../Motor/Utils/Lista.hpp"
#include "../Motor/GUI/GLogger.hpp"


namespace IVJ {

    Juego :: Juego() : CE ::GameLayer{}, texto(font_juego), muerto(font_juego) {
        termino = false;
    }

    void Juego :: OnInit(void) {
        /*if (std::ifstream file(ASSETS "/config/figuras.txt"); !file) {
            std::cerr << "No se pudo abrir assets/config/figuras.txt\n";
        } else {
            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;
                if (auto f = Figuras::deserialize(line)) {
                    objetos.push_back(std::move(f));

                }
            }
        }*/
        /*float altoVentana = 1080;
        float anchoVentana = 1920;
        auto circ = std::make_shared<Circulo>(100,100,200,sf::Color::Blue,sf::Color::Red);
        circ->getTransformada()->centroX = circ->transform->posicion.x;
        circ->getTransformada()->centroY = circ->transform->posicion.y;
        circ->getTransformada()->inicializado = true;
        circ->getTransformada()->velocidad = {25.f, 100.f};
        circ->addComponente(std::make_shared<CE::ITimer>());;
        objetos.push_back(circ);*/

        std::string txt = "Un texto";
        CE::Vector2D P1{100.3, 200.1};
        CE::Vector2D P2{500.3, 100.1};

        CE::Lista<int> listilla;
        listilla.agregarDato(1);
        listilla.agregarDato(2);
        listilla.agregarDato(3);
        listilla.agregarDato(4);
        listilla.agregarDato(5);

        listilla.eliminarDato(2);
        listilla.eliminarDato(2);
;
        for (auto i = listilla.iterInicio(); !listilla.iterTermino(); i = listilla.iterSiguiente()) {
            std::cout<<*i<<"\n";
        }
        while (listilla.getSize() > 0)
            std::cout<<listilla.popDato()<<"\n";

        std::string normalT = ("Normal: ");
        std::string enfermoT = ("Enfermo: ");
        std::string recuperarT = ("Recuperar: ");
        std::string inmuneT = ("Inmune: ");
        std::string muertoT = ("Muerto: ");

        if (!font_juego.openFromFile(ASSETS "/fonts/Montserrat-Bold.ttf"))
        {
            std::cout << "Error al cargar font\n";
            exit(1);
        }
        texto.setFont(font_juego);
        texto.setString(normalT);
        texto.setCharacterSize(50);
        texto.setPosition({500, 300});
        texto.setFillColor(sf::Color::Black);
        texto.setStyle(sf::Text::Bold | sf::Text::Underlined);

        muerto.setFont(font_juego);
        muerto.setString(muertoT);
        muerto.setCharacterSize(50);
        muerto.setPosition({700, 300});
        muerto.setFillColor(sf::Color::Black);
        muerto.setStyle(sf::Text::Bold | sf::Text::Underlined);
    }
    void Juego :: OnInputs(float dt,
        std::optional<sf::Event>& eventos) {

        if (eventos->getIf<sf::Event::Closed>())
            termino = true;
    }
    void Juego::OnUpdate(float dt)
    {
        float ancho = 1920;
        float alto  = 1080;

        SistemaRectangulo(objetos, dt, alto);
        SistemaTriangulo(objetos, dt, ancho, alto);
        SistemaCirculo(objetos, dt, ancho, alto);

        auto nivelPos = [ancho, alto](float x, float y) {
            if (x<=100.f)
                return CE::GLogger::Niveles::LOG_DEBUG;
            if (x > ancho - 100.0f)
                return CE::GLogger::Niveles::LOG_ERROR;
            if (std::abs(x - alto) < 50.0f)
                return CE::GLogger::Niveles::LOG_SEVERO;
            return CE::GLogger::Niveles::LOG;
        };

        for (auto &f: objetos) {

            auto transform = f->getTransformada();
            auto nombre = f->getNombre() ? f->getTipo() : "Figura";
            float x = transform->posicion.x;
            float y = transform->posicion.y;
            float vx = transform->velocidad.x;
            float vy = transform->velocidad.y;

            auto nivel = nivelPos(x, y);

            std::string info =
                "Nombre: " + nombre +
                " | Posición: (" + std::to_string(x) + ", " + std::to_string(y) + ")" +
                " | Velocidad: (" + std::to_string(vx) + ", " + std::to_string(vy) + ")";

           // CE::GLogger::Get().agregarLog(info, CE::GLogger::Niveles::LOG);

            CE::GLogger::Get().agregarLog(info, nivel);
            f->onUpdate(dt);

        }

    }

    void Juego :: OnRender(float dt) {
        for (auto& f : objetos) {
            if (f) {
                CE::Render::Get().AddToDraw(*f);

            }
        }
        CE::Render::Get().AddToDraw(texto);
        CE::Render::Get().AddToDraw(muerto);
    }
}
