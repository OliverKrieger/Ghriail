import shutil
from Scripts.utils.paths import unreal_binaries_dir, unreal_intermediate_dir, unreal_sln_fpath

def remove_compiled():
    shutil.rmtree(unreal_binaries_dir)
    shutil.rmtree(unreal_intermediate_dir)
    shutil.rmtree(unreal_sln_fpath)