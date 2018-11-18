import random
import subprocess
from datetime import datetime

random.seed(datetime.now())


tests = 10
maxsize = 10
maxrequests = 10


successes = 0

for i in range(0, tests):
	succ = True

	size = random.randint(2, maxsize)

	degrees = [0 for i in range(0, size)]
	edges = [[False for j in range(0, size)] for i in range(0, size)]
	edge_count = 0

	teststr = ""

	teststr += str(size) + "\n"
	teststr += "0\n"

	proc1 = subprocess.Popen(['./D'], 
		stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	proc2 = subprocess.Popen(['./D_lyoha'], 
		stdout=subprocess.PIPE, stdin=subprocess.PIPE)

	request_count = random.randint(1, maxrequests)
	teststr += str(request_count) + "\n"

	reqs = 0
	while reqs < request_count:
		op = random.randint(1, 3)
		if edge_count == 0:
			op = 1

		opstr = ""

		l = random.randint(1, size)
		r = random.randint(1, size)
		if op == 1: # +
			opstr += "+ " + str(l) + " " + str(r)
			if edges[l - 1][r - 1]:
				continue
			if degrees[l - 1] == 2 or degrees[r - 1] == 2:
				continue
			if l == r:
				continue
			degrees[l - 1] += 1
			degrees[r - 1] += 1
			edges[l - 1][r - 1] = True
			edge_count += 1
		elif op == 2: # -
			opstr += "- " + str(l) + " " + str(r)
			if not edges[l - 1][r - 1]:
				continue
			degrees[l - 1] -= 1
			degrees[r - 1] -= 1
			edges[l - 1][r - 1] = False
			edge_count -= 1
		elif op == 3: # ?
			opstr += "? " + str(l) + " " + str(r)

		opstr += "\n"

		teststr += opstr


		reqs += 1


	proc1.stdin.write(bytes(teststr, "utf-8"))
	proc2.stdin.write(bytes(teststr, "utf-8"))

	proc1.stdin.close()
	proc2.stdin.close()

	str1 = proc1.stdout.read().decode("utf-8")
	str2 = proc2.stdout.read().decode("utf-8")

	if str1 != str2:
		print("FAIL")
		print(teststr)
		print("GOT")
		print(str1)
		print("AND")
		print(str2)
		succ = False

	if succ:
		successes += 1


print(str(successes) + "/" + str(tests))
