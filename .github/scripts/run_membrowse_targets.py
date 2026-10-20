#!/usr/bin/env python3
"""Build and upload every configured MemBrowse target in one CI job."""

import json
import os
import shlex
import subprocess
import sys

from generate_membrowse_targets import CONFIG_PATH, ROOT, generate_targets


def run(command: list[str] | str, *, shell: bool = False) -> int:
    result = subprocess.run(command, cwd=ROOT, shell=shell, check=False)
    return result.returncode


def main() -> int:
    api_key = os.environ.get("MEMBROWSE_API_KEY", "")
    config = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
    targets = generate_targets(config)
    failures = []

    print("Installing Arduino CLI and the CH32 core...")
    if run(config["setup_cmd"], shell=True) != 0:
        return 1

    print("Replacing the installed core with this checkout...")
    install_core = (
        'CORE_DIR="$(arduino-cli config get directories.data)/packages/'
        'ch32-community/hardware/ch32v" && '
        'rm -rf "$CORE_DIR"/* && '
        'mkdir -p "$CORE_DIR/0.0.0" && '
        'rsync -a --exclude=.git --exclude=build ./ "$CORE_DIR/0.0.0/"'
    )
    if run(install_core, shell=True) != 0:
        return 1

    print("Installing MemBrowse CLI...")
    if run([sys.executable, "-m", "pip", "install", "membrowse"]) != 0:
        return 1

    for index, target in enumerate(targets, start=1):
        print(f"\n[{index}/{len(targets)}] {target['target_name']}: building")
        compile_command = target["build_cmd"].split(" && arduino-cli compile ", 1)[1]
        if run(f"arduino-cli compile {compile_command}", shell=True) != 0:
            failures.append((target["target_name"], "build failed"))
            continue

        report_command = [
            "membrowse",
            "-v",
            "INFO",
            "report",
            target["elf"],
            target["ld"],
            "--map-file",
            target["map_file"],
        ]

        if api_key:
            report_command.extend(
                [
                    "--upload",
                    "--github",
                    "--target-name",
                    target["target_name"],
                    "--api-key",
                    api_key,
                ]
            )
            api_url = os.environ.get("MEMBROWSE_API_URL", "").strip()
            if api_url:
                report_command.extend(["--api-url", api_url])

        for definition in shlex.split(target["linker_vars"]):
            report_command.extend(["--def", definition])

        action = "uploading report" if api_key else "analyzing locally (no API key available)"
        print(f"[{index}/{len(targets)}] {target['target_name']}: {action}")
        if run(report_command) != 0:
            failures.append((target["target_name"], "MemBrowse analysis/upload failed"))

    if failures:
        print("\nSome MemBrowse targets failed:", file=sys.stderr)
        for target_name, reason in failures:
            print(f"- {target_name}: {reason}", file=sys.stderr)
        return 1

    result = "built and uploaded" if api_key else "built and analyzed locally"
    print(f"\nSuccessfully {result} {len(targets)} MemBrowse targets.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
