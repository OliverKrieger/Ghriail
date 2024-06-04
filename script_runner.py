from Scripts.creation.create_module import request_module_create

def select_option():
    print("1 - create new module")
    selected_option = input("Selected Option:")

    match selected_option:
        case 1:
            request_module_create()
        case _:
            print("WARNING - No option with selection ", select_option)

select_option()