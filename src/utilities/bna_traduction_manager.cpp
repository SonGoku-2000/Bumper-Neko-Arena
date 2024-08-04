#include "bna_traduction_manager.hpp"


bn::string<12> texto_prueba(idioma idiom) {
    switch (idiom) {
        case idioma::ESPANOL:
            return "Hola mundo";
        case idioma::INGLES:
            return "Hellow world";
        case idioma::FRANCES:
            return "Hola fran";
        case idioma::ALEMAN:
            return "Hola alem";
        default:
            return "";
    }
}
