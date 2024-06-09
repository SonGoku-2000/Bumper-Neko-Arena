#pragma once

#include "bn_camera_ptr.h"

#include "bn_fixed_point.h"
#include "bn_size.h"


namespace bna {
    class CameraManager {
        public:
        CameraManager(bn::camera_ptr& camera, bn::size size);
        ~CameraManager() = default;

        void update(bn::fixed_point pos);

        void setLimitLeft(int limitLeft);
        void setLimitRight(int limitRight);
        void setLimitUp(int limitUp);
        void setLimitDown(int limitDown);

        private:
        bn::camera_ptr _camera;
        int _limitLeft;
        int _limitRight;
        int _limitUp;
        int _limitDown;
    };
} // namespace rs
