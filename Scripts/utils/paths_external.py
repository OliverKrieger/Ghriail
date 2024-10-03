import os

# Setup all your engine folders so engine could be found!
engine_folders:list[str] = [
    os.path.join("C:\\", "Program Files", "Epic Games")
]

# Setup your MSbuild.exe location
MSbuild_fpath:str = os.path.join("C:\\", "Program Files", "Microsoft Visual Studio", "2022", "Community", "MSBuild", "Current", "Bin", "MSBuild.exe")