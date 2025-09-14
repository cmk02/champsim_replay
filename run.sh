#!/usr/bin/env bash
# run_champsim_all.sh — ChampSim 트레이스 일괄 실행 (xz + gz 모두 지원)

set -euo pipefail

# 1) HOME 강제 설정 (원하면 경로만 바꿔)
export HOME="/home/cmk/eSCaL"
echo "[INFO] HOME set to: $HOME"

# 2) 기본 설정
MAX_PARALLEL="${MAX_PARALLEL:-20}"
CHAMPSIM_BIN="${CHAMPSIM_BIN:-./bin/champsim}"
WARMUP_ITERS="${WARMUP_ITERS:-50000000}"
MEASURE_ITERS="${MEASURE_ITERS:-20000000}"
LOG_ROOT="${LOG_ROOT:-$PWD/logs_$(date +%F_%H%M%S)}"

# 3) 트레이스 디렉토리
SPEC_DIR="$HOME/prefetch_aware_insertion/traces/spec2k17"   # *.champsimtrace.xz (가끔 gz)
GAP_DIR="$HOME/prefetch_aware_insertion/traces/gap"         # *.trace.{xz,gz}
CS_DIR="$HOME/prefetch_aware_insertion/traces/cs"           # *.trace.{xz,gz}
QUALCOMM_DIR="$HOME/prefetch_aware_insertion/traces/qualcomm_srv"

# 준비
if [[ ! -x "$CHAMPSIM_BIN" ]]; then
  echo "오류: ChampSim 바이너리($CHAMPSIM_BIN)를 찾을 수 없거나 실행 권한이 없습니다." >&2
  exit 1
fi
mkdir -p "$LOG_ROOT"
trap 'echo; echo "신호 수신 — 자식 작업 종료 시도..."; kill 0 || true' INT TERM

wait_for_slot() {
  local max="$1"
  while :; do
    local running
    running=$(jobs -rp | wc -l | tr -d ' ')
    (( running < max )) && break
    sleep 0.2
  done
}

run_one() {
  local suite="$1" file_path="$2"
  local base name out_dir log rc

  base="$(basename "$file_path")"
  # 확장자별로 벤치마크 이름 추출
  case "$base" in
    *.champsimtrace.xz) name="${base%.champsimtrace.xz}" ;;
    *.champsimtrace.gz) name="${base%.champsimtrace.gz}" ;;
    *.trace.xz)         name="${base%.trace.xz}" ;;
    *.trace.gz)         name="${base%.trace.gz}" ;;
    *.xz)               name="${base%.xz}" ;;
    *.gz)               name="${base%.gz}" ;;
    *)                  name="${base%.*}" ;;
  esac

  out_dir="$LOG_ROOT/$suite"
  mkdir -p "$out_dir"
  log="$out_dir/${name}.log"

  {
    echo "[$(date +'%F %T')] START suite=$suite trace=$name"
    echo "CMD: $CHAMPSIM_BIN -w $WARMUP_ITERS -i $MEASURE_ITERS $file_path"
    "$CHAMPSIM_BIN" -w "$WARMUP_ITERS" -i "$MEASURE_ITERS" "$file_path"
    rc=$?
    echo "[$(date +'%F %T')] END   suite=$suite trace=$name (rc=$rc)"
    exit "$rc"
  } >"$log" 2>&1
}

run_one_cs() {
  local suite="$1" file_path="$2"
  local base name out_dir log rc

  base="$(basename "$file_path")"
  # 확장자별로 벤치마크 이름 추출
  case "$base" in
    *.champsimtrace.xz) name="${base%.champsimtrace.xz}" ;;
    *.champsimtrace.gz) name="${base%.champsimtrace.gz}" ;;
    *.trace.xz)         name="${base%.trace.xz}" ;;
    *.trace.gz)         name="${base%.trace.gz}" ;;
    *.xz)               name="${base%.xz}" ;;
    *.gz)               name="${base%.gz}" ;;
    *)                  name="${base%.*}" ;;
  esac

  out_dir="$LOG_ROOT/$suite"
  mkdir -p "$out_dir"
  log="$out_dir/${name}.log"

  {
    echo "[$(date +'%F %T')] START suite=$suite trace=$name"
    echo "CMD: $CHAMPSIM_BIN -c -w $WARMUP_ITERS -i $MEASURE_ITERS $file_path"
    "$CHAMPSIM_BIN" -c -w "$WARMUP_ITERS" -i "$MEASURE_ITERS" "$file_path"
    rc=$?
    echo "[$(date +'%F %T')] END   suite=$suite trace=$name (rc=$rc)"
    exit "$rc"
  } >"$log" 2>&1
}


run_suite() {
  local suite="$1" dir="$2"
  echo "==== ${suite^^} 시작: $dir ===="
  if [[ ! -d "$dir" ]]; then
    echo "경고: $dir 디렉토리가 없어 건너뜁니다."
    return 0
  fi

  local any=0
  # xz + gz 모두 탐색 (정렬 포함)
  while IFS= read -r -d '' f; do
    any=1
    wait_for_slot "$MAX_PARALLEL"
    run_one "$suite" "$f" &
    echo " -> queued [$suite] $(basename "$f")"
  done < <(find "$dir" -maxdepth 1 -type f \
           \( -name '*.champsimtrace.xz' -o -name '*.trace.xz' -o -name '*.champsimtrace.gz' -o -name '*.trace.gz' \) \
           -print0 | sort -z)

  (( any )) || echo "   (트레이스 없음)"
  wait
  echo "==== ${suite^^} 완료 ===="
  echo
}

run_cs_suite() {
  local suite="$1" dir="$2"
  echo "==== ${suite^^} 시작: $dir ===="
  if [[ ! -d "$dir" ]]; then
    echo "경고: $dir 디렉토리가 없어 건너뜁니다."
    return 0
  fi

  local any=0
  # xz + gz 모두 탐색 (정렬 포함)
  while IFS= read -r -d '' f; do
    any=1
    wait_for_slot "$MAX_PARALLEL"
    run_one_cs "$suite" "$f" &
    echo " -> queued [$suite] $(basename "$f")"
  done < <(find "$dir" -maxdepth 1 -type f \
           \( -name '*.champsimtrace.xz' -o -name '*.trace.xz' -o -name '*.champsimtrace.gz' -o -name '*.trace.gz' \) \
           -print0 | sort -z)

  (( any )) || echo "   (트레이스 없음)"
  wait
  echo "==== ${suite^^} 완료 ===="
  echo
}

# 실행 순서 (원하면 주석으로 선택)
run_suite "spec" "$SPEC_DIR"
# run_suite "gap" "$GAP_DIR"
# run_cs_suite "cs" "$CS_DIR"
# run_suite "qualcomm_srv" "$QUALCOMM_DIR"

echo "모든 스위트 완료. 로그: $LOG_ROOT/{spec,gap,cs,qualcomm_srv}/*.log"