#pragma once
#include <Motor/GUI/GLayer.hpp>
#include <Motor/Utils/Utils.hpp>
namespace CE {
    class GDock : public GLayer {
        public:
            ~GDock(){};
            void OnInit(const MotorConfig& des) override;
            void OnUpdate(float dt) override;
            void OnRender(void) override;
        private:
            MotorConfig des;
            unsigned int flags;
            unsigned int dx;
            unsigned int dy;
    };
}