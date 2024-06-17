#include "bna_memory.hpp"

#include "bn_sram.h"
#include "bn_string_view.h"

// #define DEBUG
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
        dinero = 0;

        volumen = bn::fixed(0.5);
    }
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
    BN_LOG("Datos escritos");
#endif
}