from VMTranslator import VMTranslator

# Create an instance of VMTranslator
translator = VMTranslator()

# Example usage for vm_push
tokens = ["push", "constant", "10"]  # Example tokens
print(translator.vm_push(tokens[1], int(tokens[2])))

# Example usage for vm_pop
tokens = ["pop", "local", "0"]  # Example tokens
print(translator.vm_pop(tokens[1], int(tokens[2])))
