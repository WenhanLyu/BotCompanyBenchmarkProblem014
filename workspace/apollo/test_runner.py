#!/usr/bin/env python3
"""Comprehensive test runner with timeout and detailed analysis"""

import subprocess
import sys
import os
from pathlib import Path
import signal

def run_with_timeout(cmd, stdin_data, timeout=10):
    """Run command with timeout"""
    try:
        result = subprocess.run(
            cmd,
            input=stdin_data,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        return result.returncode, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return -1, "", "TIMEOUT"
    except Exception as e:
        return -2, "", str(e)

def compare_outputs(actual, expected):
    """Compare two outputs line by line"""
    actual_lines = actual.strip().split('\n')
    expected_lines = expected.strip().split('\n')
    
    if actual_lines == expected_lines:
        return True, None
    
    # Find differences
    diff_info = []
    max_lines = max(len(actual_lines), len(expected_lines))
    for i in range(max_lines):
        actual_line = actual_lines[i] if i < len(actual_lines) else "<missing>"
        expected_line = expected_lines[i] if i < len(expected_lines) else "<missing>"
        if actual_line != expected_line:
            diff_info.append(f"Line {i+1}: Expected '{expected_line}' but got '{actual_line}'")
    
    return False, diff_info

def run_test(test_name, input_file, expected_file, executable, result_dir):
    """Run a single test and return result"""
    result = {
        'name': test_name,
        'status': 'UNKNOWN',
        'details': ''
    }
    
    if not os.path.exists(input_file):
        result['status'] = 'SKIP'
        result['details'] = 'Input file not found'
        return result
    
    # Read input
    with open(input_file, 'r') as f:
        input_data = f.read()
    
    # Run test
    exit_code, stdout, stderr = run_with_timeout([executable], input_data, timeout=10)
    
    # Save actual output
    actual_file = os.path.join(result_dir, f"{test_name}_actual.out")
    with open(actual_file, 'w') as f:
        f.write(stdout)
        if stderr:
            f.write(f"\n--- STDERR ---\n{stderr}")
    
    if exit_code == -1:
        result['status'] = 'TIMEOUT'
        result['details'] = 'Test exceeded 10 second timeout'
        return result
    
    if exit_code == -2:
        result['status'] = 'ERROR'
        result['details'] = f'Failed to run: {stderr}'
        return result
    
    if exit_code != 0:
        result['status'] = 'ERROR'
        result['details'] = f'Exit code {exit_code}: {stderr}'
        return result
    
    # Check expected output
    if not os.path.exists(expected_file):
        result['status'] = 'NO_EXPECTED'
        result['details'] = 'No expected output file available'
        return result
    
    with open(expected_file, 'r') as f:
        expected = f.read()
    
    match, diff = compare_outputs(stdout, expected)
    if match:
        result['status'] = 'PASS'
        result['details'] = 'Output matches expected'
    else:
        result['status'] = 'FAIL'
        result['details'] = '\n'.join(diff[:5]) if diff else 'Output mismatch'
    
    return result

def main():
    # Paths
    base_dir = Path.cwd()
    executable = base_dir / "code"
    basic_dir = base_dir / "testcases" / "basic-testcases"
    bigint_dir = base_dir / "testcases" / "bigint-testcases"
    result_dir = base_dir / "workspace" / "apollo" / "test_results"
    result_dir.mkdir(parents=True, exist_ok=True)
    
    print("=== COMPREHENSIVE TEST EXECUTION ===")
    print(f"Executable: {executable}")
    print(f"Results: {result_dir}")
    print()
    
    all_results = []
    
    # Run basic tests
    print("=== BASIC TESTS (test0-15) ===")
    basic_results = []
    for i in range(16):
        test_name = f"test{i}"
        input_file = basic_dir / f"{test_name}.in"
        expected_file = basic_dir / f"{test_name}.out"
        
        print(f"Running {test_name}...", end=' ')
        result = run_test(test_name, str(input_file), str(expected_file), str(executable), str(result_dir))
        basic_results.append(result)
        print(result['status'])
        
        if result['status'] in ['FAIL', 'ERROR', 'TIMEOUT']:
            print(f"  {result['details'][:100]}")
    
    all_results.extend(basic_results)
    
    # Run BigInteger tests
    print("\n=== BIGINTEGER TESTS (BigIntegerTest0-19) ===")
    bigint_results = []
    for i in range(20):
        test_name = f"BigIntegerTest{i}"
        input_file = bigint_dir / f"{test_name}.in"
        expected_file = bigint_dir / f"{test_name}.out"
        
        print(f"Running {test_name}...", end=' ')
        result = run_test(test_name, str(input_file), str(expected_file), str(executable), str(result_dir))
        bigint_results.append(result)
        print(result['status'])
        
        if result['status'] in ['FAIL', 'ERROR', 'TIMEOUT']:
            print(f"  {result['details'][:100]}")
    
    all_results.extend(bigint_results)
    
    # Summary
    print("\n=== SUMMARY ===")
    status_counts = {}
    for result in all_results:
        status = result['status']
        status_counts[status] = status_counts.get(status, 0) + 1
    
    for status, count in sorted(status_counts.items()):
        print(f"{status}: {count}")
    
    print(f"\nTotal tests: {len(all_results)}")
    
    # Save detailed report
    report_file = result_dir / "detailed_report.txt"
    with open(report_file, 'w') as f:
        f.write("=== COMPREHENSIVE TEST REPORT ===\n\n")
        f.write(f"Total tests: {len(all_results)}\n")
        for status, count in sorted(status_counts.items()):
            f.write(f"{status}: {count}\n")
        f.write("\n=== DETAILED RESULTS ===\n\n")
        for result in all_results:
            f.write(f"{result['name']}: {result['status']}\n")
            if result['details']:
                f.write(f"  {result['details']}\n")
            f.write("\n")
    
    print(f"\nDetailed report: {report_file}")

if __name__ == '__main__':
    main()
