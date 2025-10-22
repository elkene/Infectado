#include "Figuras.hpp"
#include <sstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>


namespace IVJ {
    Figuras::Figuras(int l, float x, float y, const sf::Color &relleno, const sf::Color &contorno)
        : CE::Objeto{}, lados{l}, c_relleno{relleno}, c_contorno{contorno}{

    }

    std::shared_ptr<Figuras> Figuras::deserialize(const std::string& line) {
        std::istringstream iss(line);
        std::string tipo;
        if (!(iss >> tipo)) return nullptr;

        if (tipo == "Circulo") {
            float r; int R, G, B, cR,cG,cB; float x,y;
            if (!(iss >> r >> x >> y >> R >> G >> B >> cR >> cG >> cB)) return nullptr;
            auto cir = std::make_shared<Circulo>(r, x, y, sf::Color(R,G,B), sf::Color(cR,cG,cB));
            cir->transform->posicion.x = x;
            cir->transform->posicion.y = y;
            cir->getTransformada()->velocidad = {25.f, 100.f};
            cir->addComponente(std::make_shared<CE::ITimer>());;
            return cir;
        }
        else if (tipo == "Rectangulo") {
            float an,al; int R, G, B, cR,cG,cB; float x,y;
            if (!(iss >> an >> al >> x >> y >> R >> G >> B >> cR >> cG >> cB)) return nullptr;
            auto rect = std::make_shared<Rectangulo>(an, al, x, y, sf::Color(R,G,B), sf::Color(cR,cG,cB));
            rect->transform->posicion.x = x;
            rect->transform->posicion.y = y;
            rect->getTransformada()->velocidad = {0.f, 50.f}; // mover arriba/abajo
            rect->addComponente(std::make_shared<CE::ITimer>());
            return rect;
        }
        else if (tipo == "Triangulo") {
            float l;

            int R, G, B, cR,cG,cB;
            float x,y;
            if (!(iss >> l >> x >> y >> R >> G >> B >> cR >> cG >> cB))
                return nullptr;
            auto tri = std::make_shared<Triangulo>(l, x, y, sf::Color(R,G,B), sf::Color(cR,cG,cB));
            tri->transform->posicion.x = x;
            tri->transform->posicion.y = y;
            tri->getTransformada()->velocidad = {50.f, 0.f}; // velocidad horizontal
            tri->addComponente(std::make_shared<CE::ITimer>());;

            return tri;
        }

        // tipo desconocido
        return nullptr;
    }


    Rectangulo::Rectangulo(float ancho, float largo, float x, float y, const sf::Color &relleno, const sf::Color &contorno)
        :Figuras{4, x, y, relleno, contorno},
        imagen{sf::RectangleShape{{ancho, largo}}},
        w{ancho},
        h{largo}{

        imagen.setFillColor(relleno);
        imagen.setOutlineColor(contorno);
        imagen.setOutlineThickness(2.5f);
        imagen.setOrigin({ancho/2.f, largo/2.f});



    }
    float Rectangulo::getArea() const {
        return w * h;
    }

    void Rectangulo::setPosicion(float x, float y) {
        imagen.setPosition({x, y});
    }
    void Rectangulo::draw(sf::RenderTarget &target, sf::RenderStates state) const {
        state.transform *= getTransform();
        target.draw(imagen);
        sf::CircleShape ancla{10.f};
        ancla.setFillColor(sf::Color::Black);
        ancla.setOrigin({10.f, 10.f});
        ancla.setPosition(imagen.getPosition());
        target.draw(ancla);

    }

    std::string Rectangulo::getTipo() const { return "Rectangulo"; }

    void Rectangulo::onUpdate(float dt) {
        imagen.setPosition(
            {
                transform->posicion.x,
                transform->posicion.y
            });
    }
    Circulo::Circulo(float radio, float x, float y, const sf::Color &relleno, const sf::Color &contorno)
    :Figuras{1, x, y, relleno,contorno},
    imagen{sf::CircleShape{{radio}}},
    radio{radio}
    {

        imagen.setFillColor(relleno);
        imagen.setOutlineColor(contorno);
        imagen.setOutlineThickness(2.5f);
        imagen.setOrigin({radio, radio});


    }

    void Circulo::setPosicion(float x, float y) {
        imagen.setPosition({x, y});

    }

    void Circulo::draw(sf::RenderTarget &target, sf::RenderStates state) const {
        state.transform *= getTransform();
        target.draw(imagen);
        sf::CircleShape ancla{10.f};
        ancla.setFillColor(sf::Color::Black);
        ancla.setOrigin({10.f, 10.f});
        ancla.setPosition(imagen.getPosition());
        target.draw(ancla);
    }

    float Circulo::getArea() const {
        return 3.14159f * radio * radio;
    }

    std::string Circulo::getTipo() const { return "Circulo"; }

    void Circulo::onUpdate(float dt) {
        imagen.setPosition(
            {
                transform->posicion.x,
                transform->posicion.y
            });
    }

    Triangulo::Triangulo(float l, float x, float y, const sf::Color& relleno, const sf::Color& contorno)
    : Figuras{3, x, y, relleno, contorno}, l{l} {

        imagen.setPointCount(3);
        imagen.setPoint(0, sf::Vector2f(l,0));
        imagen.setPoint(1, sf::Vector2f(0,l));
        imagen.setPoint(2, sf::Vector2f(l,l));


        imagen.setOrigin(sf::Vector2f(l/1.5, l/1.5));
        imagen.setFillColor(relleno);
        imagen.setOutlineColor(contorno);
        imagen.setOutlineThickness(2.5F);
    }

    void Triangulo::setPosicion(float x, float y) {
        imagen.setPosition({x, y});
    }


    void Triangulo::draw(sf::RenderTarget &target, sf::RenderStates state) const {
        state.transform *= getTransform();
        target.draw(imagen);
        sf::CircleShape ancla{10.f};
        ancla.setFillColor(sf::Color::Black);
        ancla.setOrigin({10.f,10.f});
        ancla.setPosition({imagen.getPosition()});
        target.draw(ancla);

    }

    float Triangulo::getArea() const {
        // Fórmula de Herón
        float s=3*l/2;
        return  sqrt (s*(s-l)*(s-l)*(s-l));
    }

    std::string Triangulo::getTipo() const { return "Triangulo"; }

    void Triangulo::onUpdate(float dt) {
        imagen.setPosition(
            {
                transform->posicion.x,
                transform->posicion.y
            });
    }
}