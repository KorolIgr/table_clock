# type: ignore
Import("env")

import os
import gzip
import shutil
import time
from pathlib import Path

PAGES_DIR = Path("pages")
DATA_DIR = Path("data")


def clean_data_dir():
    if DATA_DIR.exists():
        shutil.rmtree(DATA_DIR)
    DATA_DIR.mkdir(parents=True, exist_ok=True)


def compress_file(src_path: Path, dst_path: Path):
    with open(src_path, "rb") as f_in:
        with gzip.open(dst_path, "wb") as f_out:
            shutil.copyfileobj(f_in, f_out)


def build_littlefs():
    print("\n=== Building LittleFS (gzip) ===")

    if not PAGES_DIR.exists():
        print("Pages directory not found!")
        return

    clean_data_dir()

    for file in PAGES_DIR.rglob("*"):
        if file.is_file():
            rel_path = file.relative_to(PAGES_DIR)
            target_path = DATA_DIR / rel_path

            # создаём структуру папок
            target_path.parent.mkdir(parents=True, exist_ok=True)

            if file.suffix in [".html", ".css", ".js"]:
                gz_path = Path(str(target_path) + ".gz")

                compress_file(file, gz_path)
                print(f"Compressed: {rel_path} -> {gz_path.name}")

            else:
                # копируем как есть (например картинки)
                shutil.copy2(file, target_path)
                print(f"Copied: {rel_path}")


def uploadfs(source, target, env):
    build_littlefs()
    print("\n=== Uploading LittleFS ===")
    time.sleep(2)
    env.Execute("pio run --target uploadfs")


# 🔥 хуки PlatformIO
env.AddPostAction("upload", uploadfs)