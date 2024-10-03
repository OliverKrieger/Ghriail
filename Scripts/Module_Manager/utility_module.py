import os

from Scripts.Utils.paths import unreal_project_buildcs_fpath
from Scripts.Utils.utils import load_uproject_contents

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