#!/bin/bash

# === CONFIG ===
INPUT_PNG="template.png"

# === RUN PNGCRUSH COMMAND ===
echo "[*] Running pngcrush..."
# pngcrush -bkgd 255 20 147 "$INPUT_PNG" "test_bkgd.png"

# pngcrush -gamma 55555 "$INPUT_PNG" "test_gama.png" # set gamma value of 1.8 

# pngcrush -res 2834 2834 "$INPUT_PNG" "test_pHYS.png" # set resolution to 72 Dots Per Inch or 2834 pixels/meter vertically and horizontally 

# pngcrush -srgb 0 "$INPUT_PNG" "test_srgb_0.png"
# pngcrush -srgb 1 "$INPUT_PNG" "test_srgb_1.png"
# pngcrush -srgb 2 "$INPUT_PNG" "test_srgb_2.png"
# pngcrush -srgb 3 "$INPUT_PNG" "test_srgb_3.png"

# pngcrush -text a "Comment" "hello :)" "$INPUT_PNG" "test_tEXT.png"

# pngcrush -ztxt a "Comment" "compressed hello :)" "$INPUT_PNG" "test_ztxt.png"

pngcrush -time 2025 05 06 12 00 00 "$INPUT_PNG" "test_tIME.png"

pngcrush -trns 255 0 255 "$INPUT_PNG" "test_tRNS.png"


echo "[✓] PNG written to: $OUTPUT_DIR/$OUTPUT_PNG"
