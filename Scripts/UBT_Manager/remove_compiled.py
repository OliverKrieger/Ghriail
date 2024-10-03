import os
import shutil

from Scripts.Utils.paths import unreal_binaries_dir, unreal_intermediate_dir, unreal_sln_fpath

def remove_compiled():
    if os.path.exists(unreal_intermediate_dir):
        shutil.rmtree(unreal_intermediate_dir)
    else:
        print("WARNING - Intermediate folder does not exist at "+unreal_intermediate_dir)

    if os.path.exists(unreal_binaries_dir):
        shutil.rmtree(unreal_binaries_dir)
    else:
        print("WARNING - Binaries folder does not exist at "+unreal_binaries_dir)

    if os.path.exists(unreal_sln_fpath):
        shutil.rmtree(unreal_sln_fpath)
    else:
        print("WARNING - .sln File does not exist at "+unreal_sln_fpath)