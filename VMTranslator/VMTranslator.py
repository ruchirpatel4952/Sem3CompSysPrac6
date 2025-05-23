# inside VMTranslator.py  (adjust names / placement to your codebase)

SEGMENT_SYMBOLS = {
    "local":     "LCL",
    "argument":  "ARG",
    "this":      "THIS",
    "that":      "THAT",
}

TEMP_BASE    = 5
POINTER_BASE = 3      # 0 -> THIS, 1 -> THAT

def vm_push(self, segment: str, index: int, file_name: str) -> list[str]:
    """
    Return Hack assembly for the VM command   push segment index
    """
    asm = []

    if segment == "constant":
        # D = index
        asm += [f"@{index}", "D=A"]

    elif segment in SEGMENT_SYMBOLS:
        # D = *(segmentBase + index)
        base = SEGMENT_SYMBOLS[segment]
        asm += [
            f"@{base}",   "D=M",        # D = base address
            f"@{index}",  "A=D+A",      # A = base + index
            "D=M"                      # D = *addr
        ]

    elif segment == "temp":
        asm += [
            f"@{TEMP_BASE + index}", "D=M"
        ]

    elif segment == "pointer":
        addr = POINTER_BASE + index          # 3 or 4
        asm += [f"@{addr}", "D=M"]

    elif segment == "static":
        asm += [f"@{file_name}.{index}", "D=M"]

    else:
        raise ValueError(f"Unknown segment {segment}")

    # *SP = D ;  SP++
    asm += ["@SP", "A=M", "M=D", "@SP", "M=M+1"]
    return asm
# --------------------------------------------------------------------

def vm_pop(self, segment: str, index: int, file_name: str) -> list[str]:
    """
    Return Hack assembly for the VM command   pop segment index
    """
    asm = []

    if segment in SEGMENT_SYMBOLS:
        base = SEGMENT_SYMBOLS[segment]
        # Compute target address into R13
        asm += [
            f"@{base}",  "D=M",
            f"@{index}", "D=D+A",
            "@R13",      "M=D"
        ]
        # *SP-- ; D = *SP
        asm += ["@SP", "AM=M-1", "D=M"]
        # *(R13) = D
        asm += ["@R13", "A=M", "M=D"]

    elif segment == "temp":
        asm += ["@SP", "AM=M-1", "D=M", f"@{TEMP_BASE + index}", "M=D"]

    elif segment == "pointer":
        addr = POINTER_BASE + index          # 3 or 4
        asm += ["@SP", "AM=M-1", "D=M", f"@{addr}", "M=D"]

    elif segment == "static":
        asm += ["@SP", "AM=M-1", "D=M", f"@{file_name}.{index}", "M=D"]

    else:
        raise ValueError(f"Unknown segment {segment}")

    return asm
