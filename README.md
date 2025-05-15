# Part 3: Our improved libpng read fuzzer
This branch is dedicated to our improved libpng read fuzzer, its scripts and its results. 

## Branch Structure
```
.
├── CITATION.cff
├── CONTRIBUTING.md
├── docs/
├── infra/
│   ├── helper.py # helper file to build fuzzer
│   ...
├── LICENSE
├── part3/
│   └── improve1/
│       ├── coverage_improve1/ # our coverage report
│       │   ├── directory_view_index.html
│       │   ├── file_view_index.html
│       │   ├── index.html # main html file of our coverage report
│       │   ├── report.html
│       │   ├── src/
│       │   │   ├── libpng/
│       │   │   │   ├── contrib/
│       │   │   │   │   ├── oss-fuzz/
│       │   │   │   │   │   ├── libpng_my_read_fuzzer.cc.html
│       │   │   │   │   │   └── report.html
│       │   │   │   │   └── report.html
│       │   │   │   ├── png.c.html
│       │   │   │   ├── pngdebug.h.html
│       │   │   │   ├── pngerror.c.html
│       │   │   │   ├── pngget.c.html
│       │   │   │   ├── png.h.html
│       │   │   │   ├── pnglibconf.h.html
│       │   │   │   ├── pngmem.c.html
│       │   │   │   ├── pngpriv.h.html
│       │   │   │   ├── pngread.c.html
│       │   │   │   ├── pngrio.c.html
│       │   │   │   ├── pngrtran.c.html
│       │   │   │   ├── pngrutil.c.html
│       │   │   │   ├── pngset.c.html
│       │   │   │   ├── pngstruct.h.html
│       │   │   │   ├── pngtrans.c.html
│       │   │   │   └── report.html
│       │   │   └── report.html
│       │   └── summary.json
│       ├── oss-fuzz.diff # the difference between this branch and the oss-fuzz project
│       ├── project.diff # the difference between this branch and the libpng project
│       ├── run.improve1.sh # the mains script for your improved fuzzer
│       └── style.css
├── projects/
│   └── libpng/
│       ├── build.sh
│       ├── Dockerfile
│       ├── new_libpng_read_fuzzer.cc # our improved harness
│       └── project.yaml
├── README.md # this read me 
├── run.automated_fuzzer.sh # script to run 3x4 hours
├── show_coverage.sh # script to generate coverage report
└── tools/
```
## Requirements
Only Docker is required.

## Running the improvements
This section contains scripts to execute `new_libpng_read_fuzzer`, both for one or 3x 4 runs; with seed inputs and for generating and running a coverage report. All scripts are meant to be run from their own folder.

#### Example of usage for a 4 hour run
This will run for 4 hours, if not interrupted. This script can take an optional corpus directory name as an argument, a corresponding directory will be created under `build/out/`.Otherwise the corpus will be created as `build/out/new_read_corpus_4hours`.
```bash
./run.improve1.sh [optional corpus directory name]
```

We have also provided a script for viewing code coverage results. This script can take an optional corpus directory name as an argument. It launches a local server to display the coverage data. If no argument is given, it defaults to our merged coverage directory. 
#### Example usage of coverage script
```bash
./show_coverage.sh [optional corpus directory name]
```
Alternatively, you can view the coverage reports directly by opening the HTML files found under:
- `part3/improve1/coverage_improve1`

#### Example of opening HTML coverage report
```bash
firefox index.html
```

Lastly there also exists a `run.automated_fuzzer.sh` script to be able to run 3 x 4hours of our write_fuzzer. This script creates three corpuses in `build/out/`. 

#### Example of usage for a 12 hour run
This will run for 12 hours, if not interrupted. 
```bash
./run.automated_fuzzer.sh
```
