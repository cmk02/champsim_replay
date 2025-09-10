#include "core_stats.h"

cpu_stats operator-(cpu_stats lhs, cpu_stats rhs)
{
  lhs.begin_instrs -= rhs.begin_instrs;
  lhs.begin_cycles -= rhs.begin_cycles;
  lhs.end_instrs -= rhs.end_instrs;
  lhs.end_cycles -= rhs.end_cycles;
  lhs.total_rob_occupancy_at_branch_mispredict -= rhs.total_rob_occupancy_at_branch_mispredict;

  lhs.total_branch_types -= rhs.total_branch_types;
  lhs.branch_type_misses -= rhs.branch_type_misses;

  //@Minchan
  for (int i=0; i<StallType::NUM_STALL_TYPE; i++){
    lhs.stall_cycles[i] -= rhs.stall_cycles[i];
  }
  for (int i=0; i<ROBStallType::NUM_ROB_STALL_TYPE; i++){
    lhs.rob_stall_cycles[i] -= rhs.rob_stall_cycles[i];
    lhs.rob_stall_counts[i] -= rhs.rob_stall_counts[i];
  }

  return lhs;
}
