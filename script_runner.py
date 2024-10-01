from Scripts.creation.create_module import request_module_create
from Scripts.deletion.remove_module import request_module_remove

def select_option():
    print("1 - create new module")
    print("2 - remove existing module")
    selected_option = int(input("Selected Option:"))

    match selected_option:
        case 1:
            request_module_create()
        case 2:
            request_module_remove()
        case _:
            print("WARNING - No option with selection ", select_option)

select_option()