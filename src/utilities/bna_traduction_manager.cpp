#include "bna_traduction_manager.hpp"


bn::string<12> texto_prueba(idioma idiom){
    if(idioma::ESPANOL == idiom){
        return "Hola mundo";
    }
    else if(idioma::INGLES == idiom){
        return "Hellow world";
    }
}


bn::string<12> texto_prueba2(idioma idiom){
    if(idioma::ESPANOL == idiom){
        return bn::string<12>("Hola mundo");
    }
    else if(idioma::INGLES == idiom){
        return bn::string<12>("Hellow world");
    }
}