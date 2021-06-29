#!/usr/bin/python3
import matplotlib.pyplot as plt
import argparse, os, subprocess, statistics, sys, time

def measure(bench, parser, file, iterations):
    times = []

    prefix = 'Parse time [ms]:'

    for i in range(1, iterations + 1):
        rc = os.system("sudo echo 3 > /proc/sys/vm/drop_caches")
        if rc != 0:
            sys.exit('failed to clear page cache')
        time.sleep(1)
        result = subprocess.run([bench, '--parser', parser, file], stdout=subprocess.PIPE).stdout.decode('ascii')
        lines = result.split('\n')
        t = None

        for line in lines:
            if line.startswith(prefix):
                line = line[len(prefix):]
                t = int(line)
                break

        if not t:
            sys.exit(f'unexpected output from bench executable: {result!r}')

        times.append(t)
        print(f'Iteration {i}: {t}ms')
        time.sleep(2)

    tmin = min(times)
    tstdev = statistics.stdev(times)

    print(f'Min: {tmin}ms')
    print(f'Standard Deviation: {tstdev}')

    return [tmin, tstdev]

def parse_args():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument("file", help="path to .obj input file")
    arg_parser.add_argument("-b", "--bench", help="path to bench executable", metavar="path")
    arg_parser.add_argument("-o", "--out", help="output path", metavar="path")
    arg_parser.add_argument("-i", "--iterations", help="number of iterations per run", metavar="num")
    args = arg_parser.parse_args()

    bench = args.bench or '.'
    iterations = args.iterations or 10
    file = args.file
    out = args.out

    if os.path.isdir(bench):
        if os.name == 'nt':
            bench = os.path.join(bench, 'bench.exe')
        else:
            bench = os.path.join(bench, 'bench')

    bench = os.path.abspath(bench)

    if not os.path.exists(bench):
        sys.exit(f'bench executable {bench!r} does not exist')

    if not os.path.exists(file):
        sys.exit(f'{file!r} does not exist')

    if not os.path.isfile(file):
        sys.exit(f'{file!r} is not a file')

    outdir = os.path.dirname(file)
    outfile = os.path.splitext(os.path.basename(file))[0] + '.svg'

    if out:
        if os.path.splitext(out)[1]:
            outfile = os.path.basename(out)
            parent = os.path.dirname(out)
            if parent:
                outdir = parent
        else:
            outdir = out

    if not os.path.exists(outdir):
        sys.exit(f'output directory {outdir!r} does not exist')

    outfile = os.path.join(outdir, outfile)

    return [bench, iterations, file, outfile]

def plot(filename, outfile, parsers, times, errors):
    green = [0.18, 0.72, 0.47, 1.0]
    orange = [1.0, 0.5, 0.0, 1.0]
    blue = [0.12, 0.47, 0.71, 1.0]

    plt.rcdefaults()
    _fig, ax = plt.subplots()

    ax.barh(parsers, times, xerr = errors, align='center', color=[green, orange, blue])
    ax.set_xlabel('time in ms (lower is better)')
    ax.invert_yaxis()
    ax.set_title(filename)

    print(f'Saving figure {outfile!r}')

    plt.savefig(fname=outfile, transparent=True, bbox_inches='tight')

def main():

    bench, iterations, file, outfile = parse_args()

    filename = os.path.basename(file)

    print()
    print(f'Parsing {filename!r}')

    print()
    print('Using parser: fast_obj')
    fast_min, fast_stdev = measure(bench, 'fast', file, iterations)

    time.sleep(3)

    print()
    print('Using parser: rapidobj')
    rapid_min, rapid_stdev = measure(bench, 'rapid', file, iterations)

    time.sleep(3)

    print()
    print('Using parser: tinyobjloader')
    tiny_min, tiny_stdev = measure(bench, 'tiny', file, iterations)

    parsers = ('fast_obj', 'rapidobj', 'tinyobjloader')
    times = [fast_min, rapid_min, tiny_min]
    errors = [fast_stdev, rapid_stdev, tiny_stdev]

    plot(filename, outfile, parsers, times, errors)

    print('Done')

main()
