#!/usr/bin/python3
import matplotlib.pyplot as plt
import argparse, os, subprocess, statistics, sys, time

def measure(bench, parser, file, iterations, clear_cache):
    times = []

    prefix = 'Parse time [ms]:'

    for i in range(int(iterations)):
        if clear_cache:
            try:
                command = ['sudo', 'echo', '3', '>', '/proc/sys/vm/drop_caches']
                subprocess.run(command, stdout=subprocess.DEVNULL, check=True)
            except:
                sys.exit()

        if i == 0:
            match parser:
                case 'fast':
                    print('\nParser: fast_obj')
                case 'rapid':
                    print('\nParser: rapidobj')
                case 'tiny':
                    print('\nParser: tinyobjloader')

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
        print(f'Iteration {i+1}: {t}ms')
        time.sleep(2)

    tmin = min(times)
    tavg = statistics.mean(times)
    tstdev = statistics.stdev(times) if i > 0 else 0

    print(f'Minimum: {tmin}ms')
    print(f'Average: {tavg}ms')
    print(f'Standard Deviation: {tstdev}')

    return [tavg, tstdev]

def parse_args():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument("file", help="path to .obj input file")
    arg_parser.add_argument("-b", "--bench", help="path to bench executable", metavar="path")
    arg_parser.add_argument("-o", "--out", help="output path", metavar="path")
    arg_parser.add_argument("-i", "--iterations", help="number of iterations per run", metavar="num")
    arg_parser.add_argument("-c", "--clear-cache", action='store_true', help="clear page cache before each run")
    arg_parser.add_argument("-s", "--style", choices=['default', 'light', 'dark', 'both'], default="default", metavar="choice", help="choice is one of: default, light, dark, both")
    args = arg_parser.parse_args()

    bench = args.bench or '.'
    iterations = args.iterations or 10
    file = args.file
    out = args.out
    clear_cache = args.clear_cache
    style = args.style

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

    return [bench, iterations, file, outfile, clear_cache, style]

def configure_plot(parsers, times, errors, file, style):

    green = [0.18, 0.72, 0.47, 1.0]
    orange = [1.0, 0.5, 0.0, 1.0]
    blue = [0.12, 0.47, 0.71, 1.0]

    plt.rcdefaults()
    plt.style.use(style)

    fig, ax = plt.subplots()

    fig.set_figwidth(6)
    fig.set_figheight(3)

    bars = ax.barh(parsers, times, xerr = errors, align='center', color=[green, orange, blue])

    ax.set_xlabel('time in ms (lower is better)')
    ax.invert_yaxis()
    ax.set_title(file)
    ax.bar_label(bars, padding=5)

def plot(filename, outfile, parsers, times, errors, style):

    [_dir, name] = os.path.split(filename)
    [dir, file] = os.path.split(outfile)
    [file, _ext]  = os.path.splitext(file)

    default = style == 'default'
    light = style == 'light' or style == 'both'
    dark = style == 'dark' or style == 'both'

    if default:
        configure_plot(parsers, times, errors, name, 'bmh')
        print(f'Saving figure {outfile!r}')
        plt.savefig(fname=outfile, transparent=False, bbox_inches='tight')

    if light:
        configure_plot(parsers, times, errors, name, 'default')
        outfile = os.path.join(dir, file + '-light.svg')
        print(f'Saving figure {outfile!r}')
        plt.savefig(fname=outfile, transparent=True, bbox_inches='tight')

    if dark:
        configure_plot(parsers, times, errors, name, 'dark_background')
        outfile = os.path.join(dir, file + '-dark.svg')
        print(f'Saving figure {outfile!r}')
        plt.savefig(fname=outfile, transparent=True, bbox_inches='tight')

def main():

    bench, iterations, file, outfile, clear_cache, style = parse_args()

    if not clear_cache:
        print("Warming page cache...")
        with open(file) as f:
            data = f.readlines()

    fast_min, fast_stdev = measure(bench, 'fast', file, iterations, clear_cache)

    time.sleep(3)

    rapid_min, rapid_stdev = measure(bench, 'rapid', file, iterations, clear_cache)

    time.sleep(3)

    tiny_min, tiny_stdev = measure(bench, 'tiny', file, iterations, clear_cache)

    filename = os.path.basename(file)
    parsers = ('fast_obj', 'rapidobj', 'tinyobjloader')
    times = [fast_min, rapid_min, tiny_min]
    errors = [fast_stdev, rapid_stdev, tiny_stdev]

    plot(filename, outfile, parsers, times, errors, style)

    print('Done')

main()
