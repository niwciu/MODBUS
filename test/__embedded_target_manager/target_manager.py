#!/usr/bin/env python3
from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import webbrowser
from typing import Any, Callable, Dict, Iterable, List, Optional

import yaml


ANSI_RESET = "\033[0m"
ANSI_BOLD = "\033[1m"
ANSI_DIM = "\033[2m"
ANSI_RED = "\033[31m"
ANSI_GREEN = "\033[32m"
ANSI_YELLOW = "\033[33m"
ANSI_CYAN = "\033[36m"

ANSI_RE = re.compile(r"\x1b\[[0-9;]*m")

ProgressCallback = Callable[[str, str, str], None]


def supports_ansi() -> bool:
    if not sys.stdout.isatty():
        return False
    if os.environ.get("NO_COLOR"):
        return False
    return True


def strip_ansi(text: str) -> str:
    return ANSI_RE.sub("", text)


def colorize(text: str, color: str) -> str:
    if supports_ansi():
        return f"{color}{text}{ANSI_RESET}"
    return text


def bold(text: str) -> str:
    if supports_ansi():
        return f"{ANSI_BOLD}{text}{ANSI_RESET}"
    return text


def clear_line() -> None:
    if supports_ansi():
        sys.stdout.write("\r\033[2K")
        sys.stdout.flush()


def print_inline_progress(line_text: str) -> None:
    if supports_ansi():
        clear_line()
        sys.stdout.write("\r" + line_text)
        sys.stdout.flush()
    else:
        print(line_text)


class TableProgress:
    """
    Renders a live-updating ASCII table (modules x targets) in the terminal.
    Requires a TTY with ANSI support for live updates and colors.
    """

    def __init__(self, modules, all_targets, config_label, use_color=True, missing_symbol="-"):
        self.modules = modules
        self.targets = all_targets
        self.use_color = use_color and supports_ansi()
        self.config_label = config_label
        self.missing_symbol = missing_symbol

        self.module_col_w = max(len("MODULE"), max((len(m) for m in modules), default=6)) + 2
        self.target_col_w = {t: max(len(t), 4) + 2 for t in all_targets}

        self.status = {(m, t): "" for m in modules for t in all_targets}
        self._module_targets = {m: set() for m in modules}

    def mark_target_set_for_module(self, module, targets_for_module):
        self._module_targets[module] = set(targets_for_module)

    def _cell(self, text, width):
        visible_len = len(strip_ansi(text))
        if visible_len >= width:
            return text
        pad = width - visible_len
        left = pad // 2
        right = pad - left
        return (" " * left) + text + (" " * right)

    def _table_inner_width(self):
        return self.module_col_w + sum(1 + self.target_col_w[t] for t in self.targets)

    def _format_top_title_row(self):
        title = f"config: {self.config_label}"
        return "│" + self._cell(title, self._table_inner_width()) + "│"

    def _format_border_full(self, left, right, fill="─"):
        return left + (fill * self._table_inner_width()) + right

    def _format_border_columns(self, left, mid, right, fill="─"):
        line = left + (fill * self.module_col_w)
        for t in self.targets:
            line += mid + (fill * self.target_col_w[t])
        return line + right

    def _format_header(self):
        line = "│" + self._cell("MODULE", self.module_col_w)
        for t in self.targets:
            line += "│" + self._cell(t, self.target_col_w[t])
        return line + "│"

    def _format_row(self, module):
        line = "│" + module.ljust(self.module_col_w)
        for t in self.targets:
            line += "│" + self._cell(self.status[(module, t)], self.target_col_w[t])
        return line + "│"

    def draw(self):
        for m in self.modules:
            for t in self.targets:
                if t not in self._module_targets.get(m, set()):
                    self.status[(m, t)] = self.missing_symbol

        print(self._format_border_full("┌", "┐"))
        print(self._format_top_title_row())
        print(self._format_border_columns("├", "┬", "┤"))
        print(self._format_header())
        print(self._format_border_columns("├", "┼", "┤"))

        for m in self.modules:
            print(self._format_row(m))

        print(self._format_border_columns("└", "┴", "┘"))

    def update(self, module, target, state):
        if state == "running":
            sym = "▶"
            if self.use_color:
                sym = colorize(sym, ANSI_YELLOW)
        elif state == "ok":
            sym = "✔"
            if self.use_color:
                sym = colorize(sym, ANSI_GREEN)
        elif state == "fail":
            sym = "✖"
            if self.use_color:
                sym = colorize(sym, ANSI_RED)
        else:
            sym = str(state)

        self.status[(module, target)] = sym

        if not supports_ansi():
            return

        row_index = self.modules.index(module)
        lines_up = 1 + (len(self.modules) - row_index)

        sys.stdout.write("\0337")
        sys.stdout.write(f"\033[{lines_up}A")
        sys.stdout.write("\r\033[2K")
        sys.stdout.write(self._format_row(module))
        sys.stdout.write("\0338")
        sys.stdout.flush()


