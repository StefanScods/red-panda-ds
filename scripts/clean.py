# ==================================================================================================
# CLeans (deletes) the build directory.
#
# Usage:
#     python ./scripts/clean.py
# ==================================================================================================
import shutil
from defines import BUILD_DIR


if __name__ == "__main__":
    if BUILD_DIR.exists():
        print("Cleaning build...")
        shutil.rmtree(BUILD_DIR)
