#!/usr/bin/env python3
"""Bootstrap a PerovSat Zephyr driver repository from this template.

Usage:
  source .venv/bin/activate   # required: use an activated virtual environment
  python setup.py
"""

from __future__ import annotations

import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent

SLUG_RE = re.compile(r"^[a-z][a-z0-9_-]*$")
VENDOR_RE = re.compile(r"^[a-z][a-z0-9_-]*$")
PEROVSAT_GITHUB_ORG = "github.com/PEROVSAT"
SKIP_PRE_COMMIT_PROJECTS = frozenset({"imu-driver", "imu-mock-driver"})
REQUIRED_PYTHON_PACKAGES = ("pre-commit",)

SETUP_SCRIPT = Path(__file__).name
SKIP_PATHS = frozenset({".git", "__pycache__", SETUP_SCRIPT, "README.md"})

USE_COLOR = not os.environ.get("NO_COLOR") and sys.stdout.isatty()

RESET = "\033[0m"
BOLD = "\033[1m"
DIM = "\033[2m"
CYAN = "\033[36m"
YELLOW = "\033[33m"
GREEN = "\033[32m"
BLUE = "\033[34m"
RED = "\033[31m"


def style(text: str, *codes: str) -> str:
    if not USE_COLOR:
        return text
    return "".join(codes) + text + RESET


def step(message: str) -> None:
    print(f"\n{style('==>', BOLD, GREEN)} {style(message, BOLD)}")


def note(message: str) -> None:
    for line in message.splitlines():
        print(f"  {style(line, DIM, BLUE)}")


def info(message: str) -> None:
    print(f"  {style(message, DIM)}")


def success(message: str) -> None:
    print(f"  {style(message, GREEN)}")


def warn(message: str) -> None:
    print(f"  {style(message, RED)}")


def in_virtualenv() -> bool:
    """Return True when running inside a venv, virtualenv, or similar isolated env."""
    if os.environ.get("VIRTUAL_ENV"):
        return True
    if getattr(sys, "real_prefix", None) is not None:
        return True
    return sys.prefix != sys.base_prefix


def require_virtualenv() -> None:
    if in_virtualenv():
        return

    print(style("Error: setup.py must be run inside an activated virtual environment.", RED),
          file=sys.stderr)
    note(
        "This script installs pre-commit and other dependencies with pip. Running outside "
        "a venv can fail partway through after files have already been renamed.\n\n"
        "Activate your environment first, then rerun:\n"
        "  source .venv/bin/activate   # or your perovsat-app venv\n"
        "  python setup.py"
    )
    sys.exit(1)


def show_prompt_help(name: str, description: str | None = None,
                     examples: str | None = None) -> None:
    print(f"  {style(name, BOLD, CYAN)}")
    if description:
        print(f"  {style(description, DIM)}")
    if examples:
        print(f"  {style(f'e.g. {examples}', YELLOW)}")


def prompt(name: str, default: str | None = None, description: str | None = None,
           examples: str | None = None, validator=None) -> str:
    show_prompt_help(name, description, examples)
    while True:
        suffix = style(f" [{default}]", DIM) if default else ""
        answer = input(f"  {style(name, BOLD, CYAN)}{suffix}: ").strip()
        value = answer or default
        if value is None:
            warn("Value is required.")
            continue
        if validator and not validator(value):
            warn("Invalid value, try again.")
            continue
        print()
        return value


def normalize_compat_part(value: str) -> str:
    return re.sub(r"[^A-Za-z0-9]", "_", value)


def build_tokens(vendor: str, chip: str, module: str) -> dict[str, str]:
    compat = f"{vendor},{chip}"
    driver_upper = normalize_compat_part(chip).upper()
    dt_compat = normalize_compat_part(compat).lower()
    dt_has = f"DT_HAS_{normalize_compat_part(compat).upper()}_ENABLED"
    kconfig_sym = f"PEROVSAT_{driver_upper}"

    return {
        "__MODULE_NAME__": module,
        "__VENDOR__": vendor,
        "__DRIVER_SLUG__": chip,
        "__DRIVER_UPPER__": driver_upper,
        "__COMPAT__": compat,
        "__DT_COMPAT__": dt_compat,
        "__DT_HAS_ENABLED__": dt_has,
        "__KCONFIG_SYM__": kconfig_sym,
    }


