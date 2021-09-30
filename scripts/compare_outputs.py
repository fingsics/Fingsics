import csv
import pathlib
import sys

folder= str(pathlib.Path().resolve()) + "\\output\\"

file1 = "sap.csv"
file2 = "sapobb.csv"

def get_csv_values(file):
    bpcd_times = []
    npcd_tests = []
    npcd_times = []
    collisions = []
    total_times = []

    with open(file, newline='') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=',')
        
        first_row = True
        for row in csv_reader:
            if first_row:
                first_row = False
                continue
            bpcd_times.append(float(row[0]))
            npcd_tests.append(int(row[1]))
            npcd_times.append(float(row[2]))
            collisions.append(int(row[3]))
            total_times.append(float(row[6]))
            
    return bpcd_times, npcd_tests, npcd_times, collisions, total_times



bpcd_times_1, npcd_tests_1, npcd_times_1, collisions_1, total_times_1 = get_csv_values(folder + file1)
bpcd_times_2, npcd_tests_2, npcd_times_2, collisions_2, total_times_2 = get_csv_values(folder + file2)

first_difference = -1
equal_npcd_tests = True
equal_collisions = True

bpcd_time_1 = 0
npcd_time_1 = 0
total_time_1 = 0

bpcd_time_2 = 0
npcd_time_2 = 0
total_time_2 = 0


for i in range(0, min(len(bpcd_times_1), len(bpcd_times_2))):
    if npcd_tests_1[i] != npcd_tests_2[i]:
        if first_difference < 0:
            first_difference = i
        equal_npcd_tests = False
    if collisions_1[i] != collisions_2[i]:
        if first_difference < 0:
            first_difference = i
        equal_collisions = False
        
    bpcd_time_1 += bpcd_times_1[i]
    npcd_time_1 += npcd_times_1[i]
    total_time_1 += total_times_1[i]

    bpcd_time_2 += bpcd_times_2[i]
    npcd_time_2 += npcd_times_2[i]
    total_time_2 += total_times_2[i]
        

print("Same NPCD tests: " + str(equal_npcd_tests))
print("Same collisions: " + str(equal_collisions))
print()
print("BPCD time 1: " + str(bpcd_time_1))
print("NPCD time 1: " + str(npcd_time_1))
print("Total time 1: " + str(total_time_1))
print()
print("BPCD time 2: " + str(bpcd_time_2))
print("NPCD time 2: " + str(npcd_time_2))
print("Total time 2: " + str(total_time_2))

if first_difference >= 0:
    print()
    print("First different frame: " + str(first_difference))