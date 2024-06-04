import os
import json

from Scripts.utils.paths import unreal_source_dir, unreal_uproject_fpath

def request_module_create():
    module_name = input("Please enter module name: ")
    module_path = os.path.join(unreal_source_dir, module_name)

    create_module(module_name, module_path)
    write_build_file(module_name, module_path)
    write_module_implementation_files(module_name, module_path)
    include_module_in_uproject(module_name, module_path)

def create_module(module_name:str, module_path:str):
    if os.path.exists(module_path):
        os.rmdir(module_path)
    
    os.makedirs(module_path)
    os.makedirs(os.path.join(module_path, "Prviate"))
    os.makedirs(os.path.join(module_path, "Public"))

def write_build_file(module_name:str, module_path:str):
    build_fname:str = module_name+".Build.cs"
    build_fpath:str = os.path.join(module_path, build_fname)
    f = open(build_fpath, "w")
    f.write(
        'using UnrealBuildTool;\n'+
        '\n'+
        'public class '+module_name+': ModuleRules'+
        '{'+
        '\tpublic '+module_name+'(ReadOnlyTargetRules Target) : base(Target)'+
        '\t{'+
        '\t\tPrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});'+
        '\t}'+
        '}'
    )
    f.close()

def write_module_implementation_files(module_name:str, module_path:str):
    module_impl_h_fname:str = module_name+"Module.h"
    module_impl_h_path:str = os.path.join(module_path, "Public", module_impl_h_fname)
    module_impl_cpp_fname:str = module_name+"Module.cpp"
    module_impl_cpp_path:str = os.path.join(module_path, "Prviate", module_impl_cpp_fname)

    f = open(module_impl_h_path, "w")
    f.write()
    f.close()

    f = open(module_impl_cpp_path, "w")
    f.write(
        '#include "Modules/ModuleManager.h"\n'
        '\n'
        'IMPLEMENT_MODULE(FDefaultModuleImpl, '+module_name+');'
    )
    f.close()

def include_module_in_uproject(module_name:str, module_path:str):
    f = open(unreal_uproject_fpath, "r")
    lines:list[str] = f.readlines()
    fstr:str = ""
    for line in lines:
        fstr += line
    fjson:dict = json.loads(fstr)
    f.close()
    fjson["Modules"].append(
        {
            "Name":module_name,
            "Type":"Runtime"
        }
    )
    with open(unreal_uproject_fpath, 'w') as fuproj:
        json.dump(fjson, fuproj)