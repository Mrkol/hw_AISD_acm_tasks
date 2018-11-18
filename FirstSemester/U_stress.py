import sys, time, subprocess, random

total = 10
success = 0
maxvalue = 10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

for i in range(total):
	
	A = random.randrange(-maxvalue, maxvalue)
	B = random.randrange(-maxvalue, maxvalue)

	str0 = str(A) + " " + str(B)

	proc1 = subprocess.Popen(['./U'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	str1 = proc1.communicate(input=str0)[0]
	str2 = str(A*B)
	if str2 in str1:
		success = success + 1
	else:
		print(str(A) + " * " + str(B))
		print("Expected: " + str2)
		print("Got: " + str1)

print(str(success) + "/" + str(total));
