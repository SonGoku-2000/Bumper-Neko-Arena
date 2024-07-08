from typing import Iterable
import csv


def crear_archivo(path: str, idiomas: list, filas: Iterable | list[list[str]]):
    with open(path + '.hpp', 'w') as archivo:
        archivo.write('#pragma once\n')
        archivo.write('\n')
        archivo.write('#include "bn_string.h"\n')

        archivo.write(get_languages_string(idiomas))

        archivo.write('\n')

        archivo.write(get_traduction_string(idiomas, filas))


def get_languages_string(languages: list[str]) -> str:
    respuesta: list[str] = []
    respuesta.append("enum languages {")

    for language in languages:
        respuesta.append(f"    {language},")

    respuesta.append("};")
    respuesta.append("")
    return "\n".join(respuesta)


def get_traduction_string(languages: list[str], filas: Iterable | list[list[str]]) -> str:
    respuesta: str = ""
    for fila in filas:
        respuesta += f'bn::string<{get_max_lenght_string(fila[1:])}> {fila[0]}(languages language) {"{"}\n'
        respuesta += get_traduction_implementation(languages, fila)
        respuesta += "}\n"
        respuesta += "\n"

    return respuesta


def get_max_lenght_string(texts: list[str]) -> int:
    max_num: int = 0
    for text in texts:
        max_num = max(max_num, len(text))
    return max_num


def get_traduction_implementation(languages: list[str], traduccion: list[str]) -> str:
    respuesta: str = ""

    respuesta += "    switch (language) {\n"
    for id, language in enumerate(languages):
        respuesta += f"        case languages::{language}:\n"
        respuesta += f'            return "{traduccion[id]}";\n'
    respuesta += "        default:\n"
    respuesta += '            return "";\n'
    respuesta += "    }\n"
    return respuesta


with open("traduction/traduccion_prueba.csv") as file:
    data = csv.reader(file, delimiter=";")
    idiomas: list
    for fila in data:
        idiomas = fila
        idiomas.pop(0)
        break
    crear_archivo("archivo_prueba", idiomas, data)