class TargetExecutionError(RuntimeError):
    def __init__(self, module_path, target, returncode, cmd):
        super().__init__(f"Target '{target}' failed in '{module_path}' (exit={returncode})")
        self.module_path = module_path
        self.target = target
        self.returncode = returncode
        self.cmd = cmd


def load_config(yaml_file: str) -> Dict[str, Any]:
    with open(yaml_file, "r") as file:
        return yaml.safe_load(file)


def validate_config(config: Dict[str, Any]) -> None:
    if not isinstance(config, dict):
        raise ValueError("YAML root must be a mapping (dict).")

    if "build" not in config:
        raise ValueError("Missing required 'build' section in YAML.")

    build = config["build"]
    if not isinstance(build, dict):
        raise ValueError("'build' section must be a mapping (dict).")

    if "system" not in build:
        raise ValueError("Missing required 'build.system' (make | ninja).")

    if build["system"] not in ("make", "ninja"):
        raise ValueError("build.system must be either 'make' or 'ninja'.")

    if "jobs" in build and not isinstance(build["jobs"], int):
        raise ValueError("build.jobs must be an integer.")

    if "module_paths" not in config or not isinstance(config["module_paths"], list):
        raise ValueError("Missing 'module_paths' section or it is not a list.")

    for module_path in config["module_paths"]:
        if not isinstance(module_path, str) or not module_path.strip():
            raise ValueError("All entries in 'module_paths' must be non-empty strings.")

    if "exclude_modules" in config and config["exclude_modules"] is not None:
        if not isinstance(config["exclude_modules"], list):
            raise ValueError("'exclude_modules' must be a list of module names.")
        for module_name in config["exclude_modules"]:
            if not isinstance(module_name, str) or not module_name.strip():
                raise ValueError("All entries in 'exclude_modules' must be non-empty strings.")

    if "common_targets" not in config or not isinstance(config["common_targets"], list):
        raise ValueError("Missing 'common_targets' or it is not a list.")

    for target in config["common_targets"]:
        if not isinstance(target, str) or not target.strip():
            raise ValueError("All entries in 'common_targets' must be non-empty strings.")

    if "additional_targets" in config and config["additional_targets"] is not None:
        if not isinstance(config["additional_targets"], dict):
            raise ValueError("'additional_targets' must be a mapping: { MODULE: [targets...] }")
        for module_name, targets in config["additional_targets"].items():
            if not isinstance(module_name, str) or not module_name.strip():
                raise ValueError("Keys in 'additional_targets' must be non-empty module names (strings).")
            if not isinstance(targets, list) or any((not isinstance(x, str) or not x.strip()) for x in targets):
                raise ValueError(f"'additional_targets.{module_name}' must be a list of non-empty strings.")

    if "excluded_targets" in config and config["excluded_targets"] is not None:
        if not isinstance(config["excluded_targets"], dict):
            raise ValueError("'excluded_targets' must be a mapping: { MODULE: [targets...] }")
        for module_name, targets in config["excluded_targets"].items():
            if not isinstance(module_name, str) or not module_name.strip():
                raise ValueError("Keys in 'excluded_targets' must be non-empty module names (strings).")
            if not isinstance(targets, list) or any((not isinstance(x, str) or not x.strip()) for x in targets):
                raise ValueError(f"'excluded_targets.{module_name}' must be a list of non-empty strings.")


