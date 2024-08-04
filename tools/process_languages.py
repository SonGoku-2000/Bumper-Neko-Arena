from pathlib import Path

from traduction_converter_file_info import FileInfo


class ProcessLanguages:
    languages: set = set()

    @staticmethod
    def add_language(language: str) -> None:
        ProcessLanguages.languages.add(language)

    @staticmethod
    def process(output_folder: str):
        text_file_info_path = Path(output_folder).joinpath(
            "files_info",
            f"_traduction_languages_text_file_info.txt"
        )
        old_text_file_info: FileInfo = FileInfo.read(text_file_info_path)
        if(old_text_file_info.get_info() ==""):
            old_text_file_info = FileInfo.build_from_data(set())
        new_text_file_info: FileInfo = FileInfo.build_from_data(
            ProcessLanguages.languages)

        if(eval(old_text_file_info.get_info())==eval(new_text_file_info.get_info())):
            return

        output_path: Path = Path(output_folder).joinpath("include")
        output_path.mkdir(exist_ok=True, parents=True)
        output_path = output_path.joinpath("traduction_languages")

        ProcessLanguages._create_file(output_path.__str__())

        text_file_info_path.parent.mkdir(exist_ok=True, parents=True)
        new_text_file_info.write(text_file_info_path)

    @staticmethod
    def _create_file(path: str):
        with open(path + '.hpp', 'w') as archivo:
            archivo.write('#pragma once\n')

            archivo.write('\n')
            archivo.write('\n')

            archivo.write("namespace traduction {\n")
            archivo.write(ProcessLanguages._get_languages_string())

            archivo.write("}")

    @staticmethod
    def _get_languages_string() -> str:
        respuesta: str = ""
        respuesta += "enum languages {\n"
        for language in ProcessLanguages.languages:
            respuesta += f"    {language},\n"
        respuesta += "};"
        respuesta += "\n"
        return respuesta
