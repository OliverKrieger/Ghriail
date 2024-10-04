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

def get_project_solution(working_dir:str, file_ext:str) -> str:
    project_name:str = get_project_name(working_dir)
    sln_name:str = find_file_with_ext(working_dir, file_ext)
    return project_name+".sln" if sln_name == "" else sln_name