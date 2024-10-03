from .remove_compiled import remove_compiled
from .regenerate_solution import regenerate_solution
from vs_build_project import build_project


def request_regenerate_project_files():
    print("Beginning regeneration...")
    remove_compiled()
    regenerate_solution()
    build_project()
    print("Project regeneration complete")