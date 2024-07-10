#pragma once

#include "bn_string.h"

enum idioma{
    ESPANOL,
    INGLES,
    FRANCES,
    ALEMAN
};

bn::string<12> texto_prueba(idioma idiom);
bn::string<12> texto_prueba2(idioma idiom);