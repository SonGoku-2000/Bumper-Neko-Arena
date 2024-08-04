#include "bna_memory.hpp"

#include "bn_sram.h"
#include "bn_string_view.h"

#include "bna_brightness_manager.hpp"
#include "bna_parts.hpp"

#define DEBUG
#ifdef DEBUG
#include "bn_log.h"
#endif

bna::Memory::Memory() {
    if (read()) {
#ifdef DEBUG
        BN_LOG("Memory leida correctamente");
#endif
    }
    else {
#ifdef DEBUG
        BN_LOG("Error al leer Memory creando nuevos datos");
#endif
        brillo = 0;

        motores.fill(parts::motors::EMPTY);
        motores[0] = parts::motors::SLOW;

        cuerpos.fill(parts::bodys::EMPTY);
        cuerpos[0] = parts::bodys::LIGHT;

        ruedas.fill(parts::wheels::EMPTY);
        ruedas[0] = parts::wheels::OLD;
    }

    bna::brightness_manager::set_brightness(brillo);

    write();
}



[[nodiscard]] bool bna::Memory::read() {
    bn::sram::read(*this);
    return bn::string_view(label) == valid_label;
}

void bna::Memory::write() {
#ifdef DEBUG
    BN_LOG("Escribiendo datos...");
#endif
    bn::istring_base label_istring(label);
    bn::ostringstream label_stream(label_istring);
    label_stream.append(valid_label);
    bn::sram::write(*this);
#ifdef DEBUG
    BN_LOG("Datos escritos: ", sizeof(*this), " bytes.");
#endif
}

