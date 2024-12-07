# ==================================================================================================
# The main build script for the project.
#
# Usage:
#     python ./scripts/build.py BuildGenerator BuildType
# ==================================================================================================
from pathlib import Path
import shutil
from defines import BUILD_DIR, BUILD_GENERATORS, BUILD_TYPES, OUTPUT_BINARY
from utils import checkIfToolExists, runCommand, printStyle, ANSI_CODES
import os
import sys


def askForCleanBuild(force=False):
    """Asks if build can clean the previous build."""
    if force:
        runCommand(f"python scripts/clean.py")
    else:
        proc = input("A clean build is needed to proceed [y/n]: ")
        if proc.lower() == "y" or proc.lower() == "yes":
            runCommand(f"python scripts/clean.py")
        else:
            printStyle("Aborting build...", style=ANSI_CODES.BOLD + ANSI_CODES.RED)
            exit(1)


def determineIfNeedToRunCMakeConfigure(generator: str, type: str) -> bool:
    """Check if CMake configure needs to be ran."""
    # Check if the cache exits.
    cmakeCache = Path("build/CMakeCache.txt")
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

    # Check the generator.
    if "CMAKE_GENERATOR:INTERNAL" in cmake_cache:
        cached_generator = cmake_cache["CMAKE_GENERATOR:INTERNAL"]
    else:
        printStyle(f"Cannot parse {cmakeCache.as_posix()}", style=ANSI_CODES.BOLD)
        askForCleanBuild()
        return True
    if cached_generator != generator:
        # Need to clean the project.
        printStyle("A new generator was selected!", style=ANSI_CODES.BOLD)
        askForCleanBuild()
        return True

    # Check the build type.
    if "CMAKE_BUILD_TYPE:STRING" in cmake_cache:
        cached_type = cmake_cache["CMAKE_BUILD_TYPE:STRING"]
    elif "CMAKE_BUILD_TYPE:UNINITIALIZED" in cmake_cache:
        # The cached type is not used in this case.
        return False
    else:
        printStyle(f"Cannot parse {cmakeCache.as_posix()}", style=ANSI_CODES.BOLD)
        askForCleanBuild()
        return True
    if cached_type != type:
        return True

    return False


if __name__ == "__main__":
    # Check if required tools exist.
    checkIfToolExists("git")

    # Validate input.
    if len(sys.argv) != 3:
        printStyle("Error: Invalid Usage", style=ANSI_CODES.RED + ANSI_CODES.BOLD)
        print("Usage:")
        print(f"\t python {__file__} BuildGenerator BuildType")
        print(f"\nSupported BuildGenerators:\n\t{"\n\t".join(BUILD_GENERATORS)}")
        print(f"Supported BuildTypes:\n\t{"\n\t".join(BUILD_TYPES)}")
        print("\nExample:")
        print(f"\t python {__file__} {BUILD_GENERATORS[0]} {BUILD_TYPES[0]}")
        exit(1)
    buildGenerator = sys.argv[1]
    buildType = sys.argv[2]
    if not buildGenerator in BUILD_GENERATORS:
        printStyle(
            "Error: Invalid BuildGenerator", style=ANSI_CODES.RED + ANSI_CODES.BOLD
        )
        print(f"\nSupported BuildGenerators:\n\t{"\n\t".join(BUILD_GENERATORS)}")
        exit(1)
    if not buildType in BUILD_TYPES:
        printStyle("Error: Invalid BuildType", style=ANSI_CODES.RED + ANSI_CODES.BOLD)
        print(f"\nSupported BuildTypes:\n\t{"\n\t".join(BUILD_TYPES)}")
        exit(1)

    # Pull submodules if not already pulled.
    output = runCommand("git submodule init")
    if output.returncode:
        printStyle("Error: Failed to init submodules", style=ANSI_CODES.RED)
        exit(1)

    # Additional generator logic.
    additionalArgs = ""
    if buildGenerator == "Ninja":
        # Fetch location of gcc and g++.
        checkIfToolExists("gcc.exe", installerName="msys64")
        gccPath = shutil.which("gcc.exe")
        checkIfToolExists("g++.exe", installerName="msys64")
        gppPath = shutil.which("g++.exe")

        additionalArgs = f"-DCMAKE_C_COMPILER={gccPath} -DCMAKE_CXX_COMPILER={gppPath}"

    elif buildGenerator == "MSVC":
        # Convert MSVC into the local name of the compiler. TODO!!! Make this more robust.
        buildGenerator = "Visual Studio 17 2022"

    # Configure cmake.
    if determineIfNeedToRunCMakeConfigure(buildGenerator, buildType):
        # Logging info.
        if buildGenerator == "Ninja":
            print(f"Found gcc: {gccPath}")
            print(f"Found g++: {gppPath}")

        # Create the build directory.
        if not BUILD_DIR.exists():
            os.makedirs(BUILD_DIR)
        # Run CMake Configure.
        print(f"Configuring project...")
        output = runCommand(
            f'cmake -G "{buildGenerator}" .. -DCMAKE_BUILD_TYPE={buildType} {additionalArgs}',
            cwd=BUILD_DIR,
        )
        if output.returncode:
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
