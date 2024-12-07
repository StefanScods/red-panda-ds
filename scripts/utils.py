# ==================================================================================================
# Helper python functions for the repo.
#
# Usage:
#     None
# ==================================================================================================
from pathlib import Path
import subprocess
import shutil
import sys


# Based on https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b
class ANSI_CODES:
    ESC = "\033"
    RESET = ESC + "[0m"

    DEFAULT = ESC + "[39m"

    BOLD = ESC + "[1m"
    UNDERLINE = ESC + "[4m"

    RED = ESC + "[31m"
    GREEN = ESC + "[32m"
    YELLOW = ESC + "[33m"
    BLUE = ESC + "[34m"


# ==================================================================================================
def printStyle(*args, style: str = ANSI_CODES.DEFAULT, **kwargs):
    """A wrapper function for print allowing ANSI styling to be applied to the printed output.

    Args:
        style (str, optional): ANSI values to apply. Defaults to ANSI_CODES.DEFAULT.
    """
    print(style + " ".join(map(str, args)) + ANSI_CODES.RESET, **kwargs)


# ==================================================================================================
def runCommand(
    command: str, quiet: bool = False, cwd: str | Path | None = None
) -> subprocess.CompletedProcess[str]:
    """Runs a shell command via a sub process.

    Args:
        command (str): The string of a shell command to execute.
        quiet (bool, optional): A bool used to suppress output from the subprocess. Defaults to
                                False (no output is directed to stdout and stderr).
        cwd (str|Path, optional): The current working directory to run the supplied command.

    Returns:
        subprocess.CompletedProcess[str]: The result of the sub process's execution.
    """
    return subprocess.run(command, capture_output=quiet, text=True, shell=True, cwd=cwd)


# ==================================================================================================
def checkIfToolExists(toolName: str, installerName: str = None) -> None:
    """Checks if a tool is on the PATH env var. Upon failure, the current python process exits
    with a return code of 1.

    Args:
        toolName (str): The binary to check existent
        installerName (str, optional): The tool to install if the above binary could not be found.
                                       If no argument is supplied, the tool name will be repeated.
    """
    if not installerName:
        installerName = toolName

    if shutil.which(toolName) is None:
        printStyle(
            f"Error: {toolName} cannot be found on the PATH. Please install {installerName} "
            "and add to PATH.",
            style=ANSI_CODES.RED,
        )
        exit(1)


# ==================================================================================================
def printProgressBar(iteration, total, barLength=40, barLabel=""):
    """Write a moving progress bar to the console.

    Args:
        iteration (num): Count of progress.
        total (num): Total iteration needed to fill bar.
        barLength (num, optional): How large to print the progress bar.
        barLabel (str, optional): A label to prepend to the progress bar.
    """
    percent = (iteration * 1.0 / total) * 100.0
    filledLength = int(barLength * iteration // total)
    bar = "\u2588" * filledLength + "_" * (barLength - filledLength)
    # Add a space to the end of the label.
    if barLabel:
        barLabel += " "
    # Write the progress bar to stdout.
    sys.stdout.write(
        f"\r{barLabel}|{ANSI_CODES.BLUE}{bar}{ANSI_CODES.RESET}| {percent:.2f}%"
    )
    sys.stdout.flush()
    # Ensure future outputs are on new lines.
    if percent >= 100:
        print("")
