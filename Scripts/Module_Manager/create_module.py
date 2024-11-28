import os
import json

from Scripts.Utils.paths import unreal_source_dir, unreal_uproject_fpath, unreal_project_buildcs_fpath, project_name
from Scripts.Utils.utils import load_uproject_contents
from .utility_module import does_module_path_exist, does_module_exist_in_project_build, does_module_exist_in_uproject

def request_module_create():
    module_name = str(input("Please enter module name: "))
    module_path = os.path.join(unreal_source_dir, module_name)
    
    if does_module_path_exist(module_name):
        print("ERROR - module folder already exists!")
        return

    if does_module_exist_in_project_build(module_name):
        print("ERROR - module already exists in Project "+project_name+" build file!")
        return

    if does_module_exist_in_uproject(module_name):
        print("ERROR - module already exists in UProject file!")
        return

    create_module_folders(module_path)
    write_build_file(module_name, module_path)
    write_module_implementation_files(module_name, module_path)
    write_module_log_category(module_name, module_path)
    include_module_in_uproject(module_name)
    include_module_in_project_build(module_name)

def create_module_folders(module_path:str):
    os.makedirs(module_path)
    os.makedirs(os.path.join(module_path, "Private"))
    os.makedirs(os.path.join(module_path, "Public"))

def write_build_file(module_name:str, module_path:str):
    build_fname:str = module_name+".Build.cs"
    build_fpath:str = os.path.join(module_path, build_fname)
    f = open(build_fpath, "w")
    f.write(
        'using UnrealBuildTool;\n'+
        '\n'+
        'public class '+module_name+': ModuleRules'+
        '{\n'+
        '\tpublic '+module_name+'(ReadOnlyTargetRules Target) : base(Target)'+
        '\t{\n'+
        '\t\tPrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});\n'+
        '\t}\n'+
        '}'
    )
    f.close()

def write_module_implementation_files(module_name:str, module_path:str):
    module_impl_h_fname:str = module_name+".h"
    module_impl_h_path:str = os.path.join(module_path, "Public", module_impl_h_fname)
    module_impl_cpp_fname:str = module_name+".cpp"
    module_impl_cpp_path:str = os.path.join(module_path, "Private", module_impl_cpp_fname)

    f = open(module_impl_h_path, "w")
    f.write(
        '#pragma once'
        '\n'
        '#include "CoreMinimal.h"\n'
        '#include "Modules/ModuleManager.h"\n'
        '\n'
        'class F'+module_name+' : public IModuleInterface\n'
        '{\n'
        'public:\n'
        '\tvirtual void StartupModule() override;\n'
        '\tvirtual void ShutdownModule() override;\n'
        '};'
    )
    f.close()

    f = open(module_impl_cpp_path, "w")
    f.write(
        '#include "'+module_name+'.h"\n'
        '#include "Modules/ModuleManager.h"\n'
        '\n'
        'IMPLEMENT_MODULE(F'+module_name+', '+module_name+');'
        '\n'
        'void F'+module_name+'::StartupModule()\n'
        '{\n'
        '\n'
        '}\n'
        'void F'+module_name+'::ShutdownModule()\n'
        '{\n'
        '\n'
        '}\n'
    )
    f.close()

def write_module_log_category(module_name:str, module_path:str):
    module_log_cat_h_fname:str = module_name+"_LogCategory.h"
    module_log_cat_h_path:str = os.path.join(module_path, "Public", module_log_cat_h_fname)
    module_log_cat_cpp_fname:str = module_name+"_LogCategory.cpp"
    module_log_cat_cpp_path:str = os.path.join(module_path, "Private", module_log_cat_cpp_fname)

    f = open(module_log_cat_h_path, "w")
    f.write(
        '#pragma once\n'
        '\n'
        '#include "CoreMinimal.h"\n'
        '\n'
        'DECLARE_LOG_CATEGORY_EXTERN('+module_name+'_LogCategory, Log, All);'
    )
    f.close()

    f = open(module_log_cat_cpp_path, "w")
    f.write(
        '#include "'+module_log_cat_h_fname+'"\n'
        '\n'
        'DEFINE_LOG_CATEGORY('+module_name+'_LogCategory);'
    )
    f.close()


def include_module_in_uproject(module_name:str):
    fjson:dict = load_uproject_contents()
    fjson["Modules"].append(
        {
            "Name":module_name,
            "Type":"Runtime",
            "LoadingPhase": "Default"
        }
    )

    with open(unreal_uproject_fpath, 'w') as f:
        f.write(json.dumps(fjson, indent=4))
    f.close()

def include_module_in_project_build(module_name:str):
    f = open(unreal_project_buildcs_fpath, "r")
    lines:list[str] = f.readlines()
    fstr:str = ""
    for line in lines:
        if "PublicDependencyModuleNames.AddRange(new string[] {" in line:
            line_split = line.split("{")
            line_start = line_split[0]
            line_split = line_split[1].split("}")
            line_mid = line_split[0]
            line_end = line_split[1]
            line_mid = line_mid+', "'+module_name+'"'
            line = line_start+"{"+line_mid+" }"+line_end
        fstr += line
    f.close()
    
    with open(unreal_project_buildcs_fpath, 'w') as f:
        f.write(fstr)
    f.close()