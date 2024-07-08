from typing import Iterable
import csv


def get_max_lenght_string(texts: list[str]) -> int:
    max_num: int = 0
    for text in texts:
        max_num = max(max_num, len(text))
    return max_num


def crear_archivo(path: str, idiomas: list, filas: Iterable | list[list[str]]):
    with open(path + '.hpp', 'w') as archivo:
        archivo.write('#pragma once\n')
        archivo.write('\n')
        archivo.write('#include "bn_string.h"\n')

        for fila in filas:
            archivo.write(f'bn::string<{get_max_lenght_string(fila[1:])}> texto_prueba();\n')


with open("traduction/traduccion_prueba.csv") as file:
    data = csv.reader(file, delimiter=";")
    print(type(data))
    idiomas: list
    for fila in data:
        idiomas = fila
        idiomas.pop(0)
        break
    print(idiomas)
    crear_archivo("archivo_prueba", idiomas, data)
