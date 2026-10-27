import os
import sys
import subprocess
from subprocess import Popen, PIPE
import time
import glob
from multiprocessing import Pool

SUCCEEDED = "\033[32msucceeded\033[0m"
FAILED = "\033[31mfailed\033[0m"
SKIPPED = "\033[35mskipped\033[0m"
WARNING = "\033[33mwarnings\033[0m "

build_format = '| {:35} | {:9} | {:6} |'
build_separator = '-' * 59

# ci-arduino naming, fqbn, [family name]
all_boards = [
    ['CH32V00x' , 'ch32-community:ch32v:CH32V00x_EVT' ],
    ['CH32VM00X-CH32V002F4', 'ch32-community:ch32v:CH32VM00X_EVT:pnum=CH32V002F4', 'CH32VM00X'],
    ['CH32VM00X-CH32V006K8', 'ch32-community:ch32v:CH32VM00X_EVT:pnum=CH32V006K8', 'CH32VM00X'],    
    ['CH32L10x' , 'ch32-community:ch32v:CH32L10x_EVT' ],
    ['CH32V10x' , 'ch32-community:ch32v:CH32V10x_EVT' ],
    ['CH32V20x' , 'ch32-community:ch32v:CH32V20x_EVT' ],
    ['CH32V30x' , 'ch32-community:ch32v:CH32V30x_EVT' ],
    ['CH32X035' , 'ch32-community:ch32v:CH32X035_EVT' ],
]


def sketch_is_skipped(variant, sketch):
    name = variant[0]
    family = variant[2] if len(variant) > 2 else name
    skip_names = {family, name}
    sketchdir = os.path.dirname(sketch)

    skip_marker = any(
        os.path.exists(os.path.join(sketchdir, f".{key}.test.skip"))
        for key in skip_names
    )
    only_markers = glob.glob(os.path.join(sketchdir, ".*.test.only"))
    matches_only = any(
        os.path.exists(os.path.join(sketchdir, f".{key}.test.only"))
        for key in skip_names
    )
    if skip_marker or (only_markers and not matches_only):
        return True

    skip_txt = os.path.join(sketchdir, ".skip.txt")
    if os.path.exists(skip_txt):
        with open(skip_txt) as f:
            if any(line.strip() in skip_names for line in f):
                return True
    return False


# return [succeeded, failed, skipped], output
def build_sketch(variant, sketch):
    ret = [0, 0, 0]
    fqbn = variant[1]
    start_time = time.monotonic()

    if sketch_is_skipped(variant, sketch):
        ret[2] = 1
        success = SKIPPED
    else:
        build_result = subprocess.run("arduino-cli compile --warnings all --fqbn {} {}".format(fqbn, sketch),
                                      shell=True, stdout=PIPE, stderr=PIPE)
        # get stderr into a form where warning/error was output to stderr
        if build_result.returncode != 0:
            success = FAILED
            ret[1] = 1
        else:
            ret[0] = 1
            if build_result.stderr:
                success = WARNING
            else:
                success = SUCCEEDED

    build_duration = time.monotonic() - start_time
    output = [build_format.format(
        os.path.basename(sketch), success, '{:5.2f}s'.format(build_duration)
    )]

    if success != SKIPPED:
        # Build failed
        if build_result.returncode != 0:
            output.append(build_result.stdout.decode("utf-8"))

        # Build with warnings
        if build_result.stderr:
            output.append(build_result.stderr.decode("utf-8"))
    return ret, "\n".join(output)


def build_sketch_task(task):
    index, variant, sketch = task
    ret, output = build_sketch(variant, sketch)
    return index, ret, output


# return [succeeded, failed, skipped]
def build_variant(variant):
    print()
    print(build_separator)
    print('| {:^56} |'.format('Board ' + variant[0]))
    print(build_separator)
    print(build_format.format('Example', '\033[39mResult\033[0m', 'Time'))
    print(build_separator)

    if not all_examples:
        return [0, 0, 0]

    ordered_examples = sorted(
        all_examples,
        key=lambda sketch: (
            not sketch_is_skipped(variant, sketch),
            os.path.basename(sketch).casefold(),
            sketch.casefold(),
        ),
    )
    results = [None] * len(ordered_examples)
    pending_output = {}
    next_to_print = 0
    with Pool(processes=os.cpu_count()) as pool:
        pool_args = (
            (index, variant, sketch)
            for index, sketch in enumerate(ordered_examples)
        )
        for index, ret, output in pool.imap_unordered(
            build_sketch_task, pool_args, chunksize=1
        ):
            results[index] = ret
            pending_output[index] = output
            while next_to_print in pending_output:
                print(pending_output.pop(next_to_print), flush=True)
                next_to_print += 1

    # Sum each result column (succeeded, failed, skipped).
    return list(map(sum, zip(*results)))

if __name__ == '__main__':
    # Resolve all sketch paths from the repository, even when invoked elsewhere.
    os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    # build all variants if input not existed
    if len(sys.argv) > 1:
        build_boards = list(filter(lambda x: sys.argv[1] in x[0], all_boards))
    else:
        build_boards = all_boards

    all_examples = []
    for library_dir in ('libraries', os.path.join('ci', 'libraries')):
        if os.path.isdir(library_dir):
            all_examples.extend(
                sketch for sketch in glob.iglob(
                    os.path.join(library_dir, '**', '*.ino'), recursive=True
                )
                if (
                    os.path.splitext(os.path.basename(sketch))[0]
                    == os.path.basename(os.path.dirname(sketch))
                )
            )
    all_examples.sort(key=lambda sketch: (os.path.basename(sketch).casefold(), sketch.casefold()))

    total_time = time.monotonic()
    total_result = [0, 0, 0]
    for b in build_boards:
        ret = build_variant(b)
        total_result = list(map(lambda x, y: x + y, total_result, ret))

    # Build Summary
    total_time = time.monotonic() - total_time
    print(build_separator)
    print(F"Build Summary: {total_result[0]} {SUCCEEDED}, {total_result[1]} {FAILED}, {total_result[2]} {SKIPPED} and took {total_time:.2f}s")
    print(build_separator)
    sys.exit(total_result[1])
