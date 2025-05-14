# Part 4 : CVE-2015-8126 buffer overflow
Since our improvements did not trigger a crash, we chose to triage a bug. In this branch you will find a script to be able to trigger this bug. 

## Branch structure 
```
.
├── build/
├── docs/
├── help
├── infra/
├── libpng-1.6.18/ # the vulnerable version of libpng
├── libpng-1.6.18.tar.gz
├── LICENSE
├── Makefile
├── part1/
├── poc.c # the code that trigger the bug 
├── projects/
├── README.md
├── run_poc.sh # script to run our PoC
└── tools/
```

## Example of code usage 
This script will run our PoC, if `leak_mem` is given to the script, `png_get_PLTE` will be called after `png_set_PLTE` in order to print all the `num_palette` elements. Otherwise ASAN will be used and the script will crash. The output of the script is always stored in the `out.txt`file. The script will also clean everything it built. 
```bash
./run_poc.sh [leak_mem]
```