import questionary
from pathlib import Path

DOT_DIR = Path("./dumps/dot")
APPROPRIATE_EXTENSIONS = [".dot"]

class DotError(Exception):
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)


def get_dot(directory: Path):
    if not directory.exists():
        raise FileNotFoundError(f"dir not found {directory}")

    images = [f for f in directory.iterdir() if f.is_file() and f.suffix.lower() in APPROPRIATE_EXTENSIONS]

    return sorted(images)

def createPng(name):
    pass

choices = get_dot(DOT_DIR)
choices_str = [str(k) for k in choices]
if not choices:
    raise DotError(f"sry, no .dot files in dumps/dot dir yet.")

dot_select = questionary.select("Choose a .dot file you want to generate .png from",
                              choices=choices_str).ask()
for choice in choices:
    if choice.name == dot_select:
        createPng(dot_select)


