import os
import sys
import subprocess
from subprocess import PIPE
import time
import glob
import json
import re
import shutil
import tempfile
from contextlib import contextmanager
from multiprocessing import Pool

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

SUCCEEDED = "\033[32msucceeded\033[0m"
FAILED = "\033[31mfailed\033[0m"
SKIPPED = "\033[35mskipped\033[0m"
build_format = '| {:35} | {:9} | {:6} |'
build_separator = '-' * 59

# ci-arduino naming, local checkout fqbn, [family name]
all_boards = [
    ['CH32V00x' , 'local:ch32v:CH32V00x_EVT' ],
    ['CH32VM00X-CH32V002F4', 'local:ch32v:CH32VM00X_EVT:pnum=CH32V002F4', 'CH32VM00X'],
    ['CH32VM00X-CH32V006K8', 'local:ch32v:CH32VM00X_EVT:pnum=CH32V006K8', 'CH32VM00X'],
    ['CH32L10x' , 'local:ch32v:CH32L10x_EVT' ],
    ['CH32V10x' , 'local:ch32v:CH32V10x_EVT' ],
    ['CH32V20x' , 'local:ch32v:CH32V20x_EVT' ],
    ['CH32V30x' , 'local:ch32v:CH32V30x_EVT' ],
    ['CH32X035' , 'local:ch32v:CH32X035_EVT' ],
]


@contextmanager
def local_core_build():
    """Expose the checkout as a local platform without changing installed cores."""
    cli = shutil.which('arduino-cli')
    if cli is None:
        raise RuntimeError('arduino-cli was not found on PATH')
    directory_result = subprocess.run(
        [cli, 'config', 'get', 'directories', '--json'],
        check=True, stdout=PIPE, stderr=PIPE, text=True,
    )
    directories = json.loads(directory_result.stdout)

    # Local platforms have no Boards Manager tool dependencies. Resolve the
    # compiler required by this checkout from the existing tool installation.
    with open(os.path.join(REPO_ROOT, 'platform.txt'), encoding='utf-8') as f:
        toolchain = re.search(
            r'^toolchain_dir=\{runtime\.tools\.(.+)-([0-9][^{}]*)\.path\}\s*$',
            f.read(), re.MULTILINE,
        )
    if toolchain is None:
        raise RuntimeError('Cannot determine the compiler dependency from platform.txt')
    tool_name, tool_version = toolchain.groups()
    compiler_dir = os.path.join(
        directories['data'], 'packages', 'ch32-community', 'tools',
        tool_name, tool_version,
    )
    if not os.path.isdir(compiler_dir):
        raise RuntimeError(
            f'Missing compiler: {compiler_dir}. Install ch32-community:ch32v '
            'with arduino-cli core install to provide the compiler tools.'
        )

    compile_options = [
        '--build-property', f'toolchain_dir={compiler_dir}',
        '--libraries', os.path.join(REPO_ROOT, 'libraries'),
    ]
    user_libraries = os.path.join(directories['user'], 'libraries')
    if os.path.isdir(user_libraries):
        compile_options.extend(['--libraries', user_libraries])

    with tempfile.TemporaryDirectory(prefix='ch32-local-core-') as sketchbook:
        hardware = os.path.join(sketchbook, 'hardware', 'local')
        os.makedirs(hardware)
        core_link = os.path.join(hardware, 'ch32v')
        try:
            os.symlink(REPO_ROOT, core_link, target_is_directory=True)
        except OSError:
            if os.name != 'nt':
                raise
            # Junctions work without Windows developer mode/admin privileges.
            import _winapi
            _winapi.CreateJunction(REPO_ROOT, core_link)
        try:
            yield {
                'cli': cli,
                'sketchbook': sketchbook,
                'compile_options': compile_options,
            }
        finally:
            # Remove only the link before TemporaryDirectory cleans its own tree.
            if os.path.islink(core_link):
                os.unlink(core_link)
            else:
                os.rmdir(core_link)


def sketch_is_skipped(variant, sketch):
    name = variant[0]
    family = variant[2] if len(variant) > 2 else name


# return [succeeded, failed, skipped], output
def build_sketch(variant, sketch, local_build):
    ret = [0, 0, 0]
    fqbn = variant[1]
    start_time = time.monotonic()
        ret[2] = 1
        success = SKIPPED
    else:
        build_env = os.environ.copy()
        build_env['ARDUINO_DIRECTORIES_USER'] = local_build['sketchbook']
        build_result = subprocess.run(
            [local_build['cli'], 'compile', '--warnings', 'all', '--fqbn', fqbn,
             *local_build['compile_options'], os.path.abspath(sketch)],
            env=build_env, stdout=PIPE, stderr=PIPE,
        )
        # get stderr into a form where warning/error was output to stderr
        if build_result.returncode != 0:
            success = FAILED


def build_sketch_task(task):
    index, variant, sketch, local_build = task
    ret, output = build_sketch(variant, sketch, local_build)
    return index, ret, output


# return [succeeded, failed, skipped]
def build_variant(variant, local_build):
    print()
    print(build_separator)
    print('| {:^56} |'.format('Board ' + variant[0]))
    next_to_print = 0
    with Pool(processes=os.cpu_count()) as pool:
        pool_args = (
            (index, variant, sketch, local_build)
            for index, sketch in enumerate(ordered_examples)
        )
        for index, ret, output in pool.imap_unordered(

if __name__ == '__main__':
    # Resolve all sketch paths from the repository, even when invoked elsewhere.
    os.chdir(REPO_ROOT)

    # build all variants if input not existed
    if len(sys.argv) > 1:

    total_time = time.monotonic()
    total_result = [0, 0, 0]
    with local_core_build() as local_build:
        print(f'Building against local core: {REPO_ROOT}', flush=True)
        for b in build_boards:
            ret = build_variant(b, local_build)
            total_result = list(map(lambda x, y: x + y, total_result, ret))

    # Build Summary
    total_time = time.monotonic() - total_time