def create_required_directories(config: Dict[str, Any], verbose: bool = False) -> None:
    base_report_path = os.path.join("..", "..", "reports")
    ccm_path = os.path.join(base_report_path, "CCM")
    ccr_path = os.path.join(base_report_path, "CCR")
    json_all_path = os.path.join(ccr_path, "JSON_ALL")
    ccr_html_out_path = os.path.join(json_all_path, "HTML_OUT")

    os.makedirs(base_report_path, exist_ok=True)
    os.makedirs(ccm_path, exist_ok=True)
    os.makedirs(ccr_path, exist_ok=True)
    os.makedirs(json_all_path, exist_ok=True)
    os.makedirs(ccr_html_out_path, exist_ok=True)

    for module in config.get("modules", []):
        module_name = module["name"] if isinstance(module, dict) else str(module)
        module_path = os.path.join(ccr_path, module_name)
        os.makedirs(module_path, exist_ok=True)

    if verbose:
        print(f"Created/validated report directory structure at: {os.path.abspath(base_report_path)}")


def resolve_module_paths(module_paths: Iterable[str], config_path: str) -> List[str]:
    base_dir = os.path.dirname(os.path.abspath(config_path))
    resolved = []
    for module_path in module_paths:
        expanded = os.path.expanduser(module_path)
        if os.path.isabs(expanded):
            resolved.append(os.path.abspath(expanded))
        else:
            resolved.append(os.path.abspath(os.path.join(base_dir, expanded)))
    return resolved


def discover_modules(
    module_paths: Iterable[str],
    verbose: bool = False,
    exclude_modules: Iterable[str] | None = None,
) -> List[Dict[str, str]]:
    modules: List[Dict[str, str]] = []
    seen_names = set()
    excluded = {name.strip() for name in (exclude_modules or []) if name.strip()}

    for base_path in module_paths:
        if not os.path.isdir(base_path):
            raise ValueError(f"Module path does not exist or is not a directory: {base_path}")

        entries = sorted(os.listdir(base_path))
        for entry in entries:
            module_dir = os.path.join(base_path, entry)
            if not os.path.isdir(module_dir):
                continue

            cmake_file = os.path.join(module_dir, "CMakeLists.txt")
            if not os.path.isfile(cmake_file):
                continue

            if entry in excluded:
                continue

            if entry in seen_names:
                raise ValueError(f"Duplicate module name detected: '{entry}' in {base_path}")

            seen_names.add(entry)
            modules.append({"name": entry, "path": module_dir})

    if not modules:
        raise ValueError("No modules found. Ensure module paths contain subfolders with CMakeLists.txt.")

    if verbose:
        print(f"Discovered {len(modules)} module(s) from module_paths.")

    return modules


def configure_module(
    module_path: str,
    build_system: str,
    reconfigure: bool = False,
    verbose: bool = False,
) -> str:
    out_path = os.path.join(module_path, "out")

    if reconfigure and os.path.isdir(out_path):
        if verbose:
            print(f"[reconfigure] Removing existing 'out' directory in: {module_path}")
        shutil.rmtree(out_path)

    if not os.path.isdir(out_path):
        if verbose:
            print(f"Running CMake for module: {module_path}")

        if build_system == "ninja":
            generator = "Ninja"
        elif build_system == "make":
            generator = "Unix Makefiles"
        else:
            raise ValueError(f"Unknown build system: {build_system}")

        command = ["cmake", "-S", "./", "-B", "out", "-G", generator]

        if verbose:
            subprocess.run(command, cwd=module_path, check=True)
        else:
            subprocess.run(
                command,
                cwd=module_path,
                check=True,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.STDOUT,
            )

    return out_path


