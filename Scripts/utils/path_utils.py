import os

def find_file_with_ext(dir_path:str, file_ext:str) -> str:
    for file in os.listdir(dir_path):
        if file.endswith(file_ext):
            return file
    return ""

def get_project_name(working_dir:str) -> str:
    for file in os.listdir(working_dir):
        if file.endswith(".uproject"):
            return file.split(".")[0]
    return ""