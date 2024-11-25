import random
import sys

sys.set_int_max_str_digits(1000000000)

OPER_CNT = 10  
opers = ['*', '+', '/', '^', '%', 'c']
sizes = [10**x for x in range(1000)]

def numberToBase(n, base):
    if not (1 <= base <= 16):
        raise ValueError("Base must be between 1 and 16")

    if base == 1:
        return '1' * n

    digits = "0123456789ABCDEF"
    result = ""
    
    if n == 0:
        return "0"

    while n > 0:
        result = digits[n % base] + result
        n //= base

    return result


with open("../source/input.txt", "w") as f:
	for i in range(OPER_CNT):
		operation = random.choice(opers)

		size1 = random.choice(sizes)
		size2 = random.choice(sizes)
		
		base = random.randint(2, 16)
		if operation == 'c':
			base2 = random.randint(2, 16)
			base1 = random.randint(2, 16)
			f.write(f"{base1} {base2}\r\n\r\n")
		else:
			f.write(f"{operation} {base}\r\n\r\n")
		
		n_args = [1, 3, 4]
		ARG_CNT = random.choice(n_args) 
		for j in range(ARG_CNT):
			size1 = random.choice(sizes)
			size2 = random.choice(sizes)
			num1 = random.randint(1, size1)
			num2 = random.randint(1, size2)
			

			if operation == '^':
				num1 = num1 % 1000
				num2 = num2 % 1000
			if operation == 'c':
				num1 = numberToBase(num1, base1)
				if i== OPER_CNT-1 and j==ARG_CNT-1:
					f.write(f"{num1}\r\n\r\n")
				else:
					f.write(f"{num1}\r\n\r\n\r\n\r\n")
			else:
				num1 = numberToBase(num1, base)
				num2 = numberToBase(num2, base)
				if i== OPER_CNT-1 and j==ARG_CNT-1:
					f.write(f"{num1}\r\n\r\n{num2}\r\n\r\n")
				else:
					f.write(f"{num1}\r\n\r\n{num2}\r\n\r\n\r\n\r\n")


