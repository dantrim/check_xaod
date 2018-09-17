# check_xaod
Simple for-loop example over xAOD

## What it does
Once built there is just a simple executable `check_xaod` that loops over an input DAOD file and counts
the total number of **Medium** muons and **LooseBLLH** electrons are seen. 

## Installation
Here are the steps to setup a directory structure and compilation of the `check_xaod` executable:
```bash
mkdir analysis_area/
cd analysis_area/
mkdir source/
cd source/
git clone git@github.com:dantrim/check_xaod.git
asetup "AnalysisBase,21.2,latest,here"
cd ..
mkdir build/
cd build/
cmake ../source
make -j
check_xaod <DAOD-FILE> [n-events-to-process]
```
The option `[n-events-to-prcess]` is optional, by default all events in the input file will be looped over.
