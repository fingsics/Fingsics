import csv
import pathlib
import os

def get_csv_values(file):
    npcd_tests = []
    collisions = []

    with open(file, newline='') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=',')
        
        first_row = True
        for row in csv_reader:
            if first_row:
                first_row = False
                continue
            npcd_tests.append(int(row[1]))
            collisions.append(int(row[3]))
            
    return npcd_tests, collisions

def evaluate_test(filename, npcd_tests_1, collisions_1, npcd_tests_2, collisions_2):
    equal_npcd_tests = True
    equal_collisions = True
    for i in range(0, min(len(npcd_tests_1), len(npcd_tests_2))):
        if npcd_tests_1[i] != npcd_tests_2[i]:
            equal_npcd_tests = False
        if collisions_1[i] != collisions_2[i]:
            equal_collisions = False
    
    print("Results for:   " + filename)
    print("Same collisions: " + str(equal_collisions))
    print("Same NPCD tests: " + str(equal_npcd_tests))
    print()
            
        
expected_results_folder = str(pathlib.Path().resolve()) + "\\testing\\expected-results\\"
results_folder = str(pathlib.Path().resolve()) + "\\testing\\results\\"
files = os.listdir(expected_results_folder)

for file in files:
    npcd_tests_1, collisions_1 = get_csv_values(expected_results_folder + file)
    npcd_tests_2, collisions_2 = get_csv_values(results_folder + file)
    evaluate_test(file, npcd_tests_1, collisions_1, npcd_tests_2, collisions_2)
