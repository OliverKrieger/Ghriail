import os
import json

from .paths_external import engine_folders

def find_engine_version(uproject_fpath:str) -> str | None:
    with open(uproject_fpath, 'r') as f:
        project_data = json.load(f)
    
    # Get the Engine Association version
    engine_version = project_data.get('EngineAssociation', None)
    
    if not engine_version:
        print("EngineAssociation not found in .uproject file.")
        return None
    return engine_version

def find_engine_path(engine_version:str) -> str:
    if engine_folders == []:
        print("ERROR - Engine folder not set in paths_external, please add an engine path!")
        return ""
    
    print("Engine Folders: ", engine_folders)
    for engine_folder in engine_folders:
        engine_path:str = os.path.join(engine_folder, find_engine_name_at_path(engine_folder, engine_version))
        print("Path: "+engine_path)
        if os.path.exists(engine_path):
            print("Path found: "+engine_path)
            return engine_path
    
    print("ERROR no engines found at locations:")
    for folder_path in engine_folders:
        print(folder_path+"\n")
    return ""

def find_engine_name_at_path(engine_folder:str, engine_version:str) -> str:
    folder_names:list[str] = os.listdir(engine_folder)
    for folder_name in folder_names:
        if engine_version in folder_name:
            return folder_name

    print("ERROR - no engine found at location "+engine_folder+" that contains version "+engine_version)
    return ""