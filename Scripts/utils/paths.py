import os
from .utils import find_file_with_ext

working_dir = os.getcwd()

unreal_source_dir = os.path.join(working_dir, "Source")
unreal_uproject_fpath = find_file_with_ext(working_dir, ".uproject")