# Part 3: Our libpng write fuzzer
This branch is dedicated to our libpng write fuzzer, its scripts and its results. 

## Branch Structure
```
.
├── CITATION.cff
├── CONTRIBUTING.md
├── docs/
├── infra/
│   ├── helper.py
│   ...
├── LICENSE
├── part3/
│   └── improve2/ # our second imrovement results & scripts 
│       ├── coverage_improve2/ #html files to see our coverage report
│       │   ├── directory_view_index.html
│       │   ├── file_view_index.html
│       │   ├── index.html # run this file to get the start of the coverage report
│       │   ├── report.html
│       │   ├── src/
│       │   │   ├── libpng/
│       │   │   │   ├── contrib/
│       │   │   │   │   ├── oss-fuzz/
│       │   │   │   │   │   ├── libpng_write_fuzzer.cc.html
│       │   │   │   │   │   └── report.html
│       │   │   │   │   └── report.html
│       │   │   │   ├── png.c.html
│       │   │   │   ├── pngdebug.h.html
│       │   │   │   ├── pngerror.c.html
│       │   │   │   ├── pngget.c.html
│       │   │   │   ├── png.h.html
│       │   │   │   ├── pnglibconf.h.html
│       │   │   │   ├── pngmem.c.html
│       │   │   │   ├── pngpriv.h.html
│       │   │   │   ├── pngread.c.html
│       │   │   │   ├── pngrio.c.html
│       │   │   │   ├── pngrtran.c.html
│       │   │   │   ├── pngrutil.c.html
│       │   │   │   ├── pngset.c.html
│       │   │   │   ├── pngstruct.h.html
│       │   │   │   ├── pngtrans.c.html
│       │   │   │   ├── pngwio.c.html
│       │   │   │   ├── pngwrite.c.html
│       │   │   │   ├── pngwtran.c.html
│       │   │   │   ├── pngwutil.c.html
│       │   │   │   └── report.html
│       │   │   └── report.html
│       │   └── summary.json
│       ├── run.improve2.sh
│       └── style.css
├── projects/ 
│   └── libpng/ # directory with everything to run our fuzzer
│       ├── build.sh
│       ├── Dockerfile
│       ├── libpng_write_fuzzer.cc # our new write fuzzer
│       └── project.yaml
├── README.md
├── run_all_jobs.sh # script to run 3x4 hours
├── show_coverage.sh # script to generate coverage report
└── tools/
```
## Requirements
Only Docker is required.

## Running the improvements
This section contains scripts to execute `libpng_write_fuzzer`, both for one or 3x 4 runs, with seed inputs. If a corpus name is provided, a corresponding directory will be created under `build/out/`. Otherwise the corpus will be created as `build/out/write_corpus_4hours`.

#### Example of usage for a 4 hour run
This will run for 4 hours, if not interrupted. 
```bash
./run.improve2.sh 
```

We have also provided a script for viewing code coverage results. This script can take an optional corpus directory name as an argument. It launches a local server to display the coverage data. If no argument is given, it defaults to our merged coverage directory. 
#### Example usage of coverage script
```bash
./show_coverage.sh [optional corpus directory name]
```
Alternatively, you can view the coverage reports directly by opening the HTML files found under:
- `part2/improve2/covrage_improve2`

#### Example of opening HTML coverage report
```bash
firefox index.html
```

Lastly there also exists a `run_all_jobs.sh` script to be able to run 3 x 4hours of our write_fuzzer. This script creates three corpuses in `build/out/`. 

#### Example of usage for a 12 hour run
This will run for 12 hours, if not interrupted. 
```bash
./run_all_jobs.sh
```