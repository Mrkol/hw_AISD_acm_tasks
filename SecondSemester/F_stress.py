import random
import subprocess
from datetime import datetime

random.seed(datetime.now())


tests = 100
maxsize = 10
maxvalue = 10
maxrequests = 5

for i in range(0, tests):
	succ = True
	size = random.randint(1, maxsize)
	startdata = [random.randint(-maxvalue, maxvalue) for i in range(0, size)]

	teststr = ""

	teststr += str(size) + "\n"
	teststr += ' '.join(map(str, startdata)) + "\n"

	proc1 = subprocess.Popen(['./F'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	proc2 = subprocess.Popen(['./F_stupid'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)

	request_count = random.randint(1, maxrequests)
	teststr += str(request_count) + "\n"

	proc1.stdin.write(bytes(teststr, "utf-8"))
	proc2.stdin.write(bytes(teststr, "utf-8"))

	for a in range(0, request_count):
		op = random.randint(1, 7)
		if size == 0:
			op = 2

		opstr = str(op) + " "

		if op == 1 or op == 6 or op == 7:
			l = random.randrange(0, size)
			r = random.randrange(l, size)
			opstr += "{0} {1}".format(l, r)
		elif op == 2:
			x = random.randint(-maxvalue, maxvalue)
			pos = random.randrange(0, size + 1)
			opstr += "{0} {1}".format(x, pos)
			size = size + 1
		elif op == 3:
			pos = random.randrange(0, size)
			size = size - 1
			opstr += str(pos)
		elif op == 4 or op == 5:
			x = random.randint(-maxvalue, maxvalue)
			l = random.randrange(0, size)
			r = random.randrange(l, size)
			opstr += "{0} {1} {2}".format(x, l, r)

		opstr += "\n"

		teststr += opstr

		proc1.stdin.write(bytes(opstr, "utf-8"))
		proc2.stdin.write(bytes(opstr, "utf-8"))



	proc1.stdin.close()
	proc2.stdin.close()

	str1 = proc1.stdout.read().decode("utf-8")
	str2 = proc2.stdout.read().decode("utf-8")

	if str1 != str2:
		succ = False
		print("NO MATCH!")
		print(teststr)
		print("RECEIVED")
		print(str1)
		print("vs")
		print(str2)



