#!/usr/bin/env bash
# run_champsim_all.sh — ChampSim 트레이스 일괄 실행 (동시 최대 10개, 스위트 순차 수행)

set -euo pipefail

# === 1) HOME을 스크립트 내에서 강제 설정 ===
# 필요 시 이 줄만 원하는 경로로 고쳐서 사용하세요.
export HOME="/home/ho"
echo "[INFO] HOME set to: $HOME"

# === 2) 기본 설정 (원하면 환경변수로 덮어쓰기 가능) ===
MAX_PARALLEL="${MAX_PARALLEL:-10}"                 # 동시에 돌아갈 최대 작업 수(기본 10)
CHAMPSIM_BIN="${CHAMPSIM_BIN:-./bin/champsim}"     # ChampSim 실행 파일
WARMUP_ITERS="${WARMUP_ITERS:-20000000}"           # -w
MEASURE_ITERS="${MEASURE_ITERS:-100000000}"        # -i
LOG_ROOT="${LOG_ROOT:-$PWD/logs_$(date +%F_%H%M%S)}"

# === 3) 트레이스 디렉토리 ===
SPEC_DIR="$HOME/prefetch_aware_insertion_ptr/traces/spec2k17"
GAP_DIR="$HOME/prefetch_aware_insertion_ptr/traces/gap"
CS_DIR="$HOME/prefetch_aware_insertion_ptr/traces/cs"
QUALCOMM_DIR="$HOME/itp_asplos25_AE/traces/qualcomm_srv"   # (// → / 정리)

# === 준비 ===
if [[ ! -x "$CHAMPSIM_BIN" ]]; then
  echo "오류: ChampSim 바이너리($CHAMPSIM_BIN)를 찾을 수 없거나 실행 권한이 없습니다." >&2
  exit 1
fi
mkdir -p "$LOG_ROOT"
trap 'echo; echo "신호 수신 — 자식 작업 종료 시도..."; kill 0 || true' INT TERM

# 현재 백그라운드 작업 수가 MAX_PARALLEL 미만이 될 때까지 대기
wait_for_slot() {
  local max="$1"
  while :; do
    local running
    running=$(jobs -rp | wc -l | tr -d ' ')
    (( running < max )) && break
    sleep 0.2
  done
}

# 단일 트레이스 실행(로그는 <suite>/<bench>.log)
run_one() {
  local suite="$1" trace_dir="$2" trace_path="$3"
  local base name out_dir log rc
  base="$(basename "$trace_path")"
  name="${base%.champsimtrace.xz}"             # 벤치마크 이름
  out_dir="$LOG_ROOT/$suite"
  mkdir -p "$out_dir"
  log="$out_dir/${name}.log"

  {
    echo "[$(date +'%F %T')] START suite=$suite trace=$name"
    echo "CMD: $CHAMPSIM_BIN -w $WARMUP_ITERS -i $MEASURE_ITERS $trace_dir/${name}.champsimtrace.xz"
    "$CHAMPSIM_BIN" -w "$WARMUP_ITERS" -i "$MEASURE_ITERS" "$trace_dir/${name}.champsimtrace.xz"
    rc=$?
    echo "[$(date +'%F %T')] END   suite=$suite trace=$name (rc=$rc)"
    exit "$rc"
  } >"$log" 2>&1
}

# 스위트 단위 실행: spec → gap → cs → qualcomm_srv 순서로 각 스위트를 완주
run_suite() {
  local suite="$1" dir="$2"
  echo "==== ${suite^^} 시작: $dir ===="
  if [[ ! -d "$dir" ]]; then
    echo "경고: $dir 디렉토리가 없어 건너뜁니다."
    return 0
  fi

  local any=0
  while IFS= read -r -d '' f; do
    any=1
    wait_for_slot "$MAX_PARALLEL"
    run_one "$suite" "$dir" "$f" &
    echo " -> queued [$suite] $(basename "$f")"
  done < <(find "$dir" -maxdepth 1 -type f -name '*.champsimtrace.xz' -print0)

  if (( ! any )); then
    echo "   (트레이스 *.champsimtrace.xz 없음)"
  fi

  # 해당 스위트의 모든 작업 종료 대기
  wait
  echo "==== ${suite^^} 완료 ===="
  echo
}

# === 실행 순서: spec → gap → cs → qualcomm_srv ===
run_suite "spec" "$SPEC_DIR"
run_suite "gap" "$GAP_DIR"
run_suite "cs" "$CS_DIR"
run_suite "qualcomm_srv" "$QUALCOMM_DIR"

echo "모든 스위트 완료. 로그 위치: $LOG_ROOT/{spec,gap,cs,qualcomm_srv}/*.log"
