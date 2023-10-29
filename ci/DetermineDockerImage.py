#!/usr/bin env python3
"""
Just determine the docker image based on the json file.
"""
from argparse import ArgumentParser
from json import load
from pathlib import Path

parser = ArgumentParser("simple input.")
parser.add_argument("preset", type=str, help="The preset to check.")
args = parser.parse_args()

curPath = Path(__file__).resolve().parent
with open(curPath / "dockerImages.json") as js:
    data = load(js)

result = "notfound"
if args.preset in data["images"]:
    result = data["images"][args.preset]
print(f"##teamcity[setParameter name='docker_image' value='{result}']")

if result == "notfound":
    exit(1)
exit(0)
