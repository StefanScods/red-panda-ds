# ==================================================================================================
# Constants to be used within python helper scripts.
#
# Usage:
#     None
# ==================================================================================================
from pathlib import Path

# The build directory of the repository
BUILD_DIR = Path("build")
# The name of the generated binary.
OUTPUT_BINARY = "redPandaDS.exe"
# Directories which contain source files
SRC_DIRS = ["src/", "test/"]
# File extensions of C/C++ source files
SRC_FILE_EXTENSIONS = [".cpp", ".h", ".hpp"]
# Supported build types.
BUILD_TYPES = ["Release", "Debug"]
# Supported build generators.
BUILD_GENERATORS = ["Ninja", "MSVC"]