def _extract_target_name(line: str) -> Optional[str]:
    stripped = line.strip()
    if not stripped:
        return None

    lowered = stripped.lower()
    if lowered.startswith(("the following", "built with", "targets:", "all primary")):
        return None

    if stripped.startswith("..."):
        stripped = stripped.lstrip(". ").lstrip()

    for prefix in ("*", "-", "+"):
        if stripped.startswith(prefix):
            stripped = stripped[1:].lstrip()

    for sep in (":", " ("):
        if sep in stripped:
            stripped = stripped.split(sep, 1)[0].strip()

    token = stripped.split()[0] if stripped else ""
    if not token:
        return None

    if not re.match(r"^[A-Za-z0-9][A-Za-z0-9_.+/\\-]*$", token):
        return None

    return token


def discover_targets(
    module_path: str,
    build_system: str,
    reconfigure: bool = False,
    verbose: bool = False,
) -> List[str]:
    out_path = configure_module(
        module_path,
        build_system,
        reconfigure=reconfigure,
        verbose=verbose,
    )

    command = ["cmake", "--build", out_path, "--target", "help"]
    result = subprocess.run(command, cwd=module_path, capture_output=True, text=True, check=False)
    output = (result.stdout or "") + (result.stderr or "")

    if result.returncode != 0:
        raise ValueError(f"Failed to discover targets for {module_path}.\n{output}")

    targets = []
    seen = set()
    for line in output.splitlines():
        target = _extract_target_name(line)
        if target and target not in seen:
            seen.add(target)
            targets.append(target)

    return targets


def run_make_targets(
    module_path: str,
    targets: List[str],
    build_system: str,
    build_jobs: Optional[int],
    reconfigure: bool = False,
    keep_going: bool = False,
    verbose: bool = False,
    module_display_name: Optional[str] = None,
    progress_cb: Optional[ProgressCallback] = None,
):
    out_path = configure_module(
        module_path,
        build_system,
        reconfigure=reconfigure,
        verbose=verbose,
    )

    failed_targets = []
    display_name = module_display_name if module_display_name else module_path

    for target in targets:
        if build_system == "make":
            cmd = ["make"]
            if build_jobs:
                cmd.append(f"-j{build_jobs}")
            cmd.append(target)
        elif build_system == "ninja":
            cmd = ["ninja", target]
        else:
            raise ValueError(f"Unknown build system: {build_system}")

        if progress_cb:
            progress_cb(display_name, target, "running")

        try:
            if verbose:
                subprocess.run(cmd, cwd=out_path, check=True)
            else:
                subprocess.run(
                    cmd,
                    cwd=out_path,
                    check=True,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.STDOUT,
                )

            if progress_cb:
                progress_cb(display_name, target, "ok")

        except subprocess.CalledProcessError as exc:
            failed_targets.append(
                {
                    "module_path": module_path,
                    "target": target,
                    "returncode": exc.returncode,
                    "cmd": cmd,
                }
            )

            if progress_cb:
                progress_cb(display_name, target, "fail")

            if verbose:
                print(f"[FAIL] {module_path}: target '{target}' exited with code {exc.returncode}")

            if not keep_going:
                raise TargetExecutionError(module_path, target, exc.returncode, cmd) from None

    return failed_targets


