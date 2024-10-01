import os
import shutil
import json

from Scripts.utils.paths import unreal_source_dir, unreal_project_buildcs_fpath, unreal_uproject_fpath
from Scripts.utils.utils import does_module_exist_in_project_build, does_module_exist_in_uproject, does_module_path_exist, load_uproject_contents, write_uproject_contents

def request_module_remove():
    module_name = str(input("Please enter module name: "))
    module_path:str = os.path.join(unreal_source_dir, module_name)

    if does_module_path_exist(module_path):
        remove_module_contents(module_path)
    else:
        print("WARNING - module contents did not exist!")

    if does_module_exist_in_project_build(module_name):
        remove_from_project_build(module_name)
    else:
        print("WARNING - module did not exist in project build file!")

    if does_module_exist_in_uproject(module_name):
        remove_from_uproject(module_name)
    else:
        print("WARNING - module did not exist in uproject file!")


def remove_module_contents(module_path:str):
    if os.path.exists(module_path):
        shutil.rmtree(module_path)
    else:
        print("ERROR - unable to remove module contents at path "+module_path)        

def remove_from_project_build(module_name:str):
    f = open(unreal_project_buildcs_fpath, "r")
    lines:list[str] = f.readlines()
    fstr:str = ""
    for line in lines:
        if "PublicDependencyModuleNames.AddRange(new string[] {" in line:
            line = line.replace(', "'+module_name+'"', '', 1).replace('  ', ' ') # replace first instance of module name and then replace any double spacing
        fstr += line
    f.close()
    
    with open(unreal_project_buildcs_fpath, 'w') as f: # write with module name removed
        f.write(fstr)
    f.close()

def remove_from_uproject(module_name:str):
    fjson:dict = load_uproject_contents()
    for idx, module in enumerate(fjson["Modules"]):
        if module["Name"] == module_name:
            del fjson["Modules"][idx]
            write_uproject_contents(fjson)
            return
    print("ERROR - Was unable to remove module from UProject file!")
    return