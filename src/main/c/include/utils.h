#pragma once

#include <stdexcept>
#include <map>

/*
 * ARGUMENT PARSING FUNCTIONS
 */
typedef struct {
  std::string input_dir;
  std::string output_file;
  bool directed = false;
  unsigned long source_vertex = 0;
  double damping_factor = 0.0;
  unsigned long max_iteration = 0;
  int thread_num = 1;
} BenchmarkParameters;

BenchmarkParameters ParseBenchmarkParameters(int argc, char **argv);

typedef struct {
  std::string vertex_file;
  std::string edge_file;
  std::string market_file;
  std::string mapping_file;
  bool weighted = false;
  bool directed = false;
} ConverterParameters;

ConverterParameters ParseConverterParameters(int argc, char **argv);

/*
 * UTILITY METHODS
 */

time_t GetCurrentMilliseconds();

template<typename K, typename V>
static std::map<V, K> ReverseMap(const std::map<K, V>& m) {
    std::map<V, K> r;
    for (const auto& kv : m)
        r[kv.second] = kv.first;
    return r;
}