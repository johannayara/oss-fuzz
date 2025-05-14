# Part 3: Our libpng write fuzzer
## Branch Structure
```
.
├── CITATION.cff
├── CONTRIBUTING.md
├── LICENSE
├── README.md
├── docs/
├── infra/
├── part1/
├── part3 #directory of this part
│   └── libpng_write_fuzzer 
│       ├── linux
│       │   ├── directory_view_index.html
│       │   ├── file_view_index.html
│       │   ├── index.html
│       │   ├── report.html
│       │   ├── src
│       │   │   ├── libpng
│       │   │   │   ├── contrib
│       │   │   │   │   ├── oss-fuzz
│       │   │   │   │   │   ├── libpng_write_fuzzer.cc.html
│       │   │   │   │   │   └── report.html
│       │   │   │   │   └── report.html
│       │   │   │   ├── png.c.html
│       │   │   │   ├── png.h.html
│       │   │   │   ├── pngdebug.h.html
│       │   │   │   ├── pngerror.c.html
│       │   │   │   ├── pngget.c.html
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
│       │   │   │   ├── pngwio.c.html
│       │   │   │   ├── pngwrite.c.html
│       │   │   │   ├── pngwtran.c.html
│       │   │   │   ├── pngwutil.c.html
│       │   │   │   └── report.html
│       │   │   └── report.html
│       │   └── summary.json
│       └── style.css
├── projects
│   └── libpng
│       ├── Dockerfile
│       ├── build.sh
│       ├── libpng_write_fuzzer.cc # the write fuzzer
│       └── project.yaml
├── run_improve2.sh
├── show_coverage.sh
└── tools/
```
## Requirements
Only Docker is required.

## Example of code usage
`run_improve2.sh` builds the Docker image and our fuzzer in build/out/write_corpus_4h. This will run for 4 hours.
`show_coverage.sh` builds fuzzer with coverage instrumentation and generates coverage report

```bash
./run_improve2.sh 
./show_coverage.sh 
```
