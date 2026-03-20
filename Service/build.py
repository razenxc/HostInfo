import shutil
import subprocess
import platform
from pathlib import Path

root = Path.cwd()
build_dir = root / "build"

if build_dir.exists():
    shutil.rmtree(build_dir) 
build_dir.mkdir()

subprocess.run([
    "cmake",
    "-S", str(root),
    "-B", str(build_dir)
], check=True)

subprocess.run([
    "cmake",
    "--build", str(build_dir),
    "--config", "Release"
], check=True)