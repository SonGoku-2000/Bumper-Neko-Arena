from pathlib import Path
import argparse
from typing import Iterable
import csv
import sys
import traceback


def procesar_carpeta(folder_path: str, output_folder: str, recursive: bool):
    for path in Path(folder_path).iterdir():
        if path.is_file():
            procesar_archivo(path.__str__(), output_folder)

        elif recursive and path.is_dir():
            procesar_carpeta(path.__str__(), output_folder, recursive)


def procesar_archivo(file_path: str, output_folder: str):
    if (not comprobar_formato(file_path)):
        return
    with open(file_path) as file:
        data = csv.reader(file, delimiter=";")
        idiomas: list
        for fila in data:
            idiomas = fila
            idiomas.pop(0)
            break
        output_path: Path = Path(output_folder).joinpath("include")
        output_path.mkdir(exist_ok=True, parents=True)
        output_path = output_path.joinpath(Path(file_path).stem)
        crear_archivo(output_path.__str__(), idiomas, data)


def comprobar_formato(file_path: str) -> bool:
    path = Path(file_path)
    if file_path.find(" ") != -1:
        try:
            raise ValueError(
                f"\nError: Invalid file name: '{path.name}' (invalid character: ' ')")
        except ValueError as ex:
            sys.stderr.write(str(ex) + '\n\n')
            traceback.print_exc()
            raise

    if (path.suffix == ".csv" or path.suffix == ".CSV"):
        return True

    return False


def crear_archivo(path: str, idiomas: list, filas: Iterable | list[list[str]]):
    with open(path + '.hpp', 'w') as archivo:
        archivo.write('#pragma once\n')
        archivo.write('\n')
        archivo.write('#include "bn_string.h"\n')
        archivo.write('\n')
        archivo.write('\n')

        archivo.write("namespace tranlations {\n")
        archivo.write('\n')
        archivo.write(get_languages_string(idiomas))

        archivo.write('\n')

        archivo.write(get_traduction_string(idiomas, filas))
        archivo.write("}")


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
        respuesta += f'            return "{traduccion[id+1]}";\n'
    respuesta += "        default:\n"
    respuesta += '            return "";\n'
    respuesta += "    }\n"
    return respuesta


def process(output_folder: str, input_dirs: str | list[str], recursive: bool):
    traduction_paths: list[str] = []
    traduction_folder_paths: list[str] = []

    for dir in input_dirs:
        if Path(dir).is_file():
            traduction_paths.append(dir)

        elif Path(dir).is_dir():
            traduction_folder_paths.append(dir)

        else:
            try:
                raise ValueError('File or path not exist')
            except ValueError:
                print(f"'{dir}' is not a real file or path")
                raise

    for traduction_path in traduction_paths:
        procesar_archivo(traduction_path, output_folder)

    for traduction_folder_path in traduction_folder_paths:
        procesar_carpeta(traduction_folder_path, output_folder, recursive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='External tool example.')
    parser.add_argument('--output', "-o", required=True,
                        help='output folder path')

    parser.add_argument('--dirs', "-d", required=True,
                        type=str, nargs='+', help='Dirs for traductions or folder with traductions')

    parser.add_argument('--recursive', "-r", required=False, default=True,
                        type=bool, help='If a folder is given in dirs, it processes it recursively True by default.')

    # args = parser.parse_args()
    args = parser.parse_args([
        '-o', 'external_tool',
        '-d', 'traduction', 'traduccion_prueba copy.csv'
    ])

    process(args.output, args.dirs, args.recursive)
