/*
 * LCC algorithm implementation in GraphBLAS.
 */

#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>

#include <networkit/auxiliary/Parallelism.hpp>
#include <networkit/io/EdgeListReader.hpp>
#include <networkit/algebraic/CSRMatrix.hpp>
#include <networkit/centrality/PageRank.hpp>

#include "utils.h"

using LCC_Result = std::vector<std::pair<NetworKit::node, double>>;
using Graph_Mapping = std::map<std::string, NetworKit::node>;

/*
 * Result serializer function
 */
template<typename Matrix>
void WriteOutPRResult(
    const NetworKit::Graph &graph,
    NetworKit::PageRank &pr,
    const Graph_Mapping &mapping,
    const BenchmarkParameters &parameters
) {
    std::ofstream file{parameters.output_file};
    if (!file.is_open()) {
        std::cerr << "File " << parameters.output_file << " does not exists" << std::endl;
        exit(-1);
    }

    auto reverseMapping = ReverseMap(mapping);
    for (const auto &node : graph.nodes()) {
        std::string original_index = reverseMapping[node];
        double distance = pr.score(node);
        if (distance == std::numeric_limits<double>::infinity()) {
            file << original_index << " " << "9223372036854775807" << std::endl;
        } else {
            file << original_index << " " << distance << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    BenchmarkParameters parameters = ParseBenchmarkParameters(argc, argv);

    // Set used number of threads
    Aux::setNumberOfThreads(parameters.thread_num);

    // Read the graph
    auto reader = NetworKit::EdgeListReader(
        ' ',
        0,
        "#",
        false,
        parameters.directed
    );
    NetworKit::Graph graph = reader.read(parameters.input_dir + "/edge.csv");
    NetworKit::Graph unweightedGraph = graph.isWeighted() ? graph.toUnweighted() : graph;

    NetworKit::node sourceNode = reader.getNodeMap()[
        std::to_string(parameters.source_vertex)
    ];

    // Execute the algorithm
    std::cout << "Processing starts at: " << GetCurrentMilliseconds() << std::endl;
    NetworKit::PageRank pr(unweightedGraph);
    pr.run();
    std::cout << "Processing ends at: " << GetCurrentMilliseconds() << std::endl;

    // Write out the results
    WriteOutPRResult(
        unweightedGraph,
        pr,
        reader.getNodeMap(),
        parameters
    );
}