def substitute(text: str, tokens: dict[str, str]) -> str:
    for key in sorted(tokens, key=len, reverse=True):
        text = text.replace(key, tokens[key])
    return text


def should_skip(path: Path) -> bool:
    if path.name in SKIP_PATHS:
        return True
    return any(part in SKIP_PATHS for part in path.parts)


def collect_tokenized_files() -> list[Path]:
    files: list[Path] = []
    for path in sorted(ROOT.rglob("*")):
        if not path.is_file():
            continue
        if should_skip(path.relative_to(ROOT)):
            continue
        files.append(path)
    return files


def prune_empty_token_dirs() -> None:
    token_dirs = [p for p in ROOT.rglob("*") if p.is_dir() and "__" in p.name]
    token_dirs.sort(key=lambda p: len(p.parts), reverse=True)
    for directory in token_dirs:
        if directory.is_dir() and not any(directory.iterdir()):
            directory.rmdir()
            print(f"  {style(f'removed empty {directory.relative_to(ROOT)}', DIM)}")


def rename_tokenized_paths(tokens: dict[str, str]) -> None:
    """Rename files whose paths contain tokens (deepest paths first)."""
    files = collect_tokenized_files()
    moves: list[tuple[Path, Path]] = []

    for src in files:
        rel = src.relative_to(ROOT)
        new_rel = Path(substitute(str(rel), tokens))
        if new_rel != rel:
            moves.append((src, ROOT / new_rel))

    moves.sort(key=lambda item: len(item[0].parts), reverse=True)

    for src, dest in moves:
        dest.parent.mkdir(parents=True, exist_ok=True)
        if src.exists():
            src.rename(dest)
            print(f"  {style(str(src.relative_to(ROOT)), DIM)} -> "
                  f"{style(str(dest.relative_to(ROOT)), CYAN)}")

    prune_empty_token_dirs()


def substitute_file_contents(tokens: dict[str, str]) -> int:
    file_count = 0

    for path in collect_tokenized_files():
        rel = path.relative_to(ROOT)
        try:
            original = path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            continue

        updated = substitute(original, tokens)
        if updated != original:
            path.write_text(updated, encoding="utf-8")
            file_count += 1
            print(f"  {style(str(rel), DIM)}")

    return file_count


def finalize_readme() -> None:
    driver_readme = ROOT / "README.driver.md"
    if driver_readme.exists():
        driver_readme.replace(ROOT / "README.md")
        success("Promoted README.driver.md to README.md")


def cleanup_setup() -> None:
    Path(__file__).unlink(missing_ok=True)
    info("Removed setup.py")


