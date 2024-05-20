class VMTranslator:
    def __init__(self):
        self.file_name = ""

    def set_file_name(self, file_name):
        self.file_name = file_name

    def vm_push(self, segment, index):
        if segment == "constant":
            return f"@{index}\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
        elif segment in ["local", "argument", "this", "that"]:
            segment_pointer = {"local": "LCL", "argument": "ARG", "this": "THIS", "that": "THAT"}[segment]
            return f"@{index}\nD=A\n@{segment_pointer}\nA=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
        elif segment == "pointer":
            return f"@{'THIS' if index == 0 else 'THAT'}\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
        elif segment == "temp":
            return f"@{5 + index}\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
        elif segment == "static":
            return f"@{self.file_name}.{index}\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
        else:
            raise ValueError("Invalid segment for push command")

    def vm_pop(self, segment, index):
        if segment in ["local", "argument", "this", "that"]:
            segment_pointer = {"local": "LCL", "argument": "ARG", "this": "THIS", "that": "THAT"}[segment]
            return f"@{index}\nD=A\n@{segment_pointer}\nD=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n"
        elif segment == "pointer":
            return f"@SP\nAM=M-1\nD=M\n@{'THIS' if index == 0 else 'THAT'}\nM=D\n"
        elif segment == "temp":
            return f"@SP\nAM=M-1\nD=M\n@{5 + index}\nM=D\n"
        elif segment == "static":
            return f"@SP\nAM=M-1\nD=M\n@{self.file_name}.{index}\nM=D\n"
        else:
            raise ValueError("Invalid segment for pop command")
