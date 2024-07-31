import string
from pathlib import Path
import os

import sys
import traceback


class FileInfo:
    @staticmethod
    def validate(file_name):
        path = Path(file_name)
        if (path.suffix == ".csv" or path.suffix == ".CSV"):
            FileInfo.__comprobar_caracteres_validos(path.name)
            return True

        return False

    @staticmethod
    def remove_invalid_name_characters(text: str) -> str:
        text = text.lower()
        text = text.replace(" ", "_")

        extencion: str = Path(text).suffix
        text = Path(text).stem

        aux: str = ""
        valid_characters = '_%s%s' % (string.ascii_lowercase, string.digits)
        for character in text:
            if character not in valid_characters:
                text = text.replace(character, "")

        return text+extencion

    @staticmethod
    def __comprobar_caracteres_validos(file_name: str):
        valid_characters = '_%s%s' % (string.ascii_lowercase, string.digits)
        for character in Path(file_name).stem:
            if character not in valid_characters:
                try:
                    raise ValueError(
                        f'\nInvalid file name: {file_name} (invalid character: "{character}")'
                    )
                except ValueError as ex:
                    sys.stderr.write(str(ex) + '\n\n')
                    traceback.print_exc()
                    exit(-1)

    @staticmethod
    def read(file_path):
        info = ''
        read_failed = True

        if os.path.isfile(file_path):
            with open(file_path, 'r', encoding='utf-8') as file:
                info = file.read()
                read_failed = False

        return FileInfo(info, read_failed)

    @staticmethod
    def build_from_files(file_path: str):
        info = []

        info.append(file_path)
        info.append(str(os.path.getmtime(file_path)))

        return FileInfo('\n'.join(info), False)

    def __init__(self, info, read_failed):
        self.__info = info
        self.__read_failed = read_failed

    def write(self, file_path):
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(self.__info)

    def __eq__(self, other):
        return self.__info == other.__info and self.__read_failed == other.__read_failed

    def __ne__(self, other):
        return self.__info != other.__info or self.__read_failed != other.__read_failed

    def __repr__(self):
        if self.__read_failed:
            return '[read failed]'

        return self.__info


if __name__ == "__main__":
    FileInfo.validate("test.csv")
