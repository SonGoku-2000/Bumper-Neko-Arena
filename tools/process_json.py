import json
from pathlib import Path
import shutil


import sys
import traceback


from traduction_converter_file_info import FileInfo
from process_languages import ProcessLanguages


class ProcessJSON:
    @staticmethod
    def process(file_path: str, output_folder: str, remove_invalid_name_characters: bool, verbose: bool):
        json_data: dict[str, str]
        with open(file_path, "r")as file:
            json_data = json.load(file)

        ProcessJSON._process_items(
            json_data, Path(file_path).parent.__str__(), output_folder
        )

        ProcessJSON._process_traduction_file(
            json_data, file_path, output_folder, verbose
        )

    @staticmethod
    def _process_items(json_data: dict[str, str], base_path: str, output_folder: str):
        ProcessJSON._check_same_grafic_type(json_data, base_path)
        for sprite_name in json_data.values():
            ProcessJSON._process_sprite(base_path, sprite_name, output_folder)
            ProcessJSON._process_sprite_json(
                base_path, sprite_name, output_folder
            )

    @staticmethod
    def _process_sprite(base_path: str, sprite_name: str, output_folder: str):
        sprite_path = Path(base_path).joinpath(f"{sprite_name}.bmp")
        sprite_file_info_path = Path(output_folder).joinpath(
            f"_{sprite_name}_sprite_file_info.txt")

        old_sprite_file_info = FileInfo.read(sprite_file_info_path)
        new_sprite_file_info = FileInfo.build_from_files(
            sprite_path.__str__())

        if old_sprite_file_info == new_sprite_file_info:
            return

        output_graphic_folder = Path(output_folder).joinpath("graphics")
        output_graphic_folder.mkdir(exist_ok=True, parents=True)
        shutil.copy(
            sprite_path,
            Path(output_folder).joinpath("graphics", sprite_path.name)
        )

        new_sprite_file_info.write(sprite_file_info_path.__str__())

    @staticmethod
    def _process_sprite_json(base_path: str, sprite_name: str, output_folder: str):
        json_path = Path(base_path).joinpath(f"{sprite_name}.json")
        json_file_info_path = Path(output_folder).joinpath(
            f"_{sprite_name}_json_file_info.txt")

        old_json_file_info = FileInfo.read(json_file_info_path)
        new_json_file_info = FileInfo.build_from_files(
            json_path.__str__())

        if old_json_file_info == new_json_file_info:
            return

        output_graphic_folder = Path(output_folder).joinpath("graphics")
        output_graphic_folder.mkdir(exist_ok=True, parents=True)
        shutil.copy(
            json_path,
            Path(output_folder).joinpath("graphics", json_path.name)
        )

        new_json_file_info.write(json_file_info_path.__str__())

    @staticmethod
    def _check_same_grafic_type(json_data: dict[str, str], base_path: str):
        graphic_type: str = ''
        first_graphic: str = ''
        for graphic in json_data.values():
            with open(Path(base_path).joinpath(graphic).with_suffix(".json"), "r")as file:
                json_data_graphic = json.load(file)
                if (graphic_type == ''):
                    graphic_type = json_data_graphic["type"]
                    first_graphic = graphic

                if (graphic_type != json_data_graphic["type"]):
                    try:
                        raise ValueError(
                            f'\nDifferent graphic types: "{first_graphic}.bmp" is a "{graphic_type}", and "{graphic}.bmp" is a "{json_data_graphic["type"]}"'
                        )
                    except ValueError as ex:
                        sys.stderr.write(str(ex) + '\n\n')
                        traceback.print_exc()
                        exit(-1)

    @staticmethod
    def _process_traduction_file(json_data: dict[str, str], file_path: str, output_folder: str, verbose: bool):
        ProcessJSON._add_language_to_list(json_data)

        text_file_info_path = Path(output_folder).joinpath(
            f"_{Path(file_path).name}_text_file_info.txt"
        )
        old_text_file_info: FileInfo = FileInfo.read(text_file_info_path)
        new_text_file_info: FileInfo = FileInfo.build_from_files(file_path)

        if old_text_file_info == new_text_file_info:
            return

        if (verbose):
            print("    ", Path(file_path).name)

        output_path: Path = Path(output_folder).joinpath("include")
        output_path.mkdir(exist_ok=True, parents=True)
        output_path = output_path.joinpath(
            Path(file_path).with_suffix("").stem
        )
        ProcessJSON._create_file(output_path.__str__(), json_data)
        new_text_file_info.write(text_file_info_path.__str__())

    @staticmethod
    def _add_language_to_list(json_data: dict[str, str]):
        for language in ProcessJSON._get_languages_list(json_data):
            ProcessLanguages.add_language(language)

    @staticmethod
    def _get_languages_list(json_data: dict[str, str]) -> list[str]:
        return list(json_data.keys())

    @staticmethod
    def _create_file(path: str, json_data: dict[str, str]):
        with open(path + '.hpp', 'w') as archivo:
            archivo.write('#pragma once\n')
            archivo.write('\n')

            archivo.write(ProcessJSON._get_inlcudes_string(json_data))

            archivo.write('\n')
            archivo.write('\n')

            archivo.write("namespace tranlations {\n")

            archivo.write("\n")
            # archivo.write(ProcessJSON._get_languages_string(json_data))
            archivo.write("\n")
            archivo.write(ProcessJSON._get_traduction_string(
                json_data, Path(path).name)
            )

            archivo.write("}\n")

    @staticmethod
    def _get_inlcudes_string(json_data: dict[str, str]) -> str:
        respuesta: str = ""
        respuesta += '#include "traduction_languages.hpp"\n'
        for file_name in json_data.values():
            respuesta += f'#include "bn_sprite_items_{file_name}.h"\n'
        return respuesta

    @staticmethod
    def _get_languages_string(json_data: dict[str, str]) -> str:
        respuesta: list[str] = []
        respuesta.append("enum languages {")
        for language in ProcessJSON._get_languages_list(json_data):
            respuesta.append(f"    {language},")
        respuesta.append("};")
        respuesta.append("")
        return "\n".join(respuesta)

    @staticmethod
    def _get_traduction_string(json_data: dict[str, str], name: str) -> str:
        respuesta: str = ""

        respuesta += f'bn::sprite_item {name}(languages language) {"{"}\n'

        respuesta += ProcessJSON._get_traduction_implementation(json_data)

        respuesta += "}\n"
        respuesta += "\n"

        return respuesta

    @staticmethod
    def _get_traduction_implementation(json_data: dict[str, str]) -> str:
        respuesta: str = ""
        default_sprite: str = ""

        respuesta += "    switch (language) {\n"
        for language, sprite in json_data.items():
            if (default_sprite == ""):
                default_sprite = sprite
            respuesta += f"        case languages::{language}:\n"
            respuesta += f'            return bn::sprite_items::{sprite};\n'

        respuesta += "        default:\n"
        respuesta += f'            return bn::sprite_items::{default_sprite};\n'
        respuesta += "    }\n"
        return respuesta
