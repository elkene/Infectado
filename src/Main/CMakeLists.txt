project(CimaEngine3v)

add_executable(CimaEngine3v main.cpp
        ../Motor/Primitivos/Objetos.cpp
        ../Motor/Primitivos/Objetos.hpp
        ../Juego/Figuras/Figuras.cpp
        ../Juego/Figuras/Figuras.hpp
        ../Motor/Utils/Vector2D.cpp
        ../Motor/Utils/Vector2D.hpp
        ../Juego/Componentes/JComponentes.cpp
        ../Juego/Componentes/JComponentes.hpp
        ../Motor/Componentes/IComponentes.cpp
        ../Motor/Componentes/IComponentes.hpp
        ../Juego/Sistemas/Sistemas.cpp
        ../Juego/Sistemas/Sistemas.hpp
        ../Motor/Utils/CEInterfaces.hpp
        ../Motor/Utils/Nodo.hpp
        ../Motor/Utils/Lista.hpp
        ../Motor/GUI/GLogger.cpp
        ../Motor/GUI/GLogger.hpp
        ../Juego/Simulation/Person.hpp
        ../Juego/Simulation/Simulation.cpp
        ../Juego/Simulation/Simulation.hpp
        ../Juego/Infeccion.cpp
        ../Juego/Infeccion.hpp


)
