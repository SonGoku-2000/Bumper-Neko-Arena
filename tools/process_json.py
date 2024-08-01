import json
from pathlib import Path
import shutil

from traduction_converter_file_info import FileInfo


class ProcessJSON:
    @staticmethod
    def process(file_path: str, output_folder: str, remove_invalid_name_characters: bool, verbose: bool):
        json_data: dict[str, str]
        with open(file_path, "r")as file:
            json_data = json.load(file)

        file_output_path: str = file_path
        ProcessJSON._process_items(
            json_data, Path(file_path).parent.__str__(), output_folder
        )

        text_file_info_path = Path(output_folder).joinpath(
            f"_{Path(file_path).name}_text_file_info.txt"
        )
        old_text_file_info: FileInfo = FileInfo.read(text_file_info_path)
        new_text_file_info: FileInfo = FileInfo.build_from_files(file_path)

    @staticmethod
    def _process_items(json_data: dict[str, str], base_path: str, output_folder: str):
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
