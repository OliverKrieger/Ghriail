import os

def find_file_with_ext(dir_path:str, file_ext:str):
    for file in os.listdir(dir_path):
        if file.endswith(file_ext):
            return file
    return []