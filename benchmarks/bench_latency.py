#!/usr/bin/env python3
import os, subprocess, time, statistics, argparse, re

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--monitor-cmd", default=os.environ.get("MONITOR_CMD", "./build/log-monitor a.log b.log --bench-stamp key1"))
    ap.add_argument("--lines", type=int, default=10000)
    ap.add_argument("--rps", type=float, default=2000)
    ap.add_argument("--warmup", type=float, default=0.3)
    ap.add_argument("--poll-ms", type=float, default=10.0)
    ap.add_argument("--require-stamp", action="store_true")
    
    args = ap.parse_args()

    for p in ["a.log", "b.log"]:
        try: os.remove(p)
        except FileNotFoundError: pass

    open("a.log", "a").close()

    mon = subprocess.Popen(args.monitor_cmd, shell=True)
    time.sleep(args.warmup)

    writer = subprocess.Popen(
        f"python3 benchmarks/writer.py --path a.log --lines {args.lines} --rps {args.rps} --p-key 0.8 --long-frac 0.0",
        shell=True
    )

    # wait until writer exits, then allow b.log to settle
    writer.wait()
    last_size = -1
    last_growth_time = time.time()
    while True:
        try:
            sz = os.path.getsize("b.out")
        except FileNotFoundError:
            sz = -1
        if sz != last_size:
            last_growth_time = time.time()
            last_size = sz
        if time.time() - last_growth_time > 2.0:
            break
        time.sleep(max(0.0, args.poll_ms / 1000.0))

    try:
        mon.terminate()
    except Exception:
        pass

    print("Run complete.")
    print("Now parse process latency with:")
    print(f"  python3 parse_latency.py --path b.out")

if __name__ == "__main__":
    main()
