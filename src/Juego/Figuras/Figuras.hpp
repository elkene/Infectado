#pragma once
#include <Motor/Primitivos/Objetos.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>
#include <string>

namespace IVJ {
    class Figuras : public CE::Objeto {
    public:
        Figuras(int lados, float x, float y, const sf::Color &relleno, const sf::Color& contorno);
        Figuras(){};
        virtual ~Figuras(){};
        virtual void setPosicion(float x, float y) = 0;
        virtual float getArea() const = 0;
        static std::shared_ptr<Figuras> deserialize(const std::string& line);
        virtual std::string getTipo() const = 0;


    protected:
        int lados;
        sf::Color c_relleno;
        sf::Color c_contorno;


    };


    class Rectangulo : public Figuras {

    public:
        Rectangulo(float ancho, float largo, float x, float y, const sf::Color& relleno, const sf::Color& contorno);
        Rectangulo(){};
        ~Rectangulo(){};
        void setPosicion(float x, float y) override;
        void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
        float getArea() const override;
        virtual std::string getTipo() const override;
        void onUpdate(float dt) override;
    private:
        sf::RectangleShape imagen;
        float w;
        float h;

    };

    class Circulo : public Figuras {
    public:
        Circulo(float radio, float x, float y, const sf::Color& relleno, const sf::Color& contorno);
        ~Circulo(){};
        void setPosicion(float x, float y) override;
        void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
        float getArea() const override;
        virtual std::string getTipo() const override;
        void onUpdate(float dt) override;

    private:
        sf::CircleShape imagen;
        float radio;
    };

    class Triangulo : public Figuras {
    public:
        Triangulo(float l, float x, float y, const sf::Color& relleno, const sf::Color& contorno);
        ~Triangulo(){};
        void setPosicion(float x, float y) override;
        void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
        float getArea() const override;
        virtual std::string getTipo() const override;
        void onUpdate(float dt) override;
    private:
        sf::ConvexShape imagen;
        float l;
    };


}