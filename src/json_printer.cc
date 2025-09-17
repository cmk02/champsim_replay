/*
 *    Copyright 2023 The ChampSim Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <utility>
#include <nlohmann/json.hpp>

#include "stats_printer.h"

void to_json(nlohmann::json& j, const O3_CPU::stats_type& stats)
{
  constexpr std::array types{branch_type::BRANCH_DIRECT_JUMP, branch_type::BRANCH_INDIRECT,      branch_type::BRANCH_CONDITIONAL,
                             branch_type::BRANCH_DIRECT_CALL, branch_type::BRANCH_INDIRECT_CALL, branch_type::BRANCH_RETURN};

  auto total_mispredictions = std::ceil(
      std::accumulate(std::begin(types), std::end(types), 0LL, [btm = stats.branch_type_misses](auto acc, auto next) { return acc + btm.value_or(next, 0); }));

  std::map<std::string, std::size_t> mpki{};
  for (auto type : types) {
    mpki.emplace(branch_type_names.at(champsim::to_underlying(type)), stats.branch_type_misses.value_or(type, 0));
  }

  j = nlohmann::json{{"instructions", stats.instrs()},
                     {"cycles", stats.cycles()},
                     {"Avg ROB occupancy at mispredict", std::ceil(stats.total_rob_occupancy_at_branch_mispredict) / std::ceil(total_mispredictions)},
                     {"mispredict", mpki}};
}

void to_json(nlohmann::json& j, const CACHE::stats_type& stats)
{
  using hits_value_type = typename decltype(stats.hits)::value_type;
  using misses_value_type = typename decltype(stats.misses)::value_type;
  using mshr_merge_value_type = typename decltype(stats.mshr_merge)::value_type;
  using mshr_return_value_type = typename decltype(stats.mshr_return)::value_type;

  std::map<std::string, nlohmann::json> statsmap;
  statsmap.emplace("prefetch requested", stats.pf_requested);
  statsmap.emplace("prefetch issued", stats.pf_issued);
  statsmap.emplace("useful prefetch", stats.pf_useful);
  statsmap.emplace("useless prefetch", stats.pf_useless);



  // HJ: extended TLB/cache cross counters
  // A) dtlb miss + stlb hit -> final level
  statsmap.emplace("dtlb miss + stlb hit -> l1i hit", stats.dtlb_miss_stlb_hit_L1I_hit);
  statsmap.emplace("dtlb miss + stlb hit -> l1d hit", stats.dtlb_miss_stlb_hit_L1D_hit);
  statsmap.emplace("dtlb miss + stlb hit -> l2c hit", stats.dtlb_miss_stlb_hit_L2C_hit);
  statsmap.emplace("dtlb miss + stlb hit -> llc hit", stats.dtlb_miss_stlb_hit_LLC_hit);
  statsmap.emplace("dtlb miss + stlb hit -> mem hit", stats.dtlb_miss_stlb_hit_MEM_hit);

  // B) itlb miss + stlb hit -> final level
  statsmap.emplace("itlb miss + stlb hit -> l1i hit", stats.itlb_miss_stlb_hit_L1I_hit);
  statsmap.emplace("itlb miss + stlb hit -> l1d hit", stats.itlb_miss_stlb_hit_L1D_hit);
  statsmap.emplace("itlb miss + stlb hit -> l2c hit", stats.itlb_miss_stlb_hit_L2C_hit);
  statsmap.emplace("itlb miss + stlb hit -> llc hit", stats.itlb_miss_stlb_hit_LLC_hit);
  statsmap.emplace("itlb miss + stlb hit -> mem hit", stats.itlb_miss_stlb_hit_MEM_hit);

  // C) dtlb miss + stlb miss (resolved @ X) -> final level
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l1d) -> l1i hit", stats.dtlb_miss_stlb_miss_L1D_hit_L1I_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l1d) -> l1d hit", stats.dtlb_miss_stlb_miss_L1D_hit_L1D_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l1d) -> l2c hit", stats.dtlb_miss_stlb_miss_L1D_hit_L2C_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l1d) -> llc hit", stats.dtlb_miss_stlb_miss_L1D_hit_LLC_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l1d) -> mem hit", stats.dtlb_miss_stlb_miss_L1D_hit_MEM_hit);

  statsmap.emplace("dtlb miss + stlb miss (resolved @ l2c) -> l1i hit", stats.dtlb_miss_stlb_miss_L2C_hit_L1I_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l2c) -> l1d hit", stats.dtlb_miss_stlb_miss_L2C_hit_L1D_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l2c) -> l2c hit", stats.dtlb_miss_stlb_miss_L2C_hit_L2C_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l2c) -> llc hit", stats.dtlb_miss_stlb_miss_L2C_hit_LLC_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ l2c) -> mem hit", stats.dtlb_miss_stlb_miss_L2C_hit_MEM_hit);

  statsmap.emplace("dtlb miss + stlb miss (resolved @ llc) -> l1i hit", stats.dtlb_miss_stlb_miss_LLC_hit_L1I_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ llc) -> l1d hit", stats.dtlb_miss_stlb_miss_LLC_hit_L1D_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ llc) -> l2c hit", stats.dtlb_miss_stlb_miss_LLC_hit_L2C_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ llc) -> llc hit", stats.dtlb_miss_stlb_miss_LLC_hit_LLC_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ llc) -> mem hit", stats.dtlb_miss_stlb_miss_LLC_hit_MEM_hit);

  statsmap.emplace("dtlb miss + stlb miss (resolved @ mem) -> l1i hit", stats.dtlb_miss_stlb_miss_MEM_hit_L1I_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ mem) -> l1d hit", stats.dtlb_miss_stlb_miss_MEM_hit_L1D_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ mem) -> l2c hit", stats.dtlb_miss_stlb_miss_MEM_hit_L2C_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ mem) -> llc hit", stats.dtlb_miss_stlb_miss_MEM_hit_LLC_hit);
  statsmap.emplace("dtlb miss + stlb miss (resolved @ mem) -> mem hit", stats.dtlb_miss_stlb_miss_MEM_hit_MEM_hit);

  // D) itlb miss + stlb miss (resolved @ X) -> final level
  statsmap.emplace("itlb miss + stlb miss (resolved @ l1d) -> l1i hit", stats.itlb_miss_stlb_miss_L1D_hit_L1I_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l1d) -> l1d hit", stats.itlb_miss_stlb_miss_L1D_hit_L1D_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l1d) -> l2c hit", stats.itlb_miss_stlb_miss_L1D_hit_L2C_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l1d) -> llc hit", stats.itlb_miss_stlb_miss_L1D_hit_LLC_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l1d) -> mem hit", stats.itlb_miss_stlb_miss_L1D_hit_MEM_hit);

  statsmap.emplace("itlb miss + stlb miss (resolved @ l2c) -> l1i hit", stats.itlb_miss_stlb_miss_L2C_hit_L1I_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l2c) -> l1d hit", stats.itlb_miss_stlb_miss_L2C_hit_L1D_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l2c) -> l2c hit", stats.itlb_miss_stlb_miss_L2C_hit_L2C_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l2c) -> llc hit", stats.itlb_miss_stlb_miss_L2C_hit_LLC_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ l2c) -> mem hit", stats.itlb_miss_stlb_miss_L2C_hit_MEM_hit);

  statsmap.emplace("itlb miss + stlb miss (resolved @ llc) -> l1i hit", stats.itlb_miss_stlb_miss_LLC_hit_L1I_hit); 
  statsmap.emplace("itlb miss + stlb miss (resolved @ llc) -> l1d hit", stats.itlb_miss_stlb_miss_LLC_hit_L1D_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ llc) -> l2c hit", stats.itlb_miss_stlb_miss_LLC_hit_L2C_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ llc) -> llc hit", stats.itlb_miss_stlb_miss_LLC_hit_LLC_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ llc) -> mem hit", stats.itlb_miss_stlb_miss_LLC_hit_MEM_hit);

  statsmap.emplace("itlb miss + stlb miss (resolved @ mem) -> l1i hit", stats.itlb_miss_stlb_miss_MEM_hit_L1I_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ mem) -> l1d hit", stats.itlb_miss_stlb_miss_MEM_hit_L1D_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ mem) -> l2c hit", stats.itlb_miss_stlb_miss_MEM_hit_L2C_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ mem) -> llc hit", stats.itlb_miss_stlb_miss_MEM_hit_LLC_hit);
  statsmap.emplace("itlb miss + stlb miss (resolved @ mem) -> mem hit", stats.itlb_miss_stlb_miss_MEM_hit_MEM_hit);
  //HJ


  uint64_t total_downstream_demands = stats.mshr_return.total();
  for (std::size_t cpu = 0; cpu < NUM_CPUS; ++cpu)
    total_downstream_demands -= stats.mshr_return.value_or(std::pair{access_type::PREFETCH, cpu}, mshr_return_value_type{});

  statsmap.emplace("miss latency", std::ceil(stats.total_miss_latency_cycles) / std::ceil(total_downstream_demands));
  for (const auto type : {access_type::LOAD, access_type::RFO, access_type::PREFETCH, access_type::WRITE, access_type::TRANSLATION}) {
    std::vector<hits_value_type> hits;
    std::vector<misses_value_type> misses;
    std::vector<mshr_merge_value_type> mshr_merges;

    for (std::size_t cpu = 0; cpu < NUM_CPUS; ++cpu) {
      hits.push_back(stats.hits.value_or(std::pair{type, cpu}, hits_value_type{}));
      misses.push_back(stats.misses.value_or(std::pair{type, cpu}, misses_value_type{}));
      mshr_merges.push_back(stats.mshr_merge.value_or(std::pair{type, cpu}, mshr_merge_value_type{}));
    }

    statsmap.emplace(access_type_names.at(champsim::to_underlying(type)), nlohmann::json{{"hit", hits}, {"miss", misses}, {"mshr_merge", mshr_merges}});
  }

  j = statsmap;
}

void to_json(nlohmann::json& j, const DRAM_CHANNEL::stats_type stats)
{
  j = nlohmann::json{{"RQ ROW_BUFFER_HIT", stats.RQ_ROW_BUFFER_HIT},
                     {"RQ ROW_BUFFER_MISS", stats.RQ_ROW_BUFFER_MISS},
                     {"WQ ROW_BUFFER_HIT", stats.WQ_ROW_BUFFER_HIT},
                     {"WQ ROW_BUFFER_MISS", stats.WQ_ROW_BUFFER_MISS},
                     {"AVG DBUS CONGESTED CYCLE", (std::ceil(stats.dbus_cycle_congested) / std::ceil(stats.dbus_count_congested))},
                     {"REFRESHES ISSUED", stats.refresh_cycles}};
}

namespace champsim
{
void to_json(nlohmann::json& j, const champsim::phase_stats stats)
{
  std::map<std::string, nlohmann::json> roi_stats;
  roi_stats.emplace("cores", stats.roi_cpu_stats);
  roi_stats.emplace("DRAM", stats.roi_dram_stats);
  for (auto x : stats.roi_cache_stats) {
    roi_stats.emplace(x.name, x);
  }

  std::map<std::string, nlohmann::json> sim_stats;
  sim_stats.emplace("cores", stats.sim_cpu_stats);
  sim_stats.emplace("DRAM", stats.sim_dram_stats);
  for (auto x : stats.sim_cache_stats) {
    sim_stats.emplace(x.name, x);
  }

  std::map<std::string, nlohmann::json> statsmap{{"name", stats.name}, {"traces", stats.trace_names}};
  statsmap.emplace("roi", roi_stats);
  statsmap.emplace("sim", sim_stats);
  j = statsmap;
}
} // namespace champsim

void champsim::json_printer::print(std::vector<phase_stats>& stats) { stream << nlohmann::json::array_t{std::begin(stats), std::end(stats)}; }