def generate_missing_report_page(report_folder: str, script_name: str, verbose: bool = False) -> None:
    missing_report_path = os.path.join(report_folder, "missing_report.html")
    if not os.path.exists(missing_report_path):
        with open(missing_report_path, "w", encoding="utf-8") as file:
            file.write(
                f"""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
 <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title>Missing Code Complexity Report</title>
  <style>
    body {{
        font-family: Arial, sans-serif;
        text-align: center;
        margin: 0;
        padding: 0;
    }}
    h2 {{
        margin-top: 20px;
    }}
    p {{
        font-size: 16px;
    }}
    footer {{
        margin-top: 20px;
        font-size: 12px;
        color: #555;
    }}
  </style>
 </head>
 <body>
    <h2>Missing Code Complexity Report for This Module</h2>
    <p>
        A Code Complexity Metrics (CCMR) report has not been generated for this module. Please check if the
        <strong>ccmr</strong> target is being executed for this module or if it is properly configured to generate the report.
    </p>
    <footer>
        Generated by {script_name} script configured with config.yaml
    </footer>
 </body>
</html>
"""
            )
        if verbose:
            print(f"Created missing report page: {missing_report_path}")
    else:
        if verbose:
            print(f"Missing report page already exists: {missing_report_path}")


def generate_main_report(
    report_folder: str,
    modules_yaml_file: str,
    script_name: str,
    verbose: bool = False,
    modules: list | None = None,
) -> None:
    if modules is None:
        with open(modules_yaml_file, "r") as yaml_file:
            config_data = yaml.safe_load(yaml_file)
        modules = config_data["modules"]
    main_report_path = os.path.join(report_folder, "index.html")

    with open(main_report_path, "w", encoding="utf-8") as file:
        file.write(
            """<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
 <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title>Project Code Complexity Reports Main Page</title>
  <style>
    body {
        font-family: Arial, sans-serif;
        text-align: center;
        margin: 0;
        padding: 0;
    }
    h2 {
        margin-top: 20px;
    }
    ul {
        list-style-type: none;
        padding: 0;
        display: grid;
        grid-template-columns: repeat(4, 1fr);
        gap: 10px;
        max-width: 80%;
        margin: 0 auto;
    }
    li {
        margin: 10px 0;
    }
    a {
        text-decoration: none;
    }
    .report-button {
        display: inline-block;
        padding: 15px 25px;
        margin: 5px;
        background-color: #007BFF;
        color: white;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        width: 100%;
        text-align: center;
        font-size: 16px;
        box-sizing: border-box;
    }
    .report-button:hover {
        background-color: #0056b3;
    }
    .report-button-missing {
        background-color: #999;
    }
    .report-button-missing:hover {
        background-color: #777;
    }
    footer {
        margin-top: 20px;
        font-size: 12px;
        color: #555;
    }

    @media screen and (max-width: 1000px) {
        ul { grid-template-columns: repeat(3, 1fr); }
    }
    @media screen and (max-width: 600px) {
        ul { grid-template-columns: repeat(2, 1fr); }
    }
    @media screen and (max-width: 400px) {
        ul { grid-template-columns: 1fr; }
    }
  </style>
 </head>
 <body>
    <h2>Project Code Complexity Reports</h2>
    <ul>
"""
        )

        for module in modules:
            module_name = module["name"] if isinstance(module, dict) else str(module)
            report_file = f"{module_name}.html"
            file_path = os.path.join(report_folder, report_file)
            files_in_directory = os.listdir(report_folder)
            matching_files = [fn for fn in files_in_directory if fn.lower() == report_file.lower()]

            if matching_files:
                file_path = os.path.join(report_folder, matching_files[0])
                if verbose:
                    print(f"Found report for module: {module_name} -> {file_path}")
                file.write(
                    f'<li><a href="file://{os.path.abspath(file_path)}"><button class="report-button">{module_name}</button></a></li>\n'
                )
            else:
                if verbose:
                    print(f"Missing report for module: {module_name}")
                missing_report_path = "missing_report.html"
                file.write(
                    f'<li><a href="{missing_report_path}"><button class="report-button report-button-missing">{module_name}</button></a></li>\n'
                )

        file.write(
            f"""    </ul>
    <footer>
        Generated by {script_name} script configured with config.yaml
    </footer>
 </body>
</html>
"""
        )

    if verbose:
        print(f"Wrote main report: {main_report_path}")


