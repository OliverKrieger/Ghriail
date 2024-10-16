import os
from .path_utils import find_file_with_ext, get_project_name, get_project_solution
from .utils_engine import find_engine_version, find_engine_path

working_dir = os.getcwd()
project_name = get_project_name(working_dir)

unreal_source_dir = os.path.join(working_dir, "Source")
unreal_uproject_fpath = find_file_with_ext(working_dir, ".uproject")
unreal_uproject_path = os.path.join(working_dir, unreal_uproject_fpath)
unreal_project_buildcs_fpath = os.path.join(unreal_source_dir, project_name, (project_name+".Build.cs"))

unreal_binaries_dir = os.path.join(working_dir, "Binaries")
unreal_intermediate_dir = os.path.join(working_dir, "Intermediate")
unreal_sln_fpath = get_project_solution(working_dir, ".sln")
unreal_sln_path = os.path.join(working_dir, unreal_sln_fpath)

engine_version = find_engine_version(unreal_uproject_fpath)
engine_path = find_engine_path(engine_version)
engine_build_tool_fpath = os.path.join(engine_path, "Engine", "Binaries", "DotNET", "UnrealBuildTool", "UnrealBuildTool.exe")
engine_batch_files_path = os.path.join(engine_path, "Engine", "Build", "BatchFiles")