import argparse
import sys

"""

// https://www.argtable.org/docs/arg_installation.html


Usage:
    open <command> [options]

Commands:
    new                Create new project.
    init               Initialize project.

    run                Run project.
    build              Build project.
    clean              Clean build artifacts.

    list               List packages.
    install            Install packages.
    update             Update packages.
    uninstall          Uninstall packages.

    doc                Generate documentation.
    test               Run tests.

    help               Show help for commands.

Options:
    -h, --help         Show this help message.
    -v, --version      Show version information.

=====================================================

build [options]                     Build project in the current directory.
build <input_dir> [options]         Build project from the specified input directory.
build -c <config_file> [options]    Build project using the specified configuration file.
build -o <output_dir> [options]     Build project and output to the specified directory.

=====================================================

run [args]                          Run project with default settings.
run <build_dir> [args]              Run project from the specified build directory.
run -c <config_file> [args]         Run project using the specified configuration file.

=====================================================

clean [options]                     Clean build artifacts in the current directory.
clean <build_dir> [options]         Clean build artifacts from the specified build directory.
clean -c <config_file> [options]    Clean build artifacts using the specified configuration file.

"""


def cmd_new(args):
    print(f"new project")


def cmd_init(args):
    print(f"init project")


def cmd_build(args):
    input_dir = args.input_dir or "."
    output_dir = args.output or "."
    config = args.config or None
    print(f"build: input={input_dir}, output={output_dir}, config={config}")


def cmd_run(args):
    build_dir = args.build_dir or "."
    config = args.config or None
    print(f"run: build_dir={build_dir}, config={config}, args={args.args}")


def cmd_clean(args):
    build_dir = args.build_dir or "."
    config = args.config or None
    print(f"clean: build_dir={build_dir}, config={config}")


def cmd_list(args):
    print("list packages")


def cmd_install(args):
    print(f"install: {args.package}")


def cmd_update(args):
    print(f"update: {args.package or 'all'}")


def cmd_uninstall(args):
    print(f"uninstall: {args.package}")


def cmd_doc(args):
    print("generate documentation")


def cmd_test(args):
    print("run tests")


def main():
    parser = argparse.ArgumentParser(
        prog="open",
        description="Open language toolchain",
    )
    parser.add_argument("-v", "--version", action="version", version="%(prog)s 0.1.0")

    subparsers = parser.add_subparsers(dest="command", metavar="<command>")

    # new
    subparsers.add_parser("new", help="Create new project.")

    # init
    subparsers.add_parser("init", help="Initialize project.")

    # build
    p_build = subparsers.add_parser("build", help="Build project.")
    p_build.add_argument(
        "input_dir",
        nargs="?",
        default=None,
        metavar="input_dir",
        help="Input directory.",
    )
    p_build.add_argument(
        "-c", dest="config", metavar="config_file", help="Configuration file."
    )
    p_build.add_argument(
        "-o", dest="output", metavar="output_dir", help="Output directory."
    )

    # run
    p_run = subparsers.add_parser("run", help="Run project.")
    p_run.add_argument(
        "build_dir",
        nargs="?",
        default=None,
        metavar="build_dir",
        help="Build directory.",
    )
    p_run.add_argument(
        "-c", dest="config", metavar="config_file", help="Configuration file."
    )
    p_run.add_argument(
        "args", nargs=argparse.REMAINDER, help="Arguments passed to the program."
    )

    # clean
    p_clean = subparsers.add_parser("clean", help="Clean build artifacts.")
    p_clean.add_argument(
        "build_dir",
        nargs="?",
        default=None,
        metavar="build_dir",
        help="Build directory.",
    )
    p_clean.add_argument(
        "-c", dest="config", metavar="config_file", help="Configuration file."
    )

    # list
    subparsers.add_parser("list", help="List packages.")

    # install
    p_install = subparsers.add_parser("install", help="Install packages.")
    p_install.add_argument("package", help="Package to install.")

    # update
    p_update = subparsers.add_parser("update", help="Update packages.")
    p_update.add_argument(
        "package", nargs="?", default=None, help="Package to update (all if omitted)."
    )

    # uninstall
    p_uninstall = subparsers.add_parser("uninstall", help="Uninstall packages.")
    p_uninstall.add_argument("package", help="Package to uninstall.")

    # doc
    subparsers.add_parser("doc", help="Generate documentation.")

    # test
    subparsers.add_parser("test", help="Run tests.")

    args = parser.parse_args()

    commands = {
        "new": cmd_new,
        "init": cmd_init,
        "build": cmd_build,
        "run": cmd_run,
        "clean": cmd_clean,
        "list": cmd_list,
        "install": cmd_install,
        "update": cmd_update,
        "uninstall": cmd_uninstall,
        "doc": cmd_doc,
        "test": cmd_test,
    }

    if args.command in commands:
        commands[args.command](args)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
