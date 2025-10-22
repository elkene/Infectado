#pragma once
#include <Motor/GUI/GLayer.hpp>
namespace CE {
    class GViewport : public GLayer {
        public:
            ~GViewport(){};
            void OnInit(const MotorConfig& des) override;
            void OnUpdate(float dt) override;
            void OnRender(void) override;

        private:
            unsigned int vw;
            unsigned int vh;
            MotorConfig motor_info;
    };
}