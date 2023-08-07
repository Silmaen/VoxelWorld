#!/usr/bin/env python3
from pathlib import Path
from sys import stderr

dependencies = [
    {"name": "cpptrace", "version": "0.1", "header": False},
    {"name": "debugbreak", "version": "*", "header": True},
    {"name": "entt", "version": "*", "header": True},
    {"name": "magic_enum", "version": "*", "header": True},
    {"name": "stb_image", "version": "*", "header": True},
    {"name": "fmt", "version": "10.0.0", "header": False},
    {"name": "glad", "version": "*", "header": False},
    {"name": "glfw", "version": "*", "header": False},
    {"name": "glm", "version": "*", "header": False},
    {"name": "imgui", "version": "*", "header": False},
    {"name": "imguizmo", "version": "*", "header": False},
    {"name": "shaderc", "version": "*", "header": False},
    {"name": "spdlog", "version": "1.11.0", "header": False},
    {"name": "spirv-cross", "version": "*", "header": False},
    {"name": "yaml-cpp", "version": "*", "header": False},
    {"name": "nfd", "version": "*", "header": False},
    {"name": "googletest", "version": "*", "header": False},
]


class HostInfo:
    def __init__(self):
        self.os = "unknown"
        self.arch = "unknown"
        self.release = ""
        self.libc = ""
        self.__analyse()

    def is_valid(self) -> bool:
        if self.os in [None, "", "unknown"]:
            return False
        if self.arch in [None, "", "unknown"]:
            return False
        return True

    def __analyse(self):
        import platform
        if Path("/etc/os-release").exists():
            rel = platform.freedesktop_os_release()
            self.release = f"{rel['NAME']} {rel['VERSION']}"
            self.libc = " ".join(platform.libc_ver())
        # print(f"linux_distribution     : {platform.linux_distribution()}")
        self.os = platform.system()
        self.arch = platform.machine()
        if self.arch == "AMD64":
            self.arch = "x86_64"


hi = HostInfo()


def has_depmanager():
    import subprocess
    try:
        ret = subprocess.run("depmanager info version", shell=True).returncode
        if ret != 0:
            print(f"Error running depmanager ({ret}).", file=stderr)
            exit(1)
    except Exception as err:
        print(f"Error Exception while running depmanager, {err}.", file=stderr)
        exit(1)
    return True


def check_dependencies(target: dict):
    from depmanager.api.package import PackageManager
    if "os" not in target:
        print("Error: no target os.", file=stderr)
        exit(1)
    if "arch" not in target:
        print("Error: no target arch.", file=stderr)
        exit(1)
    if "kind" not in target:
        print("Error: no target kind.", file=stderr)
        exit(1)

    # loc = LocalManager()
    # pm = PackageManager(system=loc, verbosity=4)
    pm = PackageManager(verbosity=4)

    class Args:
        default = True
        name = ""

    def_name = pm.remote_name(Args())
    if target["kind"] == "both":
        kinds = ["static", "shared"]
    else:
        kinds = [target["kind"]]

    for kind in kinds:
        for dep in dependencies:
            query = {
                "name": dep["name"],
                "version": dep["version"],
                "os": target["os"],
                "arch": target["arch"],
                "kind": "header"
            }
            if not dep["header"]:
                query["kind"] = kind
            # search local
            result = pm.query(query, "")
            if len(result) > 0:  # already in local
                print(f"Dependency '{dep['name']} for {query['kind']}' found locally.")
                continue
            print(f"Dependency '{dep['name']} for {query['kind']}' not found locally.")
            if def_name in ["", None]:
                print(f"Error: No remote defined.", file=stderr)
                exit(1)
            result = pm.query(query, def_name)
            if len(result) == 0:  # Not in remote
                print(f"Error: Unable to find '{dep['name']} for {query['kind']}' on default remote.", file=stderr)
                exit(1)
            pm.add_from_remote(result[0], def_name)
            # loc.clean_tmp()
            # pm = PackageManager(verbosity=4)
            result = pm.query(query, "")
            if len(result) > 0:  # already in local
                print(f"Dependency '{dep['name']} for {query['kind']}' successfully added to local.")
            else:
                print(f"Error: Unable to get '{dep['name']} for {query['kind']}' from default remote.", file=stderr)
                exit(1)


def main():
    from argparse import ArgumentParser as ap
    arg_p = ap()
    arg_p.add_argument("--kind", "-k",
                       type=str,
                       default="both", choices=["both", "static", "shared"])
    arg_p.add_argument("--os", "-o",
                       type=str,
                       default="")
    arg_p.add_argument("--arch", "-a",
                       type=str,
                       default="")
    args = arg_p.parse_args()
    has_depmanager()
    target = {
        "kind": args.kind,
        "os": hi.os,
        "arch": hi.arch
    }
    if args.os not in ["", None]:
        target["os"] = args.os
    if args.arch not in ["", None]:
        target["arch"] = args.arch
    check_dependencies(target)


if __name__ == "__main__":
    main()
