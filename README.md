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
├── libpng-1.6.18/   # vulnerable version of libpng, only used on branch part4
├── part1/
│   ├── merged_w_corpus/ # merged corpuses of all 3 x 4 hours runs
│   │   └──src/ #inital corpus
│   └── report/
│       ├── add_seeds.diff 
│       ├── comment_seeds.diff
│       ├── oss_fuzz.diff
│       ├── run.w_corpus.sh
│       ├── run.w_o_corpus.sh
│       ├── w_corpus/
│       │   ├── directory_view_index.html
│       │   ├── file_view_index.html
│       │   ├── index.html
│       │   ├── report.html
│       │   ├── src/
│       │   │   ├── libpng/
│       │   │   │   ├── contrib/
│       │   │   │   │   ├── oss-fuzz/
│       │   │   │   │   │   ├── libpng_read_fuzzer.cc.html
│       │   │   │   │   │   └── report.html
│       │   │   │   │   └── report.html
│       │   │   │   ├── png.c.html
│       │   │   │   ├── pngdebug.h.html
│       │   │   │   ├── pngerror.c.html
│       │   │   │   ├── pngget.c.html
│       │   │   │   ├── png.h.html
│       │   │   │   ├── pnglibconf.h.html
│       │   │   │   ├── pngmem.c.html
│       │   │   │   ├── pngprefix.h.html
│       │   │   │   ├── pngpriv.h.html
│       │   │   │   ├── pngread.c.html
│       │   │   │   ├── pngrio.c.html
│       │   │   │   ├── pngrtran.c.html
│       │   │   │   ├── pngrutil.c.html
│       │   │   │   ├── pngset.c.html
│       │   │   │   ├── pngstruct.h.html
│       │   │   │   ├── pngtrans.c.html
│       │   │   │   └── report.html
│       │   │   └── report.html
│       │   ├── style.css
│       │   └── summary.json
│       └── w_o_corpus/
│           ├── directory_view_index.html
│           ├── file_view_index.html
│           ├── index.html
│           ├── report.html
│           ├── src/
│           │   ├── libpng/
│           │   │   ├── contrib/
│           │   │   │   ├── oss-fuzz/
│           │   │   │   │   ├── libpng_read_fuzzer.cc.html
│           │   │   │   │   └── report.html
│           │   │   │   └── report.html
│           │   │   ├── png.c.html
│           │   │   ├── pngdebug.h.html
│           │   │   ├── pngerror.c.html
│           │   │   ├── pngget.c.html
│           │   │   ├── png.h.html
│           │   │   ├── pnglibconf.h.html
│           │   │   ├── pngmem.c.html
│           │   │   ├── pngprefix.h.html
│           │   │   ├── pngpriv.h.html
│           │   │   ├── pngread.c.html
│           │   │   ├── pngrio.c.html
│           │   │   ├── pngrtran.c.html
│           │   │   ├── pngrutil.c.html
│           │   │   ├── pngset.c.html
│           │   │   ├── pngstruct.h.html
│           │   │   ├── pngtrans.c.html
│           │   │   └── report.html
│           │   └── report.html
│           ├── style.css
│           └── summary.json
├── part1/
│   └── coverage_noimprove/
│       ├── linux/
│       │   ├── directory_view_index.html
│       │   ├── file_view_index.html
│       │   ├── index.html
│       │   ├── report.html
│       │   ├── src/
│       │   │   ├── libpng/
│       │   │   │   ├── contrib/
│       │   │   │   │   ├── oss-fuzz/
│       │   │   │   │   │   ├── libpng_read_fuzzer.cc.html
│       │   │   │   │   │   └── report.html
│       │   │   │   │   └── report.html
│       │   │   │   ├── png.c.html
│       │   │   │   ├── pngdebug.h.html
│       │   │   │   ├── pngerror.c.html
│       │   │   │   ├── pngget.c.html
│       │   │   │   ├── png.h.html
│       │   │   │   ├── pnglibconf.h.html
│       │   │   │   ├── pngmem.c.html
│       │   │   │   ├── pngprefix.h.html
│       │   │   │   ├── pngpriv.h.html
│       │   │   │   ├── pngread.c.html
│       │   │   │   ├── pngrio.c.html
│       │   │   │   ├── pngrtran.c.html
│       │   │   │   ├── pngrutil.c.html
│       │   │   │   ├── pngset.c.html
│       │   │   │   ├── pngstruct.h.html
│       │   │   │   ├── pngtrans.c.html
│       │   │   │   └── report.html
│       │   │   └── report.html
│       │   └── summary.json
│       └── style.css
│   projects/
│   └── libpng/
│       ├── build.sh # our build.sh taken from libpng's repository
│       ├── Dockerfile # our dockerfile taken from oss-fuzzes' repository
│       └── project.yaml # project.yaml from oss-fuzzes' repository
├── README.md  # this file
├──show_coverage.sh  # script to show coverage using libpng_read_fuzzer
└──tools/

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

To run the scripts provided in this project, you must have Docker installed.
All scripts are meant to be run from their own folder.

---

## Part 1

This section contains scripts to execute `libpng_read_fuzzer`, both with and without seed inputs. If a corpus name is provided, a corresponding directory will be created under `build/out/`.

#### Example Usage with seeds
```bash
./run.w_o_corpus.sh [optional corpus directory name]
```
#### Example Usage without seeds
```bash
./run.w_corpus.sh [optional corpus directory name]
```

We have also provided a script for viewing code coverage results. This script can take an optional corpus directory name as an argument. It launches a local server to display the coverage data. If no argument is given, it defaults to our merged coverage directory. 
#### Example usage of coverage script
```bash
./show_coverage.sh [optional corpus directory name]
```
Alternatively, you can view the coverage reports directly by opening the HTML files found under:
- `part1/report/w_corpus`
- `part1/report/w_o_corpus`

#### Example of opening HTML coverage report
```bash
firefox index.html
```

### Notes on `.diff` Files (Part 1)

We created three `.diff` files to document modifications:

1. **Two diff files** compare our `build.sh` script against the original from the `libpng` project. These diffs are used by our scripts to toggle seed input (i.e., comment/uncomment related lines).

2. **The third diff**, `oss_fuzz.diff`, shows the differences between our repository and the original OSS-Fuzz repository. It primarily documents our changes to the `Dockerfile`.

> 📌 **Note:** We added a version pin in the Dockerfile to prevent a recent `libpng` commit—which broke the fuzzer—from affecting our build.

## Part 3
### First improvement 


### Second improvement
For our second improvment, we chose to write a `libpng_write_fuzzer.c`, to be able to test the writing part of the `libpng` API. Our code as well as a dedicated `ReadMe` can be found on our [write_fuzzer](https://github.com/johannayara/oss-fuzz/tree/write_fuzzer) branch.

## Part 4 
Our code as well as a dedicated `ReadMe` can be found on our [part4](https://github.com/johannayara/oss-fuzz/tree/part4) branch. 
