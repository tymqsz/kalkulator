import operator

# Define the operations dictionary
operations = {
    '+': operator.add,
    '*': operator.mul,
    '/': operator.floordiv,
    '^': operator.pow,
    '%': operator.mod
}

def numberToBase(n, base):
    digits = "0123456789ABCDEF"
    if n == 0:
        return "0"
    result = ""
    while n > 0:
        result = digits[n % base] + result
        n //= base
    return result

def read_and_calculate(input_file, output_file):
    with open(input_file, "r") as infile, open(output_file, "w") as outfile:
        # Read and strip leading/trailing whitespace
        content = infile.read().strip()
        # Split the content by triple newlines to separate blocks
        blocks = content.split('\r\n\r\n\r\n')
        
        for block in blocks:
            # Remove extra whitespace and split into lines
            lines = [line.strip() for line in block.strip().splitlines() if line.strip()]
            
            # Ensure there are at least 3 lines for arithmetic operations

            try:
                # Read the operation and base from the first line
                oper_line = lines[0].strip()

                operation, base = oper_line.split()
                base = int(base)

                if operation in operations:
                    # Perform the arithmetic operation
                    num1 = lines[1].strip()
                    num2 = lines[2].strip()
                    num1 = int(num1, base)
                    num2 = int(num2, base)
                    result = operations[operation](num1, num2)
                    result = numberToBase(result, base)
                else:
                    # Convert between bases if an arithmetic operation isn't specified
                    base1 = int(operation)
                    base2 = base
                    num1 = lines[1].strip()
                    decimal = int(num1, base1)
                    result = numberToBase(decimal, base2)

                # Write the result to the output file
                outfile.write(block)
                outfile.write(f"{result}\n\n")

            except Exception as e:
                print(f"Error processing block: {e}")
                continue  # Skip the block if any error occurs

# Example usage (adjust paths as needed)
read_and_calculate("../source/input.txt", "../source/_py_output.txt")
