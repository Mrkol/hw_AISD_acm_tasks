import sys, time, subprocess, random

total = 100
success = 0
maxvalue = 10**(10**2);

for i in range(total):
	A = int("{0:b}".format(random.randrange(0, maxvalue)))

	proc1 = subprocess.Popen(['./V'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
	str1 = proc1.communicate(input=str(A))[0]
	answ = 0
	for digit in str(A*101):
		if digit == '2':
			answ = answ + 1
	str2 = str(answ)

	if str2 in str1:
		success = success + 1
	else:
		print(str(A))
		print("Expected: " + str2)
		print("Got: " + str1)

print(str(success) + "/" + str(total));
