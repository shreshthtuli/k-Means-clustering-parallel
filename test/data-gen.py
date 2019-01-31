import random
from sys import argv

num_points = int(argv[2])
k = int(argv[1])
filename = "./data/"+str(k)+"-"+str(num_points)+".dat"

clustered = 1

f = open(filename, "w")

f.write(str(num_points)+"\n")

if clustered == 0:
    for i in range(num_points):
        f.write(str(random.randint(0, 100))+" "+str(random.randint(0, 100))+" "+str(random.randint(0, 100))+"\n")
else:
	for i in range(k):
		x = random.randint(-1000, +1000)
		y = random.randint(-1000, +1000)
		z = random.randint(-1000, +1000)
		print x, y, z
		for j in range(num_points / k):
  			f.write(str(x+random.randint(-50, 50))+" "+str(y+random.randint(-50, 50))+" "+str(z+random.randint(-50, 50))+"\n")


f.close()