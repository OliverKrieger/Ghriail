import os
from .utils import find_file_with_ext, get_project_name

working_dir = os.getcwd()
project_name = get_project_name(working_dir)

unreal_source_dir = os.path.join(working_dir, "Source")
unreal_uproject_fpath = find_file_with_ext(working_dir, ".uproject")
unreal_project_buildcs_fpath = os.path.join(unreal_source_dir, project_name, (project_name+".Build.cs"))

unreal_binaries_dir = os.path.join(working_dir, "Binaries")
unreal_intermediate_dir = os.path.join(working_dir, "Intermediate")
unreal_sln_fpath = find_file_with_ext(working_dir, ".sln")