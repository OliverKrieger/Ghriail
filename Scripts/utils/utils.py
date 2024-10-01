import os
import json
from .paths import unreal_uproject_fpath, unreal_project_buildcs_fpath

def load_uproject_contents() -> dict:
    f = open(unreal_uproject_fpath, "r")
    lines:list[str] = f.readlines()
    fstr:str = ""
    for line in lines:
        fstr += line
    fjson:dict = json.loads(fstr)
    f.close()
    return fjson

def write_uproject_contents(data:dict):
    with open(unreal_uproject_fpath, 'w') as f:
        f.write(json.dumps(data, indent=4))
    f.close()

def does_module_exist_in_uproject(module_name:str) -> bool:
    fjson:dict = load_uproject_contents()
    for module in fjson["Modules"]:
        if module["Name"] == module_name:
            return True
    return False

def does_module_exist_in_project_build(module_name:str) -> bool:
    f = open(unreal_project_buildcs_fpath, "r")
    lines:list[str] = f.readlines()
    for line in lines:
        if "PublicDependencyModuleNames.AddRange(new string[] {" in line and module_name in line:
            return True
    return False

def does_module_path_exist(module_path:str) -> bool:
    return os.path.exists(module_path)