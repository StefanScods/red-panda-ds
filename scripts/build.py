# ==================================================================================================
# The main build script for the project.
#
# Usage:
#     python ./scripts/build.py BuildType
# ==================================================================================================
from pathlib import Path
import shutil
import sys
from defines import BUILD_DIR, BUILD_TYPES, OUTPUT_BINARY
from utils import checkIfToolExists, runCommand, printStyle, ANSI_CODES
import os

cmakeCache = Path("build/CMakeCache.txt")

def determineIfNeedToRunCMakeConfigure(type: str) -> bool:
    """Check if CMake configure needs to be ran."""
    # Check if the cache exits.
    if not cmakeCache.exists():
        return True
    
    # Read in the CMake Cache.
    cmake_cache = {}
    with open(cmakeCache.as_posix(), "r") as file:
        for line in file:
            # Skip comments and empty lines.
            if line.startswith("#") or not line.strip():
                continue

            # Parse actual data.
            if "=" in line:
                key_value = line.split("=")
                key = key_value[0].strip()
                value = "=".join(key_value[1:]).strip()
                cmake_cache[key] = value
    
    # Check the build type.
    if "CMAKE_BUILD_TYPE:STRING" in cmake_cache:
        cached_type = cmake_cache["CMAKE_BUILD_TYPE:STRING"]
    elif "CMAKE_BUILD_TYPE:UNINITIALIZED" in cmake_cache:
        # The cached type is not used in this case.
        return False
    else:
        printStyle(f"Cannot parse {cmakeCache.as_posix()}", style=ANSI_CODES.BOLD)
        return True
    if cached_type != type:
        return True
    return False


if __name__ == "__main__":
    # Validate input.
    if len(sys.argv) != 2:
        printStyle("Error: Invalid Usage", style=ANSI_CODES.RED + ANSI_CODES.BOLD)
        print("Usage:")
        print(f"\t python {__file__} BuildType")
        print("Supported BuildTypes:\n\t" + "\n\t".join(BUILD_TYPES))
        print("\nExample:")
        print(f"\t python {__file__} {BUILD_TYPES[0]}")
        exit(1)
    buildType = sys.argv[1]
    if not buildType in BUILD_TYPES:
        printStyle("Error: Invalid BuildType", style=ANSI_CODES.RED + ANSI_CODES.BOLD)
        print("Supported BuildTypes:\n\t" + "\n\t".join(BUILD_TYPES))
        exit(1)
    
    # Check if required tools exist.
    checkIfToolExists("git")

    # Pull submodules if not already pulled.
    output = runCommand("git submodule init")
    if output.returncode:
        printStyle("Error: Failed to init submodules", style=ANSI_CODES.RED)
        exit(1)

    # Additional generator logic.
    additionalArgs = ""
    # Fetch location of gcc and g++.
    checkIfToolExists("gcc.exe", installerName="msys64")
    gccPath = shutil.which("gcc.exe")
    checkIfToolExists("g++.exe", installerName="msys64")
    gppPath = shutil.which("g++.exe")
    additionalArgs = f"-DCMAKE_C_COMPILER={gccPath} -DCMAKE_CXX_COMPILER={gppPath}"

    # Configure cmake.
    if determineIfNeedToRunCMakeConfigure(buildType):
        print(f"Found gcc: {gccPath}")
        print(f"Found g++: {gppPath}")

        # Create the build directory.
        if not BUILD_DIR.exists():
            os.makedirs(BUILD_DIR)
        # Run CMake Configure.
        print(f"Configuring project...")
        output = runCommand(
            f'cmake -G Ninja .. -DCMAKE_BUILD_TYPE={buildType} {additionalArgs}',
            cwd=BUILD_DIR,
        )
        if output.returncode:
            shutil.move(cmakeCache, cmakeCache.with_stem("failed-cmake-cache"))
            printStyle("Error: Failed to configure cmake!", style=ANSI_CODES.RED)
            exit(1)

    # Build the project.
    print(f"Building project...")
    output = runCommand(
        f"cmake --build . --config {buildType}",
        cwd=BUILD_DIR,
    )
    if output.returncode:
        printStyle("Error: Failed to build project!", style=ANSI_CODES.RED)
        exit(1)

    # Check that the binary exists.
    binaryPath = BUILD_DIR.joinpath(buildType, OUTPUT_BINARY).absolute()
    if not binaryPath.exists():
        printStyle(
            f"Error: Could not find binary at {binaryPath.as_posix()}!",
            style=ANSI_CODES.RED,
        )
        exit(1)
    printStyle(f"Successfully built project!", style=ANSI_CODES.BOLD + ANSI_CODES.GREEN)
    print(f"\tBinary found at: {binaryPath.as_posix()}")
    print(f"\tBinary size: {os.path.getsize(binaryPath)} bytes")
