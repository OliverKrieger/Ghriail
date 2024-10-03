# Ghriail Scripts

## Script Configurations

### Script runner:

To run helper scripts, enter `python .\script_runner.py`. Tested on python version `3.12.6`

The script runner is at the top of the root to make sure that the os.getcwd() would return the correct working directory automatically.

### Pathing

All project related paths will be setup automatically.

However, `paths_external` is for the user to configure for the right locations.

### MSBuild

For the builds to work, you must have the appropriate `.NET SDK version` installed, as well as have access to `nuget packages` so they can be restored.

For .NET and Unreal Engine 5.4 the recommended version currently is .NET 6.x and could be found here:
https://dotnet.microsoft.com/en-us/download/dotnet/6.0

For NuGet, if visual studio is in `offline mode` you need to either have nuget packages configured offline, or enable `online package restore`. To do the latter, as of 3/10/24, you can do the following:
- In Visual Studio go to **Tools > Options > NuGet Package Manager > Package Sources**
- Make sure there is an active online source like `https://api.nuget.org/v3/index.json`
- If it is not there, click on the small `+` icon
    - In the "Name" field add **NuGet Official**
    - In the "Source" field add the official NuGet Package source `https://api.nuget.org/v3/index.json`
- Make sure the checkbox next to `new online source` is checked.
- In the **Solution Explorer** right click the solution and select **Restore NuGet Packages**. This will download any missing packages.
- To verify, you can go **Tools > NuGet Package Manager > Manage NuGetPackages for Solution** and check that the **Browse** tab has packages from online sources 