def open_html_files_in_default_browser(file_paths: List[str]) -> None:
    for report in file_paths:
        if os.path.exists(report):
            webbrowser.open(f"file://{os.path.abspath(report)}", new=2)


def exit_with_error(message: str, exit_code: int = 2) -> None:
    if supports_ansi():
        print(colorize(bold(message), ANSI_RED))
    else:
        print(message)
    raise SystemExit(exit_code)


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Run module targets from a YAML configuration.",
        formatter_class=argparse.RawTextHelpFormatter,
    )
    parser.add_argument(
        "-c",
        "--config",
        default="config.yaml",
        metavar="FILE",
        help=(
            "Path to YAML configuration file.\n"
            "(default: config.yaml)"
        ),
    )
    parser.add_argument(
        "-r",
        "--reconfigure",
        action="store_true",
        help="Remove existing 'out' directory and re-run CMake.",
    )
    parser.add_argument(
        "-k",
        "--keep-going",
        action="store_true",
        help="Continue executing remaining targets/modules even if a target fails.",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Show full command output and verbose informational messages.",
    )
    parser.add_argument(
        "-m",
        "--modules",
        nargs="+",
        metavar="MODULE",
        help=(
            "Run targets only for selected module(s).\n"
            "Provide one or more module names separated by spaces."
        ),
    )
    parser.add_argument(
        "-t",
        "--targets",
        nargs="+",
        metavar="TARGET",
        help=(
            "Run only selected target(s).\n"
            "Provide one or more target names separated by spaces."
        ),
    )
    return parser


