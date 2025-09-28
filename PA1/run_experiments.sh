#!/bin/bash

# quick settings you can change
Ns=(100000000 1000000000 10000000000)
threads=(2 4 8)
tasks=(2 4 8)
reps=10

# grab the number after time=
grab_time() {
  awk -F'time=' 'NF>1{print $2}' | awk '{print $1}'
}

echo "Baseline"
for N in "${Ns[@]}"; do
  times=""
  for ((i=1;i<=reps;i++)); do
    ./baseline_seq "$N"
  done | grab_time > tmp_times.txt

  avg=$(python3 - <<'PY'
import sys,statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.mean(x):.6f}")
PY
)
  stdev=$(python3 - <<'PY'
import sys,statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.stdev(x):.6f}")
PY
)
  echo "N=$N avg=$avg stdev=$stdev"
done

echo
echo "Multithreading"
for N in "${Ns[@]}"; do
  for T in "${threads[@]}"; do
    for ((i=1;i<=reps;i++)); do
      ./Multithreading "$N" "$T"
    done | grab_time > tmp_times.txt

    avg=$(python3 - <<'PY'
import statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.mean(x):.6f}")
PY
)
    stdev=$(python3 - <<'PY'
import statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.stdev(x):.6f}")
PY
)
    echo "N=$N threads=$T avg=$avg stdev=$stdev"
  done
done

echo
echo "forkMultitasking"
for N in "${Ns[@]}"; do
  for P in "${tasks[@]}"; do
    for ((i=1;i<=reps;i++)); do
      ./forkMultitasking "$N" "$P"
    done | grab_time > tmp_times.txt

    avg=$(python3 - <<'PY'
import statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.mean(x):.6f}")
PY
)
    stdev=$(python3 - <<'PY'
import statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.stdev(x):.6f}")
PY
)
    echo "N=$N tasks=$P avg=$avg stdev=$stdev"
  done
done

echo
echo "popopenMultitasking"
for N in "${Ns[@]}"; do
  for P in "${tasks[@]}"; do
    for ((i=1;i<=reps;i++)); do
      ./popopenMultitasking "$N" "$P"
    done | grab_time > tmp_times.txt

    avg=$(python3 - <<'PY'
import statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.mean(x):.6f}")
PY
)
    stdev=$(python3 - <<'PY'
import statistics
x=[float(l.strip()) for l in open("tmp_times.txt") if l.strip()]
print(f"{statistics.stdev(x):.6f}")
PY
)
    echo "N=$N tasks=$P avg=$avg stdev=$stdev"
  done
done

rm -f tmp_times.txt
