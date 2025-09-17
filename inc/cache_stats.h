#ifndef CACHE_STATS_H
#define CACHE_STATS_H

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

#include "channel.h"
#include "event_counter.h"

struct cache_stats {
  std::string name;
  // prefetch stats
  uint64_t pf_requested = 0;
  uint64_t pf_issued = 0;
  uint64_t pf_useful = 0;
  uint64_t pf_useless = 0;
  uint64_t pf_fill = 0;


  //HJ
  // ===== Cross translation/cache hit counters (50 total) =====
  // Context:
  //   These counters attribute where address translation eventually hits
  //   (L1D/L2C/LLC/MEM) and where the final cache tag check hits
  //   (L1I/L1D/L2C/LLC/MEM), conditioned on DTLB/ITLB miss and STLB hit/miss.
  //
  // Naming convention:
  //   <{d|i}tlb_miss>_stlb_{hit|miss}
  //     [ _<L1D|L2C|LLC|MEM>_hit ]   // only for the stlb_miss case: where translation hit
  //     _<L1I|L1D|L2C|LLC|MEM>_hit   // final cache tag hit level
  //
  // A) DTLB miss + STLB hit  → final cache tag hit level (5)
  uint64_t dtlb_miss_stlb_hit_L1I_hit = 0;
  uint64_t dtlb_miss_stlb_hit_L1D_hit = 0;
  uint64_t dtlb_miss_stlb_hit_L2C_hit = 0;
  uint64_t dtlb_miss_stlb_hit_LLC_hit = 0;
  uint64_t dtlb_miss_stlb_hit_MEM_hit = 0;

  // B) ITLB miss + STLB hit  → final cache tag hit level (5)
  uint64_t itlb_miss_stlb_hit_L1I_hit = 0;
  uint64_t itlb_miss_stlb_hit_L1D_hit = 0;
  uint64_t itlb_miss_stlb_hit_L2C_hit = 0;
  uint64_t itlb_miss_stlb_hit_LLC_hit = 0;
  uint64_t itlb_miss_stlb_hit_MEM_hit = 0;

  // C) DTLB miss + STLB miss + (translation hit level) × (final cache tag hit level) (20)
  //    stlb_miss_L1D_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t dtlb_miss_stlb_miss_L1D_hit_L1I_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L1D_hit_L1D_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L1D_hit_L2C_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L1D_hit_LLC_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L1D_hit_MEM_hit = 0;
  //    stlb_miss_L2C_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t dtlb_miss_stlb_miss_L2C_hit_L1I_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L2C_hit_L1D_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L2C_hit_L2C_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L2C_hit_LLC_hit = 0;
  uint64_t dtlb_miss_stlb_miss_L2C_hit_MEM_hit = 0;
  //    stlb_miss_LLC_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t dtlb_miss_stlb_miss_LLC_hit_L1I_hit = 0;
  uint64_t dtlb_miss_stlb_miss_LLC_hit_L1D_hit = 0;
  uint64_t dtlb_miss_stlb_miss_LLC_hit_L2C_hit = 0;
  uint64_t dtlb_miss_stlb_miss_LLC_hit_LLC_hit = 0;
  uint64_t dtlb_miss_stlb_miss_LLC_hit_MEM_hit = 0;
  //    stlb_miss_MEM_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t dtlb_miss_stlb_miss_MEM_hit_L1I_hit = 0;
  uint64_t dtlb_miss_stlb_miss_MEM_hit_L1D_hit = 0;
  uint64_t dtlb_miss_stlb_miss_MEM_hit_L2C_hit = 0;
  uint64_t dtlb_miss_stlb_miss_MEM_hit_LLC_hit = 0;
  uint64_t dtlb_miss_stlb_miss_MEM_hit_MEM_hit = 0;

  // D) ITLB miss + STLB miss + (translation hit level) × (final cache tag hit level) (20)
  //    stlb_miss_L1D_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t itlb_miss_stlb_miss_L1D_hit_L1I_hit = 0;
  uint64_t itlb_miss_stlb_miss_L1D_hit_L1D_hit = 0;
  uint64_t itlb_miss_stlb_miss_L1D_hit_L2C_hit = 0;
  uint64_t itlb_miss_stlb_miss_L1D_hit_LLC_hit = 0;
  uint64_t itlb_miss_stlb_miss_L1D_hit_MEM_hit = 0;
  //    stlb_miss_L2C_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t itlb_miss_stlb_miss_L2C_hit_L1I_hit = 0;
  uint64_t itlb_miss_stlb_miss_L2C_hit_L1D_hit = 0;
  uint64_t itlb_miss_stlb_miss_L2C_hit_L2C_hit = 0;
  uint64_t itlb_miss_stlb_miss_L2C_hit_LLC_hit = 0;
  uint64_t itlb_miss_stlb_miss_L2C_hit_MEM_hit = 0;
  //    stlb_miss_LLC_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t itlb_miss_stlb_miss_LLC_hit_L1I_hit = 0;
  uint64_t itlb_miss_stlb_miss_LLC_hit_L1D_hit = 0;
  uint64_t itlb_miss_stlb_miss_LLC_hit_L2C_hit = 0;
  uint64_t itlb_miss_stlb_miss_LLC_hit_LLC_hit = 0;
  uint64_t itlb_miss_stlb_miss_LLC_hit_MEM_hit = 0;
  //    stlb_miss_MEM_hit × {L1I,L1D,L2C,LLC,MEM}
  uint64_t itlb_miss_stlb_miss_MEM_hit_L1I_hit = 0;
  uint64_t itlb_miss_stlb_miss_MEM_hit_L1D_hit = 0;
  uint64_t itlb_miss_stlb_miss_MEM_hit_L2C_hit = 0;
  uint64_t itlb_miss_stlb_miss_MEM_hit_LLC_hit = 0;
  uint64_t itlb_miss_stlb_miss_MEM_hit_MEM_hit = 0;
  // ===== end of custom counters =====
  //HJ

  champsim::stats::event_counter<std::pair<access_type, std::remove_cv_t<decltype(NUM_CPUS)>>> hits = {};
  champsim::stats::event_counter<std::pair<access_type, std::remove_cv_t<decltype(NUM_CPUS)>>> misses = {};
  champsim::stats::event_counter<std::pair<access_type, std::remove_cv_t<decltype(NUM_CPUS)>>> mshr_merge = {};
  champsim::stats::event_counter<std::pair<access_type, std::remove_cv_t<decltype(NUM_CPUS)>>> mshr_return = {};

  long total_miss_latency_cycles{};
};

cache_stats operator-(cache_stats lhs, cache_stats rhs);

#endif
