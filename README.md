# OSS-fuzz libpng project
## Project Description
OSS-Fuzz aims to make common open source software more secure and stable by combining modern fuzzing techniques with scalable, distributed execution. In this project we aim to analyze and improve the existing [existing oss-fuzz](https://github.com/google/oss-fuzz.git) project for [libpng](https://github.com/pnggroup/libpng.git). 

Libpng and OSS-Fuzz are written in C/C++. OSS-Fuzz supports fuzzing x86_64 and i386 builds.


## Project structure
This project structure has been edited to highlight the most important zones.
```
.
├── build/
├── docs/
├── help/
├── infra/
│   ├── helper.py/
├── libpng-1.6.18/   # vulnerable version of libpng, only on branch part 4
├── part1/
│   └── report/
│       ├── w_corpus/
│       └── w_o_corpus/
└── projects/
    └── libpng/
```

## Setup

### Clone Repository

#### Via HTTPS:
```bash
git clone https://github.com/johannayara/oss-fuzz.git
```

#### Via SSH:
```bash
git clone git@github.com:johannayara/oss-fuzz.git
```
---

### Requirements 
You need to ensure that you have docker installed. 

---
## Part 1 
This part provides scripts to run the libpng_read_fuzzer, with and without seeds. 
#### Example Usage with seeds
```bash
./run.w_o_corpus.sh [optional directory name]
```
#### Example Usage without seeds
```bash
./run.w_corpus.sh [optional directory name]
```

We have also provided a script that shows the coverage generated, to be able to run ; this script requires that the corpus be located in `/build/out` and that its name be passed to the script. This script will start a local server where the coverage can be seen. If no name is given it will look for our merged coverage. 
#### Example usage of coverage script
```bash
./show_coverage.sh [optional corpus directory name]
```
The other way to visualize coverage is to go to the `part1/report/w_corpus` or `part1/report/w_o_corpus` directories and run the html files. 
#### Example usage of html files
```bash
firefox index.html
```
### **Notes on diff files for part 1 **
In this part we have chosen to create three `.diff` files. Two of the files showcase the difference between our `build.sh` and libpng's original `build.sh`, these are used by our scripts to comment and uncomment seeds. 
The thrid `.diff` file, `oss_fuzz.diff` shows the difference between our repository and the original oss-fuzz repository. That is the changes we have brought to the `Dockerfile`. We have had to add a version specification on top of our modifications because a commit was made to the `libpng` repository that broke the fuzzer. 

## Part 3
### First improvement 


### Second improvement
For our second improvment, we chose to write a `libpng_write_fuzzer.c`, to be able to test the writing part of the `libpng` API. Our code as well as a dedicated `ReadMe` can be found on our [write_fuzzer](https://github.com/johannayara/oss-fuzz/tree/write_fuzzer) branch.

## Part 4 
Our code as well as a dedicated `ReadMe` can be found on our [part4](https://github.com/johannayara/oss-fuzz/tree/part4) branch. 