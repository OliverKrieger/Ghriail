import subprocess

from Scripts.Utils.paths import unreal_uproject_fpath

# def regenerate_solution():
#     regenerate_command = [
#         unreal_build_tool_fpath, 
#         "-projectfiles", 
#         "-project=" + unreal_uproject_fpath, 
#         "-game",
#         "-engine"
#     ]
#     subprocess.run(regenerate_command, check=True)
#     print("Regenerated Visual Studio solution file.")