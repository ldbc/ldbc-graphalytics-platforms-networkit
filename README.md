# Graphalytics NetworKit platform driver

NetworKit is a linear algebra-based library for graph analytics. To execute Graphalytics benchmark on NetworKit, follow the steps in the Graphalytics tutorial on [Running Benchmark](https://github.com/ldbc/ldbc_graphalytics/wiki/Manual%3A-Running-Benchmark) with the NetworKit-specific instructions listed below.

## Getting started

Install NetworKit:

```bash
git clone https://github.com/networkit/networkit.git
git checkout 6.1
git submodule init
git submodule update
mkdir build
cd build/
JOBS=$(nproc) make
sudo make install
sudo ldconfig
```

In this repository, initialize the experiments:

```bash
./init.sh
```
