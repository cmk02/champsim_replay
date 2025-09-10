#!/usr/bin/env python3
import argparse
import re
import sys
from collections import defaultdict

def calculate_average_stalls_per_id(path: str) -> dict:
    # Pattern to match all types of rob stalls
    pattern = re.compile(r'^cycles:\s*(?P<cycles>\d+)\s+instr_id:\s*(?P<instr_id>\d+)\s+rob stalled due to (?P<stall_cause>page table walk|non-replay load|replay load)\s*$')    
    
    # Dictionary to count total occurrences for each stall cause
    stall_counts = defaultdict(int)
    # Dictionary to track unique instruction IDs for each stall cause
    unique_ids = defaultdict(set)
    
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            m = pattern.search(line)
            if m:
                instr_id = m.group('instr_id')
                stall_cause = m.group('stall_cause')
                stall_counts[stall_cause] += 1
                unique_ids[stall_cause].add(instr_id)
    
    # Calculate average occurrences per unique ID for each cause
    averages = {}
    for cause in stall_counts:
        if len(unique_ids[cause]) > 0:
            averages[cause] = stall_counts[cause] / len(unique_ids[cause])
    
    return averages

def main():
    parser = argparse.ArgumentParser(description="Calculate average rob stall occurrences per unique instruction ID for each cause (page table walk, non-replay load, replay load).")
    parser.add_argument("path", help="Path to the trace file")
    args = parser.parse_args()

    try:
        averages = calculate_average_stalls_per_id(args.path)
    except FileNotFoundError:
        print(f"Error: file not found: {args.path}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

    if not averages:
        print("No rob stall events found in the trace file.")
        return

    print("Average rob stall occurrences per unique instruction ID by cause:")
    for cause, avg_count in sorted(averages.items()):
        print(f"{cause}: {avg_count:.2f}")

if __name__ == "__main__":
    main()