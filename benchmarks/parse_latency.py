import re

STAMP_RE = re.compile(r"#MON_TS=(\d{12,})")

def parse_ts_ns(line):
    m = re.match(r"(\d{12,})", line)
    return int(m.group(1)) if m else None

def parse_mon_ts_ns(line):
    m = STAMP_RE.search(line)
    return int(m.group(1)) if m else None

def main():
    latency_ms = []

    with open("b.log", "r") as f:
        for line in f:
            ts = parse_ts_ns(line)
            mon_ts = parse_mon_ts_ns(line)
            latency_ms.append((mon_ts - ts) / 1e6)

    if not latency_ms:
        print("No process-stamped lines found")
        return

    latency_ms.sort()
    def p(q: float) -> float:
        idx = int(q * (len(latency_ms) - 1))
        return latency_ms[idx]

    print(f"Process latency ms over {len(latency_ms)} lines:")
    print(f"  p50={p(0.50):.2f}  p90={p(0.90):.2f}  p95={p(0.95):.2f}  p99={p(0.99):.2f}")

if __name__ == "__main__":
    main()