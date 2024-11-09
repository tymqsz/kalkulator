import random
import sys

sys.set_int_max_str_digits(1000000000)

OPER_CNT = 1000        
MAX_NUM = int("1"+"0"*100)
opers = ['*', '+', '/', '^', '%', 'c']
sizes = [10**x for x in range(40)]

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


with open("../assets/input.txt", "w") as f:
	for _ in range(OPER_CNT):
		operation = random.choice(opers)

		size1 = random.choice(sizes)
		size2 = random.choice(sizes)
		
		base = random.randint(2, 16)

		num1 = random.randint(1, size1)
		num2 = random.randint(1, size2)
		

		if operation == '^':
			num1 = num1 % 200
			num2 = num2 % 50
		
		if operation == 'c':
			base2 = random.randint(2, 16)
			base1 = random.randint(2, 16)
			num1 = numberToBase(num1, base1)
			f.write(f"{base1} {base2}\n\n{num1}\n\n\n\n")
		else:
			num1 = numberToBase(num1, base)
			num2 = numberToBase(num2, base)
			f.write(f"{operation} {base}\n\n{num1}\n\n{num2}\n\n\n\n")
