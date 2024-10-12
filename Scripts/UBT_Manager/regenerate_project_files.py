from .remove_compiled import remove_compiled
from .regenerate_solution import regenerate_solution
from .vs_build_project import UBTbuild_project, MSbuild_project
from Scripts.Process_Manager.process_terminate import close_unreal_and_vs

def request_regenerate_project_files():
    print("Closing unreal and visual studio...")
    close_unreal_and_vs()
    print("Beginning regeneration...")
    remove_compiled()
    regenerate_solution()
    print("Project regeneration complete")

def request_regenerate_and_rebuild_project_files():
    print("Closing unreal and visual studio...")
    close_unreal_and_vs()
    print("Beginning regeneration...")
    remove_compiled()
    regenerate_solution()
    # MSbuild_project()
    UBTbuild_project("Rebuild")
    print("Project regeneration complete")