def main(argv: Optional[List[str]] = None) -> None:
    parser = build_argument_parser()
    args = parser.parse_args(argv)

    try:
        config = load_config(args.config)
    except (OSError, yaml.YAMLError) as exc:
        exit_with_error(f"Configuration error: failed to read/parse '{args.config}'.\n{exc}", exit_code=2)

    try:
        validate_config(config)
    except ValueError as exc:
        exit_with_error(f"Configuration error in '{args.config}':\n{exc}", exit_code=2)

    build_cfg = config["build"]
    build_system = build_cfg["system"]

    build_jobs = build_cfg.get("jobs")
    if build_system == "make" and build_jobs is None:
        build_jobs = os.cpu_count()
        if args.verbose:
            print(f"Auto-selected make jobs: -j{build_jobs}")

    resolved_paths = resolve_module_paths(config["module_paths"], args.config)
    excluded_modules = config.get("exclude_modules") or []
    try:
        modules = discover_modules(
            resolved_paths,
            verbose=args.verbose,
            exclude_modules=excluded_modules,
        )
    except ValueError as exc:
        exit_with_error(f"Configuration error in '{args.config}':\n{exc}", exit_code=2)

    common_targets = [target.strip() for target in config.get("common_targets", [])]
    additional_targets = config.get("additional_targets") or {}
    excluded_targets = config.get("excluded_targets") or {}

    for module in modules:
        module_targets = discover_targets(
            module["path"],
            build_system,
            reconfigure=args.reconfigure,
            verbose=args.verbose,
        )
        available_set = set(module_targets)
        excluded = set(excluded_targets.get(module["name"], []) or [])
        additional = list(additional_targets.get(module["name"], []) or [])

        expected = [target for target in common_targets if target not in excluded]
        for target in additional:
            if target not in expected:
                expected.append(target)

        module["targets"] = expected
        module["available_targets"] = [target for target in expected if target in available_set]

    config["modules"] = modules

    create_required_directories(config, verbose=args.verbose)

    reports_to_open = []
    reports_to_show = config.get("reports_to_show", [])

    for report in reports_to_show:
        if report.lower() == "ccm":
            reports_to_open.append("../../reports/CCM/index.html")
        elif report.lower() == "ccr":
            reports_to_open.append("../../reports/CCR/JSON_ALL/HTML_OUT/project_coverage.html")
        else:
            if os.path.exists(report):
                reports_to_open.append(report)
            else:
                if args.verbose:
                    print(f"Report path not found: {report}")

    all_failed_targets = []

    modules_by_name = {m["name"]: m for m in config["modules"]}
    missing_selected_modules = []

    if args.modules:
        requested = args.modules
        missing_selected_modules = [name for name in requested if name not in modules_by_name]
        modules_to_run = [modules_by_name[name] for name in requested if name in modules_by_name]
    else:
        modules_to_run = config["modules"]

    missing_selected_targets = []
    if args.targets:
        requested_targets = args.targets

        all_known_targets = set()
        for module in config["modules"]:
            for target in module.get("targets", []):
                all_known_targets.add(target)

        missing_selected_targets = [target for target in requested_targets if target not in all_known_targets]
        requested_set = set([target for target in requested_targets if target in all_known_targets])

        for module in modules_to_run:
            original = module.get("targets", [])
            module["targets"] = [target for target in original if target in requested_set]
            available = module.get("available_targets", [])
            module["available_targets"] = [target for target in available if target in requested_set]

    progress = None
    if not args.verbose and supports_ansi():
        module_names = [module["name"] for module in modules_to_run]
        all_targets = []
        seen = set()
        for module in modules_to_run:
            for target in module["targets"]:
                if target not in seen:
                    seen.add(target)
                    all_targets.append(target)

        progress = TableProgress(
            module_names,
            all_targets,
            config_label=args.config,
            use_color=True,
            missing_symbol="-",
        )

        for module in modules_to_run:
            progress.mark_target_set_for_module(module["name"], module.get("available_targets", []))

        progress.draw()

    try:
        for module in modules_to_run:
            module_path = module["path"]
            if args.verbose:
                print(f"Module: {module_path}")

            if not module.get("available_targets"):
                continue

            failed_targets = run_make_targets(
                module_path,
                module["available_targets"],
                build_system,
                build_jobs,
                reconfigure=args.reconfigure,
                keep_going=args.keep_going,
                verbose=args.verbose,
                module_display_name=module["name"],
                progress_cb=(progress.update if progress else None),
            )
            all_failed_targets.extend(failed_targets)

    except TargetExecutionError as exc:
        msg = (
            "Execution stopped because a target failed and '--keep-going' was not set.\n"
            f"Failed target:\n"
            f"  module : {os.path.basename(exc.module_path.rstrip(os.sep))}\n"
            f"  target : {exc.target}\n"
            f"  exit   : {exc.returncode}"
        )
        exit_with_error(msg, exit_code=(exc.returncode if isinstance(exc.returncode, int) else 1))

    script_name = os.path.basename(__file__)
    generate_missing_report_page(os.path.join("..", "..", "reports", "CCM"), script_name, verbose=args.verbose)
    generate_main_report(
        os.path.join("..", "..", "reports", "CCM"),
        args.config,
        script_name,
        verbose=args.verbose,
        modules=config["modules"],
    )

    if (not args.modules) and reports_to_open:
        open_html_files_in_default_browser(reports_to_open)

    if missing_selected_modules:
        for name in missing_selected_modules:
            print(f"Module '{name}' was not found in configuration file '{args.config}'.")

    if missing_selected_targets:
        for target in missing_selected_targets:
            print(f"Target '{target}' was not found in configuration file '{args.config}'.")

    if args.verbose and all_failed_targets:
        header = f"========== SUMMARY: FAILED TARGETS ({len(all_failed_targets)}) =========="
        print("\n" + colorize(bold(header), ANSI_RED))
        for idx, item in enumerate(all_failed_targets, start=1):
            line = f"{idx:>3}. module: {item['module_path']} | target: {item['target']} | exit: {item['returncode']}"
            print(colorize(line, ANSI_RED))
        print(colorize(bold("======================================================"), ANSI_RED) + "\n")

    if args.keep_going and all_failed_targets:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
