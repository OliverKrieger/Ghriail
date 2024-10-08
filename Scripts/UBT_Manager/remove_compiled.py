import os
import shutil

from Scripts.Utils.paths import unreal_binaries_dir, unreal_intermediate_dir, unreal_sln_path

def remove_compiled():
    if os.path.exists(unreal_intermediate_dir):
        shutil.rmtree(unreal_intermediate_dir)
    else:
        print("WARNING - Intermediate folder does not exist at "+unreal_intermediate_dir)

    if os.path.exists(unreal_binaries_dir):
        shutil.rmtree(unreal_binaries_dir)
    else:
        print("WARNING - Binaries folder does not exist at "+unreal_binaries_dir)

    print(unreal_sln_path)
    if os.path.exists(unreal_sln_path):
        os.remove(unreal_sln_path)
    else:
        print("WARNING - .sln File does not exist at "+unreal_sln_path)