def get_git_remotes() -> dict[str, str]:
    git_dir = ROOT / ".git"
    if not git_dir.exists():
        return {}

    result = subprocess.run(
        ["git", "remote", "-v"],
        cwd=ROOT,
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode != 0:
        return {}

    remotes: dict[str, str] = {}
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[2] == "(fetch)":
            remotes[parts[0]] = parts[1]
    return remotes


def restore_git_remotes(remotes: dict[str, str]) -> None:
    for name, url in remotes.items():
        subprocess.run(["git", "remote", "add", name, url], cwd=ROOT, check=True)
        print(f"  {style('Restored remote', DIM)} {style(name, CYAN)}: {style(url, DIM)}")


def remove_git_metadata() -> None:
    git_dir = ROOT / ".git"
    if not git_dir.exists():
        return

    info("Removing existing .git metadata")
    if git_dir.is_file():
        git_dir.unlink()
    else:
        shutil.rmtree(git_dir)


def find_workspace_root() -> Path | None:
    for parent in ROOT.parents:
        if (parent / ".west").is_dir():
            return parent
    return None


def init_fresh_git() -> None:
    git_dir = ROOT / ".git"
    if find_workspace_root() is not None and git_dir.exists():
        success("Preserving existing git repository (west workspace)")
        return

    remotes = get_git_remotes()
    remove_git_metadata()

    subprocess.run(["git", "init"], cwd=ROOT, check=True)
    success("Initialized fresh git repository")

    if remotes:
        info("Restoring git remotes")
        restore_git_remotes(remotes)


def ensure_python_packages(packages: tuple[str, ...] = REQUIRED_PYTHON_PACKAGES) -> None:
    if not packages:
        return

    info(f"Installing: {', '.join(packages)}")
    subprocess.run(
        [sys.executable, "-m", "pip", "install", *packages],
        check=True,
    )


def is_perovsat_west_project(name: str, url: str) -> bool:
    if name in SKIP_PRE_COMMIT_PROJECTS:
        return False
    return name == "manifest" or PEROVSAT_GITHUB_ORG in url


def install_pre_commit_in_repo(repo: Path) -> None:
    if not (repo / ".git").is_dir():
        info(f"Skipping {repo} (not a git repository)")
        return
    if not (repo / ".pre-commit-config.yaml").is_file():
        return

    info(f"Installing pre-commit hooks in {repo}")
    subprocess.run(["pre-commit", "install"], cwd=repo, check=True)


def install_perovsat_pre_commit_hooks() -> None:
    workspace = find_workspace_root()
    if workspace is None:
        install_pre_commit_in_repo(ROOT)
        return

    info(f"Found west workspace at {workspace}")
    result = subprocess.run(
        ["west", "list", "-f", "{name} {path} {url}"],
        cwd=workspace,
        capture_output=True,
        text=True,
        check=True,
    )

    for line in result.stdout.splitlines():
        if not line.strip():
            continue

        name, rel_path, url = line.split(maxsplit=2)
        if not is_perovsat_west_project(name, url):
            continue

        install_pre_commit_in_repo(workspace / rel_path)

    install_pre_commit_in_repo(ROOT)


def remove_legacy_layout() -> None:
    """Remove pre-refactor drivers/ tree if still present in the template."""
    legacy = ROOT / "drivers"
    if not legacy.is_dir():
        return

    shutil.rmtree(legacy)
    info(f"Removed legacy {legacy.relative_to(ROOT)}")


def gather() -> tuple[str, str]:
    note(
        "Name the repository after the physical device, not a logical role like IMU.\n"
        "Map logical roles in perovsat-app."
    )
    print()

    chip = prompt(
        "device-model",
        description="Lowercase slug used in filenames, C symbols, and devicetree compatible",
        examples="mpu6050",
        validator=lambda v: bool(SLUG_RE.match(v)),
    ).lower()
    vendor = prompt(
        "devicetree-vendor",
        description="Vendor prefix in compatible strings (not zephyr)",
        examples="invensense",
        validator=lambda v: bool(VENDOR_RE.match(v)),
    )

    return vendor, chip


def main() -> None:
    require_virtualenv()

    print(style("PerovSat driver template setup", BOLD))
    info(f"Working directory: {ROOT}")

    step("Collecting driver configuration")
    vendor, chip = gather()
    module = f"{chip}-driver"
    print()
    info(f"device-model={style(chip, CYAN)}, vendor={style(vendor, CYAN)}")
    info(f"module={style(module, CYAN)}")
    tokens = build_tokens(vendor, chip, module)

    step("Removing legacy layout")
    remove_legacy_layout()

    step("Substituting tokens in file contents")
    content_count = substitute_file_contents(tokens)
    success(f"Updated {content_count} files")

    step("Renaming tokenized paths")
    rename_tokenized_paths(tokens)

    step("Finalizing README")
    finalize_readme()

    step("Cleaning up setup script")
    cleanup_setup()

    step("Initializing fresh git repository")
    init_fresh_git()

    step("Installing Python dependencies")
    ensure_python_packages()

    step("Installing pre-commit hooks")
    install_perovsat_pre_commit_hooks()

    step("Setup complete")
    print(f"  {style('Module:', BOLD)} {style(tokens['__MODULE_NAME__'], CYAN)}")
    note("Next: wire the new driver into perovsat-app (see README.md)")


if __name__ == "__main__":
    main()
