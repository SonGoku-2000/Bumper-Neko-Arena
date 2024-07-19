from pathlib import Path
import argparse
from typing import Iterable
import csv
import sys
import traceback
import string
from traduction_converter_file_info import FileInfo


def procesar_carpeta(folder_path: str, output_folder: str, recursive: bool, remove_invalid_characters: bool, verbose: bool, delimiter: str):
    for path in Path(folder_path).iterdir():
        if path.is_file():
            procesar_archivo(path.__str__(), output_folder,
                             remove_invalid_characters, verbose, delimiter)

        elif recursive and path.is_dir():
            procesar_carpeta(path.__str__(), output_folder, recursive,
                             remove_invalid_characters, verbose, delimiter)


def procesar_archivo(file_path: str, output_folder: str, remove_invalid_characters: bool, verbose: bool, delimiter: str):
    file_output_path: str = file_path
    if (remove_invalid_characters):
        file_output_path = eliminar_caracteres_invalidos_nombre_archivo(
            file_output_path)

    if (not FileInfo.validate(file_output_path)):
        return

    text_file_info_path = Path(output_folder).joinpath(
        f"_{Path(file_path).name}_text_file_info.txt"
    )

    old_text_file_info = FileInfo.read(text_file_info_path)
    new_text_file_info = FileInfo.build_from_files(file_path)
    if old_text_file_info == new_text_file_info and old_text_file_info == new_text_file_info:
        return

    if (verbose):
        print("    ", Path(file_path).name)

    with open(file_path) as file:
        data = csv.reader(file, delimiter=delimiter)

        idiomas: list[str] = generar_lista_idiomas(
            data, remove_invalid_characters)

        output_path: Path = Path(output_folder).joinpath("include")
        output_path.mkdir(exist_ok=True, parents=True)
        output_path = output_path.joinpath(Path(file_output_path).stem)
        crear_archivo(output_path.__str__(), idiomas,
                      data, remove_invalid_characters)

    new_text_file_info.write(text_file_info_path.__str__())


def generar_lista_idiomas(datos_csv: Iterable | list[list[str]], remove_invalid_characters: bool) -> list[str]:
    idiomas: list[str] = []
    for fila in datos_csv:
        idiomas = fila
        idiomas.pop(0)
        break

    for id, idioma in enumerate(idiomas):
        if (remove_invalid_characters):
            idiomas[id] = eliminar_caracteres_invalidos_funciones(idioma)
        else:
            comprobar_caracteres_invalidos_funciones(
                idioma, f'Invalid language name "{idioma}" at column "{id+1}" of csv file'
            )

    return idiomas


def comprobar_caracteres_invalidos_funciones(texto: str, error_mesage: str = "Invalid value") -> None:
    for character in texto:
        if character not in get_caracteres_validos():
            try:
                raise ValueError(
                    f'\n{error_mesage} - Error:(invalid character: "{character}")'
                )
            except ValueError as ex:
                sys.stderr.write(str(ex) + '\n\n')
                traceback.print_exc()
                exit(-1)


def get_caracteres_validos():
    return '_%s%s' % (string.ascii_letters, string.digits)


def eliminar_caracteres_invalidos_funciones(texto: str):
    texto = texto.replace(" ", "_")

    valid_characters = get_caracteres_validos()
    for character in texto:
        if character not in valid_characters:
            texto = texto.replace(character, "")

    return texto


def eliminar_caracteres_invalidos_nombre_archivo(texto: str):
    texto = texto.lower()
    texto = texto.replace(" ", "_")

    extencion: str = Path(texto).suffix
    texto = Path(texto).stem

    aux: str = ""
    valid_characters = '_%s%s' % (string.ascii_lowercase, string.digits)
    for character in texto:
        if character not in valid_characters:
            texto = texto.replace(character, "")

    return texto+extencion


def crear_archivo(path: str, idiomas: list, filas: Iterable | list[list[str]], remove_invalid_characters: bool):
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

        archivo.write(get_traduction_string(
            idiomas, filas, remove_invalid_characters))
        archivo.write("}")


def get_languages_string(languages: list[str]) -> str:
    respuesta: list[str] = []
    respuesta.append("enum languages {")

    for language in languages:
        respuesta.append(f"    {language},")

    respuesta.append("};")
    respuesta.append("")
    return "\n".join(respuesta)


def get_traduction_string(languages: list[str], filas: Iterable | list[list[str]], remove_invalid_characters: bool) -> str:
    respuesta: str = ""
    for id, fila in enumerate(filas):
        if (remove_invalid_characters):
            fila[0] = eliminar_caracteres_invalidos_funciones(fila[0])
        else:
            comprobar_caracteres_invalidos_funciones(
                fila[0], f'Invalid value name "{fila[0]}" at row "{id + 2}" of csv file'
            )

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
        respuesta += f'            return "{get_formato_correcto(traduccion[id+1])}";\n'
    respuesta += "        default:\n"
    respuesta += '            return "";\n'
    respuesta += "    }\n"
    return respuesta

def get_formato_correcto(text:str)->str:
    respuesta:str
    respuesta= text.replace('"','\\"')
    return respuesta


def process(output_folder: str, input_dirs: str | list[str], recursive: bool, remove_invalid_characters: bool, verbose: bool, delimiter: str):
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
            except ValueError as ex:
                sys.stderr.write(str(ex) + '\n\n')
                traceback.print_exc()
                print(f"'{dir}' is not a real file or path")
                exit(-1)

    for traduction_path in traduction_paths:
        procesar_archivo(traduction_path, output_folder,
                         remove_invalid_characters, verbose, delimiter)

    for traduction_folder_path in traduction_folder_paths:
        procesar_carpeta(traduction_folder_path, output_folder,
                         recursive, remove_invalid_characters, verbose, delimiter)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='External tool example.')
    parser.add_argument('--output', "-o", required=True,
                        help='output folder path')

    parser.add_argument('--dirs', "-d", required=True,
                        type=str, nargs='+', help='Dirs for traductions or folder with traductions')

    parser.add_argument('--recursive', "-r", required=False, default=True,
                        type=bool, help='If a folder is given in dirs, it processes it recursively True by default.')

    parser.add_argument('--remove_invalid_characters', "-rm",
                        action='store_true', help='Remove invalid characters from the final name')

    parser.add_argument('--delimiter', "-de", required=False, default=";",
                        type=str, help='Delimiter for the cels values')

    parser.add_argument('--verbose', '-v', action='store_true')

    args = parser.parse_args()
    # args = parser.parse_args([
    #     '-o', 'external_tool',
    #     '-d', 'traduction',
    #     "-v",
    #     "-rm",
    #     "-de", ','
    # ])

    process(args.output, args.dirs, args.recursive,
            args.remove_invalid_characters, args.verbose, args.delimiter)
