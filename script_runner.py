# Script runner always has to be on top level so that
# the project working directory is recognised correctly
# (does so as the working directory is set from where the
# entry script is ran from).

# This ensures paths will be configured correctly automatically

from Scripts.Module_Manager.create_module import request_module_create
from Scripts.Module_Manager.remove_module import request_module_remove
from Scripts.UBT_Manager.regenerate_project_files import request_regenerate_project_files, request_regenerate_and_rebuild_project_files

def select_option():
    while(True):
        print("\n1 - create new module")
        print("2 - remove existing module")
        print("3 - regenerate project files")
        print("4 - regenerate and rebuild project files")
        print("-1 - exit")
        try:
            selected_option = int(input("Selected Option:"))
        except ValueError:
            print("WARNING - Given input is not a number!")
            continue

        match selected_option:
            case 1:
                request_module_create()
            case 2:
                request_module_remove()
            case 3:
                request_regenerate_project_files()
            case 4:
                request_regenerate_and_rebuild_project_files()
            case -1:
                break
            case _:
                print("WARNING - No option with selection ", select_option)
        
select_option()