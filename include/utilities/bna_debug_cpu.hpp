#pragma once

#include "bn_fixed.h"
#include "bn_log.h"
#include "bn_core.h"


namespace bna {
    template <int ciclos = 128>
    void debug_cpu() {
        static int cpuCont = 0;
        static bn::fixed cpu = 0;
        if (cpuCont == ciclos) {
            BN_LOG("CPU : % ", cpu / ciclos * 100);
            cpu = 0;
            cpuCont = 0;
        }
        else {
            cpu += bn::core::last_cpu_usage();
            cpuCont++;
        }
    }
} // namespace rs
