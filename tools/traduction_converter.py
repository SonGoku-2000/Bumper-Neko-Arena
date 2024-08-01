import argparse
import sys
import traceback

from pathlib import Path

from traduction_converter_file_info import FileInfo
from process_csv import ProcessCSV
from process_json import ProcessJSON


def procesar_carpeta(folder_path: str, output_folder: str, recursive: bool, remove_invalid_name_characters: bool, remove_invalid_csv_value_characters: bool, verbose: bool, delimiter: str):
    for path in Path(folder_path).iterdir():
        if path.is_file():
            procesar_archivo(path.__str__(), output_folder,
                             remove_invalid_name_characters, remove_invalid_csv_value_characters,
                             verbose, delimiter
                             )

        elif recursive and path.is_dir():
            procesar_carpeta(path.__str__(), output_folder, recursive,
                             remove_invalid_name_characters, remove_invalid_csv_value_characters,
                             verbose, delimiter
                             )


def procesar_archivo(file_path: str, output_folder: str, remove_invalid_name_characters: bool, remove_invalid_csv_value_characters: bool, verbose: bool, delimiter: str):
    if (FileInfo.check_extencion(file_path) == "csv"):
        ProcessCSV.process(file_path, output_folder,
                           remove_invalid_name_characters, remove_invalid_csv_value_characters,
                           verbose, delimiter
                           )

    elif (FileInfo.check_extencion(file_path) == "json"):
        ProcessJSON.process(file_path, output_folder,
                            remove_invalid_name_characters,
                            verbose
                            )


def process(output_folder: str, input_dirs: str | list[str], recursive: bool, remove_invalid_name_characters: bool, remove_invalid_csv_value_characters: bool, verbose: bool, delimiter: str):
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
                         remove_invalid_name_characters, remove_invalid_csv_value_characters,
                         verbose, delimiter)

    for traduction_folder_path in traduction_folder_paths:
        procesar_carpeta(traduction_folder_path, output_folder, recursive,
                         remove_invalid_name_characters, remove_invalid_csv_value_characters,
                         verbose, delimiter)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='External tool example.')
    parser.add_argument('--output', "-o", required=True,
                        help='output folder path')

    parser.add_argument('--dirs', "-d", required=True,
                        type=str, nargs='+', help='Dirs for traductions or folder with traductions')

    parser.add_argument('--recursive', "-r", required=False, default=True,
                        type=bool, help='If a folder is given in dirs, it processes it recursively True by default.')

    parser.add_argument('--remove_invalid_name_characters', "-rmn",
                        action='store_true', help='Remove invalid characters from the final name')

    parser.add_argument('--remove_invalid_csv_characters', "-rmcsv",
                        action='store_true', help='Remove invalid characters from the parameters of the csv')

    parser.add_argument('--delimiter', "-de", required=False, default=";",
                        type=str, help='Delimiter for the cels values')

    parser.add_argument('--verbose', '-v', action='store_true')

    # args = parser.parse_args()
    args = parser.parse_args([
        '-o', 'external_tool',
        '-d', 'traduction',
        "-v",
        "-rmn",
        "-rmcsv",
        "-de", ','
    ])

    process(args.output, args.dirs, args.recursive,
            args.remove_invalid_name_characters, args.remove_invalid_csv_characters,
            args.verbose, args.delimiter)
