import psutil

# Function to check if a process is running and terminate it
def close_program(process_name):
    for proc in psutil.process_iter(['pid', 'name']):
        if proc.info['name'] and process_name.lower() in proc.info['name'].lower():
            print(f"Closing {process_name} (PID: {proc.info['pid']})")
            proc.terminate()  # Terminate the process
            proc.wait()       # Wait for the process to be terminated

# Check for Unreal Engine and Visual Studio processes
def close_unreal_and_vs():
    # Unreal Engine process names may vary; these are common ones
    unreal_process_names = ['UnrealEditor.exe', 'UE4Editor.exe', 'UE5Editor.exe']
    vs_process_name = 'devenv.exe'  # Visual Studio

    # Close Unreal Engine if running
    for process in unreal_process_names:
        close_program(process)

    # Close Visual Studio if running
    close_program(vs_process_name)