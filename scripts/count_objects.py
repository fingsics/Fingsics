import re
import pathlib
import os

def count_objects(file):
    f = open(file, "r")
    contents = f.read()
    f.close()
    
    num_spheres = len(re.findall(r'<sphere', contents))
    num_capsules = len(re.findall(r'<capsule', contents))
    num_planes = len(re.findall(r'<plane', contents))
            
    return num_spheres, num_capsules, num_planes
            
        
scene_folder = str(pathlib.Path().resolve()) + "\\scenes\\"
files = os.listdir(scene_folder)

for file in files:
    num_spheres, num_capsules, num_planes = count_objects(scene_folder + file)
    print(file)
    print("  Balls: " + str(num_spheres))
    print("  Capsules: " + str(num_capsules))
    print("  Planes : " + str(num_planes))
    print()
