import sys
import traceback

from typing import Iterable
import string

from pathlib import Path
import csv

from traduction_converter_file_info import FileInfo


class ProcessCSV:
    @staticmethod
    def process(file_path: str, output_folder: str, remove_invalid_name_characters: bool, remove_invalid_csv_value_characters: bool, verbose: bool, delimiter: str):
        file_output_path: str = file_path
        if (remove_invalid_name_characters):
            file_output_path = FileInfo.remove_invalid_name_characters(
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

            idiomas: list[str] = ProcessCSV.generar_lista_idiomas(
                data, remove_invalid_csv_value_characters)

            output_path: Path = Path(output_folder).joinpath("include")
            output_path.mkdir(exist_ok=True, parents=True)
            output_path = output_path.joinpath(Path(file_output_path).stem)
            ProcessCSV.crear_archivo(output_path.__str__(), idiomas,
                                     data, remove_invalid_csv_value_characters)

        new_text_file_info.write(text_file_info_path.__str__())

    @staticmethod
    def generar_lista_idiomas(datos_csv: Iterable | list[list[str]], remove_invalid_characters: bool) -> list[str]:
        idiomas: list[str] = []
        for fila in datos_csv:
            idiomas = fila
            idiomas.pop(0)
            break

        for id, idioma in enumerate(idiomas):
            if (remove_invalid_characters):
                idiomas[id] = ProcessCSV.eliminar_caracteres_invalidos_funciones(
                    idioma)
            else:
                ProcessCSV.comprobar_caracteres_invalidos_funciones(
                    idioma, f'Invalid language name "{idioma}" at column "{id+1}" of csv file'
                )

        return idiomas

    @staticmethod
    def crear_archivo(path: str, idiomas: list, filas: Iterable | list[list[str]], remove_invalid_characters: bool):
        with open(path + '.hpp', 'w') as archivo:
            archivo.write('#pragma once\n')
            archivo.write('\n')
            archivo.write('#include "bn_string.h"\n')
            archivo.write('\n')
            archivo.write('\n')

            archivo.write("namespace tranlations {\n")
            archivo.write('\n')
            archivo.write(ProcessCSV.get_languages_string(idiomas))

            archivo.write('\n')

            archivo.write(ProcessCSV.get_traduction_string(
                idiomas, filas, remove_invalid_characters))
            archivo.write("}")

    @staticmethod
    def get_languages_string(languages: list[str]) -> str:
        respuesta: list[str] = []
        respuesta.append("enum languages {")

        for language in languages:
            respuesta.append(f"    {language},")

        respuesta.append("};")
        respuesta.append("")
        return "\n".join(respuesta)

    @staticmethod
    def get_traduction_string(languages: list[str], filas: Iterable | list[list[str]], remove_invalid_characters: bool) -> str:
        respuesta: str = ""
        for id, fila in enumerate(filas):
            if (len(fila[0]) == 0):
                continue

            if (remove_invalid_characters):
                fila[0] = ProcessCSV.eliminar_caracteres_invalidos_funciones(
                    fila[0])
            else:
                ProcessCSV.comprobar_caracteres_invalidos_funciones(
                    fila[0], f'Invalid value name "{fila[0]}" at row "{id + 2}" of csv file'
                )

            respuesta += f'bn::string<{ProcessCSV.get_max_lenght_string(fila[1:])}> {fila[0]}(languages language) {"{"}\n'
            respuesta += ProcessCSV.get_traduction_implementation(
                languages, fila)
            respuesta += "}\n"
            respuesta += "\n"

        return respuesta

    @staticmethod
    def get_traduction_implementation(languages: list[str], traduccion: list[str]) -> str:
        respuesta: str = ""

        respuesta += "    switch (language) {\n"
        for id, language in enumerate(languages):
            respuesta += f"        case languages::{language}:\n"
            respuesta += f'            return "{ProcessCSV.get_formato_correcto(traduccion[id+1])}";\n'
        respuesta += "        default:\n"
        respuesta += '            return "";\n'
        respuesta += "    }\n"
        return respuesta

    @staticmethod
    def get_max_lenght_string(texts: list[str]) -> int:
        max_num: int = 0
        for text in texts:
            max_num = max(max_num, len(text))
        return max_num

    @staticmethod
    def get_formato_correcto(text: str) -> str:
        respuesta: str
        respuesta = text.replace('"', '\\"')
        return respuesta

    @staticmethod
    def eliminar_caracteres_invalidos_funciones(texto: str):
        texto = texto.replace(" ", "_")

        valid_characters = ProcessCSV.get_caracteres_validos()
        for character in texto:
            if character not in valid_characters:
                texto = texto.replace(character, "")

        return texto

    @staticmethod
    def comprobar_caracteres_invalidos_funciones(texto: str, error_mesage: str = "Invalid value") -> None:
        for character in texto:
            if character not in ProcessCSV.get_caracteres_validos():
                try:
                    raise ValueError(
                        f'\n{error_mesage} - Error:(invalid character: "{character}")'
                    )
                except ValueError as ex:
                    sys.stderr.write(str(ex) + '\n\n')
                    traceback.print_exc()
                    exit(-1)

    @staticmethod
    def get_caracteres_validos():
        return '_%s%s' % (string.ascii_letters, string.digits)
