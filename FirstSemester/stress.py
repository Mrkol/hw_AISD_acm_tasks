import sys, time, subprocess, random

total = 1
success = 0
maxsize = 30
maxvalue = 10

for i in range(total):
	
	n = random.randrange(1, maxsize)
	L = random.randrange(1, n + 1)

	a = [[0 for x in range(n)] for y in range(n)] 
	str0 = str(n) + " " + str(L) + " "

	for j in range(n):
		for k in range(n):
			a[j][k] = random.randrange(-maxvalue, maxvalue)
			str0 = str0 + str(a[j][k]) + " "

	proc1 = subprocess.Popen(['./T'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	str1 = proc1.communicate(input=str0)[0]

	proc2 = subprocess.Popen(['./T_stupid'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	str2 = proc2.communicate(input=str0)[0]
	
	if str1 == str2:
		success = success + 1
	#else:
	print(str(n) + " " + str(L) + ":")
	for j in range(n):
		for k in range(n):
			print str(a[j][k]),
		print

print(str(success) + "/" + str(total));