#include "cache_stats.h"

cache_stats operator-(cache_stats lhs, cache_stats rhs)
{
  cache_stats result;
  result.pf_requested = lhs.pf_requested - rhs.pf_requested;
  result.pf_issued = lhs.pf_issued - rhs.pf_issued;
  result.pf_useful = lhs.pf_useful - rhs.pf_useful;
  result.pf_useless = lhs.pf_useless - rhs.pf_useless;
  result.pf_fill = lhs.pf_fill - rhs.pf_fill;

  
  // HJ: extended TLB/cache cross counters (lhs - rhs)
  // A) DTLB miss + STLB hit -> final level
  result.dtlb_miss_stlb_hit_L1I_hit = lhs.dtlb_miss_stlb_hit_L1I_hit - rhs.dtlb_miss_stlb_hit_L1I_hit;
  result.dtlb_miss_stlb_hit_L1D_hit = lhs.dtlb_miss_stlb_hit_L1D_hit - rhs.dtlb_miss_stlb_hit_L1D_hit;
  result.dtlb_miss_stlb_hit_L2C_hit = lhs.dtlb_miss_stlb_hit_L2C_hit - rhs.dtlb_miss_stlb_hit_L2C_hit;
  result.dtlb_miss_stlb_hit_LLC_hit = lhs.dtlb_miss_stlb_hit_LLC_hit - rhs.dtlb_miss_stlb_hit_LLC_hit;
  result.dtlb_miss_stlb_hit_MEM_hit = lhs.dtlb_miss_stlb_hit_MEM_hit - rhs.dtlb_miss_stlb_hit_MEM_hit;

  // B) ITLB miss + STLB hit -> final level
  result.itlb_miss_stlb_hit_L1I_hit = lhs.itlb_miss_stlb_hit_L1I_hit - rhs.itlb_miss_stlb_hit_L1I_hit;
  result.itlb_miss_stlb_hit_L1D_hit = lhs.itlb_miss_stlb_hit_L1D_hit - rhs.itlb_miss_stlb_hit_L1D_hit;
  result.itlb_miss_stlb_hit_L2C_hit = lhs.itlb_miss_stlb_hit_L2C_hit - rhs.itlb_miss_stlb_hit_L2C_hit;
  result.itlb_miss_stlb_hit_LLC_hit = lhs.itlb_miss_stlb_hit_LLC_hit - rhs.itlb_miss_stlb_hit_LLC_hit;
  result.itlb_miss_stlb_hit_MEM_hit = lhs.itlb_miss_stlb_hit_MEM_hit - rhs.itlb_miss_stlb_hit_MEM_hit;

  // C) DTLB miss + STLB miss (resolved @ L1D/L2C/LLC/MEM) -> final level
  result.dtlb_miss_stlb_miss_L1D_hit_L1I_hit = lhs.dtlb_miss_stlb_miss_L1D_hit_L1I_hit - rhs.dtlb_miss_stlb_miss_L1D_hit_L1I_hit;
  result.dtlb_miss_stlb_miss_L1D_hit_L1D_hit = lhs.dtlb_miss_stlb_miss_L1D_hit_L1D_hit - rhs.dtlb_miss_stlb_miss_L1D_hit_L1D_hit;
  result.dtlb_miss_stlb_miss_L1D_hit_L2C_hit = lhs.dtlb_miss_stlb_miss_L1D_hit_L2C_hit - rhs.dtlb_miss_stlb_miss_L1D_hit_L2C_hit;
  result.dtlb_miss_stlb_miss_L1D_hit_LLC_hit = lhs.dtlb_miss_stlb_miss_L1D_hit_LLC_hit - rhs.dtlb_miss_stlb_miss_L1D_hit_LLC_hit;
  result.dtlb_miss_stlb_miss_L1D_hit_MEM_hit = lhs.dtlb_miss_stlb_miss_L1D_hit_MEM_hit - rhs.dtlb_miss_stlb_miss_L1D_hit_MEM_hit;

  result.dtlb_miss_stlb_miss_L2C_hit_L1I_hit = lhs.dtlb_miss_stlb_miss_L2C_hit_L1I_hit - rhs.dtlb_miss_stlb_miss_L2C_hit_L1I_hit;
  result.dtlb_miss_stlb_miss_L2C_hit_L1D_hit = lhs.dtlb_miss_stlb_miss_L2C_hit_L1D_hit - rhs.dtlb_miss_stlb_miss_L2C_hit_L1D_hit;
  result.dtlb_miss_stlb_miss_L2C_hit_L2C_hit = lhs.dtlb_miss_stlb_miss_L2C_hit_L2C_hit - rhs.dtlb_miss_stlb_miss_L2C_hit_L2C_hit;
  result.dtlb_miss_stlb_miss_L2C_hit_LLC_hit = lhs.dtlb_miss_stlb_miss_L2C_hit_LLC_hit - rhs.dtlb_miss_stlb_miss_L2C_hit_LLC_hit;
  result.dtlb_miss_stlb_miss_L2C_hit_MEM_hit = lhs.dtlb_miss_stlb_miss_L2C_hit_MEM_hit - rhs.dtlb_miss_stlb_miss_L2C_hit_MEM_hit;

  result.dtlb_miss_stlb_miss_LLC_hit_L1I_hit = lhs.dtlb_miss_stlb_miss_LLC_hit_L1I_hit - rhs.dtlb_miss_stlb_miss_LLC_hit_L1I_hit;
  result.dtlb_miss_stlb_miss_LLC_hit_L1D_hit = lhs.dtlb_miss_stlb_miss_LLC_hit_L1D_hit - rhs.dtlb_miss_stlb_miss_LLC_hit_L1D_hit;
  result.dtlb_miss_stlb_miss_LLC_hit_L2C_hit = lhs.dtlb_miss_stlb_miss_LLC_hit_L2C_hit - rhs.dtlb_miss_stlb_miss_LLC_hit_L2C_hit;
  result.dtlb_miss_stlb_miss_LLC_hit_LLC_hit = lhs.dtlb_miss_stlb_miss_LLC_hit_LLC_hit - rhs.dtlb_miss_stlb_miss_LLC_hit_LLC_hit;
  result.dtlb_miss_stlb_miss_LLC_hit_MEM_hit = lhs.dtlb_miss_stlb_miss_LLC_hit_MEM_hit - rhs.dtlb_miss_stlb_miss_LLC_hit_MEM_hit;

  result.dtlb_miss_stlb_miss_MEM_hit_L1I_hit = lhs.dtlb_miss_stlb_miss_MEM_hit_L1I_hit - rhs.dtlb_miss_stlb_miss_MEM_hit_L1I_hit;
  result.dtlb_miss_stlb_miss_MEM_hit_L1D_hit = lhs.dtlb_miss_stlb_miss_MEM_hit_L1D_hit - rhs.dtlb_miss_stlb_miss_MEM_hit_L1D_hit;
  result.dtlb_miss_stlb_miss_MEM_hit_L2C_hit = lhs.dtlb_miss_stlb_miss_MEM_hit_L2C_hit - rhs.dtlb_miss_stlb_miss_MEM_hit_L2C_hit;
  result.dtlb_miss_stlb_miss_MEM_hit_LLC_hit = lhs.dtlb_miss_stlb_miss_MEM_hit_LLC_hit - rhs.dtlb_miss_stlb_miss_MEM_hit_LLC_hit;
  result.dtlb_miss_stlb_miss_MEM_hit_MEM_hit = lhs.dtlb_miss_stlb_miss_MEM_hit_MEM_hit - rhs.dtlb_miss_stlb_miss_MEM_hit_MEM_hit;

  // D) ITLB miss + STLB miss (resolved @ L1D/L2C/LLC/MEM) -> final level
  result.itlb_miss_stlb_miss_L1D_hit_L1I_hit = lhs.itlb_miss_stlb_miss_L1D_hit_L1I_hit - rhs.itlb_miss_stlb_miss_L1D_hit_L1I_hit;
  result.itlb_miss_stlb_miss_L1D_hit_L1D_hit = lhs.itlb_miss_stlb_miss_L1D_hit_L1D_hit - rhs.itlb_miss_stlb_miss_L1D_hit_L1D_hit;
  result.itlb_miss_stlb_miss_L1D_hit_L2C_hit = lhs.itlb_miss_stlb_miss_L1D_hit_L2C_hit - rhs.itlb_miss_stlb_miss_L1D_hit_L2C_hit;
  result.itlb_miss_stlb_miss_L1D_hit_LLC_hit = lhs.itlb_miss_stlb_miss_L1D_hit_LLC_hit - rhs.itlb_miss_stlb_miss_L1D_hit_LLC_hit;
  result.itlb_miss_stlb_miss_L1D_hit_MEM_hit = lhs.itlb_miss_stlb_miss_L1D_hit_MEM_hit - rhs.itlb_miss_stlb_miss_L1D_hit_MEM_hit;

  result.itlb_miss_stlb_miss_L2C_hit_L1I_hit = lhs.itlb_miss_stlb_miss_L2C_hit_L1I_hit - rhs.itlb_miss_stlb_miss_L2C_hit_L1I_hit;
  result.itlb_miss_stlb_miss_L2C_hit_L1D_hit = lhs.itlb_miss_stlb_miss_L2C_hit_L1D_hit - rhs.itlb_miss_stlb_miss_L2C_hit_L1D_hit;
  result.itlb_miss_stlb_miss_L2C_hit_L2C_hit = lhs.itlb_miss_stlb_miss_L2C_hit_L2C_hit - rhs.itlb_miss_stlb_miss_L2C_hit_L2C_hit;
  result.itlb_miss_stlb_miss_L2C_hit_LLC_hit = lhs.itlb_miss_stlb_miss_L2C_hit_LLC_hit - rhs.itlb_miss_stlb_miss_L2C_hit_LLC_hit;
  result.itlb_miss_stlb_miss_L2C_hit_MEM_hit = lhs.itlb_miss_stlb_miss_L2C_hit_MEM_hit - rhs.itlb_miss_stlb_miss_L2C_hit_MEM_hit;

  result.itlb_miss_stlb_miss_LLC_hit_L1I_hit = lhs.itlb_miss_stlb_miss_LLC_hit_L1I_hit - rhs.itlb_miss_stlb_miss_LLC_hit_L1I_hit;
  result.itlb_miss_stlb_miss_LLC_hit_L1D_hit = lhs.itlb_miss_stlb_miss_LLC_hit_L1D_hit - rhs.itlb_miss_stlb_miss_LLC_hit_L1D_hit;
  result.itlb_miss_stlb_miss_LLC_hit_L2C_hit = lhs.itlb_miss_stlb_miss_LLC_hit_L2C_hit - rhs.itlb_miss_stlb_miss_LLC_hit_L2C_hit;
  result.itlb_miss_stlb_miss_LLC_hit_LLC_hit = lhs.itlb_miss_stlb_miss_LLC_hit_LLC_hit - rhs.itlb_miss_stlb_miss_LLC_hit_LLC_hit;
  result.itlb_miss_stlb_miss_LLC_hit_MEM_hit = lhs.itlb_miss_stlb_miss_LLC_hit_MEM_hit - rhs.itlb_miss_stlb_miss_LLC_hit_MEM_hit;

  result.itlb_miss_stlb_miss_MEM_hit_L1I_hit = lhs.itlb_miss_stlb_miss_MEM_hit_L1I_hit - rhs.itlb_miss_stlb_miss_MEM_hit_L1I_hit;
  result.itlb_miss_stlb_miss_MEM_hit_L1D_hit = lhs.itlb_miss_stlb_miss_MEM_hit_L1D_hit - rhs.itlb_miss_stlb_miss_MEM_hit_L1D_hit;
  result.itlb_miss_stlb_miss_MEM_hit_L2C_hit = lhs.itlb_miss_stlb_miss_MEM_hit_L2C_hit - rhs.itlb_miss_stlb_miss_MEM_hit_L2C_hit;
  result.itlb_miss_stlb_miss_MEM_hit_LLC_hit = lhs.itlb_miss_stlb_miss_MEM_hit_LLC_hit - rhs.itlb_miss_stlb_miss_MEM_hit_LLC_hit;
  result.itlb_miss_stlb_miss_MEM_hit_MEM_hit = lhs.itlb_miss_stlb_miss_MEM_hit_MEM_hit - rhs.itlb_miss_stlb_miss_MEM_hit_MEM_hit;
  //HJ


  result.hits = lhs.hits - rhs.hits;
  result.misses = lhs.misses - rhs.misses;

  result.total_miss_latency_cycles = lhs.total_miss_latency_cycles - rhs.total_miss_latency_cycles;
  return result;
}
