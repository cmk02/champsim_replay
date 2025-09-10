#ifndef CORE_STATS_H
#define CORE_STATS_H

#include <cstdint>
#include <string>

#include "event_counter.h"
#include "instruction.h"

//@Minchan
enum StallType{
  ReOrderBuffer,
  LoadQueue,
  StoreQueue,
  NUM_STALL_TYPE
};

enum ROBStallType{
  ADDR_TRANS,
  REPLAY_LOAD,
  NON_REPLAY_LOAD,
  NUM_ROB_STALL_TYPE
};

struct cpu_stats {
  std::string name;
  long long begin_instrs = 0;
  long long begin_cycles = 0;
  long long end_instrs = 0;
  long long end_cycles = 0;
  uint64_t total_rob_occupancy_at_branch_mispredict = 0;

  //@Minchan
  uint64_t stall_cycles[StallType::NUM_STALL_TYPE] = {0};
  uint64_t rob_stall_cycles[ROBStallType::NUM_ROB_STALL_TYPE] = {0};
  uint32_t rob_stall_counts[ROBStallType::NUM_ROB_STALL_TYPE] = {0};
  uint64_t non_stall_cycles = 0;

  champsim::stats::event_counter<branch_type> total_branch_types = {};
  champsim::stats::event_counter<branch_type> branch_type_misses = {};

  [[nodiscard]] auto instrs() const { return end_instrs - begin_instrs; }
  [[nodiscard]] auto cycles() const { return end_cycles - begin_cycles; }
};

cpu_stats operator-(cpu_stats lhs, cpu_stats rhs);

#endif
