import random

filename = "points.dat"
num_points = 100

f = open(filename, "w")

for i in range(num_points):
    f.write(str(random.randint(0, 100))+" "+str(random.randint(0, 100))+" "+str(random.randint(0, 100))+"\n")

f.close()