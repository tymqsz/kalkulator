import operator

operations = {
    '+': operator.add,
    '*': operator.mul,
    '/': operator.floordiv,
    '^': operator.pow,
    '%': operator.mod
}

INPUT = "../source/input.txt"
OUTPUT = "../source/py_output.txt"
out = open(OUTPUT, "w")

def numberToBase(n, base):
    digits = "0123456789ABCDEF"
    if n == 0:
        return "0"
    result = ""
    while n > 0:
        result = digits[n % base] + result
        n //= base
    return result

def change_bases(arg_cnt, args, base1, base2):
	if arg_cnt != 1:
		return ""
	
	int_value = int(args[0], base1)
	
	result = numberToBase(int_value, base2)
	
	return result

def calculate(arg_cnt, args, operator, base1):
	args = [int(arg, base1) for arg in args]
	result = operations[operator](args[0], args[1])

	i = 2
	while i < arg_cnt:
		result = operations[operator][result, args[i]]
	
	result = numberToBase(result, base1)
	return result

def oper_line(line):
	split = line.split(' ')

	if len(split) != 2:
		return 0
	
	if split[0].isdigit():
		return 2
	return 1

with open(INPUT, "r") as file:
	empty_cnt = 0
	args = []
	arg_cnt = 0
	base1, base2 = -1, -1
	operator = -1
	oper_type = -1
	arg_correct = 1

	
	for line in file.readlines():
		line = line.strip()
		
		if line == "":
			empty_cnt += 1
			
			if empty_cnt == 3:
				if oper_type == 1:
					output = change_bases(arg_cnt, args, base1, base2)
				elif oper_type == 0:
					output = calculate(arg_cnt, args, operator, base1)

				out.write(output+"\n\n")
				arg_cnt = 0
				args = []
		else:
			empty_cnt = 0

			if oper_line(line) == 1: # calc
				split = line.split()
				operator = split[0]
				base1 = int(split[1])
				oper_type = 0
			elif oper_line(line) == 2: # change
				split = line.split()
				base1 = int(split[0])
				base2 = int(split[1])
				oper_type = 1
			else:
				arg_cnt+=1
				args.append(line)

			out.write(line+"\n\n")

	if oper_type == 1:
		output = change_bases(arg_cnt, args, base1, base2)
	elif oper_type == 0:
		output = calculate(arg_cnt, args, operator, base1)

	out.write(output+"\n\n")
	arg_cnt = 0
	args = []

			



