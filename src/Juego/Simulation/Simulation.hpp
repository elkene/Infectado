#pragma once
#include "Person.hpp"
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>


class Simulation {
public:
    Simulation(unsigned gridW = 15, unsigned gridH = 15);


    void init();
    void update(float dt);
    void render(float x0, float y0, float cellSize);


    bool hasInfectious() const;
    void countStates(int &normal, int &enfermo, int &recuperar, int &inmune, int &muerto) const;


    // helper to map mouse to cell
    void tryToggleAt(int mx, int my, float x0, float y0, float cellSize);


private:
    unsigned w,h;
    std::vector<Person> grid; // tamaño = w*h


    mutable std::mt19937 rng;
    std::uniform_real_distribution<float> uni01{0.f,1.f};


    inline Person& at(int i,int j) { return grid[j*w + i]; }
    inline const Person& atc(int i,int j) const { return grid[j*w + i]; }


    // rules parameters
    const float T_ENFERMO = 3.f; // segundos
    const float T_RECUP = 2.f; // segundos
    const float T_PARTIAL_IMM = 2.f; // segundos
};