# ==================================================================================================
# Runs clang-format recursively on a directory of src files.
#
# Usage:
#     python ./scripts/formatter.py
# ==================================================================================================
import os
import pathlib
from concurrent.futures import ThreadPoolExecutor, as_completed
import time
from utils import (
    ANSI_CODES,
    checkIfToolExists,
    printProgressBar,
    printStyle,
    runCommand,
)
from defines import SRC_DIRS, SRC_FILE_EXTENSIONS


# Checks to see if a file needs to be formatted. If so performs the formatting.
def checkAndFormatFile(file_path):
    # Check if the file needs formatting.
    result = runCommand(f"clang-format --dry-run -Werror {file_path}", quiet=True)

    # Perform the format if file is dirty.
    if result.returncode != 0:
        runCommand(f"clang-format -i {file_path}")
        return True
    return False


# Find all relevant files which may need reformating.
def collectSourceFiles():
    sourceFiles = []
    for directory in SRC_DIRS:
        for root, _, files in os.walk(directory):
            for file in files:
                if any(file.endswith(ext) for ext in SRC_FILE_EXTENSIONS):
                    sourceFiles.append(pathlib.Path(root, file).as_posix())
    return sourceFiles


# Perform reformatting of all source files within parallel.
if __name__ == "__main__":

    # Check if clang-format exists.
    checkIfToolExists(toolName="clang-format", installerName="LLVM")

    startTime = time.time()
    sourceFiles = collectSourceFiles()
    numSourceFiles = len(sourceFiles)
    modified = []
    completedCount = 0

    # Use ThreadPoolExecutor for multithreading
    with ThreadPoolExecutor() as executor:
        # Submit all tasks to the thread pool
        workerThreads = {
            executor.submit(checkAndFormatFile, file): file for file in sourceFiles
        }

        # Process completed tasks
        for task in as_completed(workerThreads):
            try:
                completedCount += 1
                printProgressBar(completedCount, numSourceFiles)
                file_path = workerThreads[task]
                if task.result():
                    modified.append(file_path)

            except Exception as e:
                # Failed to process file. Throw error message.
                print(f"Error: Could not process file {file_path}: {e}")

    # Print output message.
    modifiedCount = len(modified)
    if modifiedCount:
        print(f"Reformated:\n\t{"\n\t".join(modified)}")
    timeElapsed = time.time() - startTime
    print(f"Looked at {len(sourceFiles)} files within {timeElapsed} seconds.")
    if modifiedCount == 0:
        printStyle(
            "All files up-to-date. No formatting required!",
            style=ANSI_CODES.BOLD + ANSI_CODES.GREEN,
        )
    else:
        printStyle(
            f"Reformatted {modifiedCount} file{"s" if modifiedCount > 1 else ""}!",
            style=ANSI_CODES.BOLD + ANSI_CODES.GREEN,
        )
