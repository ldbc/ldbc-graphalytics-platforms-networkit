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
#include <networkit/components/WeaklyConnectedComponents.hpp>
#include <networkit/components/ConnectedComponents.hpp>


#include "utils.h"

using LCC_Result = std::vector<std::pair<NetworKit::node, double>>;
using Graph_Mapping = std::map<std::string, NetworKit::node>;

/*
 * Result serializer function
 */
void WriteOutWCCResult(
    const NetworKit::Graph &graph,
    NetworKit::WeaklyConnectedComponents &wcc,
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
        double distance = wcc.componentOfNode(node);
        if (distance == std::numeric_limits<double>::infinity()) {
            file << original_index << " " << "9223372036854775807" << std::endl;
        } else {
            file << original_index << " " << distance << std::endl;
        }
    }
}

/*
 * Result serializer function
 */
void WriteOutCCResult(
        const NetworKit::Graph &graph,
        NetworKit::ConnectedComponents &cc,
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
        double distance = cc.componentOfNode(node);
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


    if (graph.isDirected()) {

        std::cout << "Processing starts at: " << GetCurrentMilliseconds() << std::endl;
        // Execute the algorithm
        NetworKit::WeaklyConnectedComponents wcc(unweightedGraph);
        wcc.run();
        std::cout << "Processing ends at: " << GetCurrentMilliseconds() << std::endl;

        // Write out the results
        WriteOutWCCResult(
                unweightedGraph,
                wcc,
                reader.getNodeMap(),
                parameters
        );

    } else {
        std::cout << "Processing starts at: " << GetCurrentMilliseconds() << std::endl;
        // Execute the algorithm
        NetworKit::ConnectedComponents cc(unweightedGraph);
        cc.run();
        std::cout << "Processing ends at: " << GetCurrentMilliseconds() << std::endl;

        // Write out the results
        WriteOutCCResult(
                unweightedGraph,
                cc,
                reader.getNodeMap(),
                parameters
        );
    }
}
