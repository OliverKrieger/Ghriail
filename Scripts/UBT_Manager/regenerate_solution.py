import subprocess

from Scripts.Utils.paths import unreal_uproject_path, engine_build_tool_fpath

def regenerate_solution():
    regenerate_command = [
        engine_build_tool_fpath, 
        "-projectfiles", 
        "-project="+unreal_uproject_path, 
        "-game",
        "-rocket" # make -engine if source build
    ]
    subprocess.run(regenerate_command, check=True)
    print("Regenerated Visual Studio solution file.")