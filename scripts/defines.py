# ==================================================================================================
# Constants to be used within python helper scripts.
#
# Usage:
#     None
# ==================================================================================================
from pathlib import Path
import sys

# Exe Suffix for windows / linux.
EXE_SUFFIX = ".exe" if sys.platform == "win32" else ""
# The build directory of the repository
BUILD_DIR = Path("build")
# The name of the generated binary.
OUTPUT_BINARY = "redPandaDS" + EXE_SUFFIX
# Directories which contain source files
SRC_DIRS = ["common/", "frontends/", "core/", "test/",]
# File extensions of C/C++ source files
SRC_FILE_EXTENSIONS = [".cpp", ".h", ".hpp"]
# Supported build types.
BUILD_TYPES = ["Release", "Debug"]
