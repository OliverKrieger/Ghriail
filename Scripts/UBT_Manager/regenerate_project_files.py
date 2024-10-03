from .remove_compiled import remove_compiled
from Scripts.Utils.paths import unreal_uproject_fpath, engine_path, engine_version

def request_regenerate_project_files():
    print("Beginning regeneration...")
    # remove_compiled()
    print("Engine Version: ", engine_version)
    print("Engine Path: ", engine_path)
    print("Project regeneration complete")