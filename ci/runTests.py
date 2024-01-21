#!/usr/bin/env python3
from argparse import ArgumentParser
from pathlib import Path
from subprocess import run
from sys import stderr

root_dir = Path(__file__).resolve().parent.parent


def runtest(build_path: str):
    build_dir = root_dir / build_path
    if not build_dir.exists():
        print(f"ERROR: no build dir '{build_dir}' found.", file=stderr)
        exit(66)
    list_tests = []
    for file in (build_dir / "bin").iterdir():
        if "unit_test" in file.name:
            list_tests.append(file)
    is_ok = True
    for test in list_tests:
        try:
            report_file = build_dir / "test" / f"{test.stem}_UTest_Report.xml"
            report_file.unlink(missing_ok=True)
            out = run(
                f"{test} --gtest_output=xml:{report_file}",
                cwd=build_dir,
                shell=True,
            )
            if out.returncode != 0:
                is_ok = False
            elif not report_file.exists():
                print(f"ERROR: report file {report_file} not found", file=stderr)
                is_ok = False
        except Exception as err:
            print(
                f"ERROR: exception '{err}' while running test {test.stem}", file=stderr
            )
            is_ok = False
    return is_ok


def main():
    parser = ArgumentParser()
    parser.add_argument(
        "build", type=str, help="The path to the build relative to source"
    )
    args = parser.parse_args()

    if not runtest(args.build):
        exit(1)


if __name__ == "__main__":
    main()
