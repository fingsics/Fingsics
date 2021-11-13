import pathlib
import os
import random

# Aux

def delete_random_elems(list, n):
    removed_indexes = set(random.sample(range(len(list)), n))
    return [x for i,x in enumerate(list) if not i in removed_indexes]

def get_random_velocity():
    posible_velocities = [i/6 for i in range(-30,31,1)]
    return f"{random.choice(posible_velocities)},{random.choice(posible_velocities)},{random.choice(posible_velocities)}"
    
def get_random_color():
    posible_color_values = [i for i in range(0,256,1)]
    return f"{random.choice(posible_color_values)},{random.choice(posible_color_values)},{random.choice(posible_color_values)}"

# Objects

def create_capsule_vertical(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"90,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />"
    
def create_capsule_horizontal(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />"
 
def create_ball(pos, mass, rad, vel, color):
    return f"\t\t<sphere pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" />"
    
def create_plane(pos, ang, color, drawLength, drawWidth):
    return f"\t\t<plane pos=\"{pos}\" mass=\"{1}\" elasticityCoef=\"1\" ang=\"{ang}\" color=\"{color}\" static=\"true\" drawLength=\"{drawLength}\" drawWidth=\"{drawWidth}\" />"

def create_tile(pos, ang, color, length, width, invisible = False):
    invisibleString = "draw=\"false\"" if invisible else ""
    return f"\t\t<tile pos=\"{pos}\" mass=\"{1}\" elasticityCoef=\"1\" ang=\"{ang}\" color=\"{color}\" static=\"true\" length=\"{length}\" width=\"{width}\" {invisibleString} />"

# Scenes

def make_capsule_cube_horizontal(num_x, num_y, num_z, dist_1, dist_2):
    objects = []
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                objects.append(create_capsule_horizontal(f"{-12 + dist_1 * i},{-15 + dist_2 * j},{-50 + dist_1 * k}", "1", "0.5", "4", "0,0,15", "220,0,0"))
    return objects

def make_capsule_cube_vertical(num_x, num_y, num_z, dist_1, dist_2, radius=0.5, length=4, randomVel=False):
    objects = []
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                objects.append(create_capsule_vertical(f"{(dist_2 + 2 * radius) * (i - num_x / 2)},{(dist_1 + length + 2 * radius) * (j - num_y / 2)},{(dist_2 + 2 * radius) * (k - num_z / 2)}", "1", f"{radius}", f"{length}", get_random_velocity() if randomVel else "0,0,0", "0,220,0"))
    return objects

def make_ball_cube(num_x, num_y, num_z, dist, rad, use_velocities, use_random_colors):
    possible_disturbances = [i/100 for i in range(-10,11,1)]
    objects = []
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                if 1 == random.choice([1]):
                    random_disturbance_1 = random.choice(possible_disturbances)
                    random_disturbance_2 = 0
                    random_disturbance_3 = random.choice(possible_disturbances)
                    vel = get_random_velocity() if use_velocities else "0,0,0"
                    color = get_random_color() if use_random_colors else "220,0,0"
                    objects.append(create_ball(f"{-((num_x - 1) * dist / 2) + dist * i + random_disturbance_1},{-((num_y - 1) * dist / 2) + dist * j + random_disturbance_2},{0.4 + dist * k + random_disturbance_3}", "1", f"{rad}", f"{vel}", color))
    return objects
    
def make_mixed_cube(num_x, num_y, num_z, dist, rad, len, use_velocities, use_random_colors):
    possible_disturbances = [i/100 for i in range(-10,11,1)]
    objects = []
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                if 1 == random.choice([1]):
                    random_disturbance_1 = random.choice(possible_disturbances)
                    random_disturbance_2 = random.choice(possible_disturbances)
                    random_disturbance_3 = random.choice(possible_disturbances)
                    vel = get_random_velocity() if use_velocities else "0,0,0"
                    color = get_random_color() if use_random_colors else "220,0,0"
                    pos = f"{-((num_x - 1) * dist / 2) + dist * i + random_disturbance_1},{-((num_y - 1) * (dist + len / 2) / 2) + (dist + len / 2) * j + random_disturbance_2},{-((num_z - 1) * dist / 2) + dist * k + random_disturbance_3}"
                    if ((i + j + k) % 2 == 0):
                        objects.append(create_ball(f"{pos}", "1", f"{rad}", f"{vel}", color))
                    else:
                        objects.append(create_capsule_vertical(f"{pos}", f"{(len / rad + 1)}", f"{rad}", f"{len}", f"{vel}", color))
    return objects


def make_two_waves_scene():
    num_x = 500
    num_y = 1
    num_z = 500
    dist = 0.38

    objects = make_ball_cube(num_x, num_y, num_z, dist, 0.1, False, False)

    objects.append(create_ball("13,0,-30", "25", "6", "0,0,20", "220,220,220"))
    objects.append(create_ball("-13,0,-30", "25", "6", "0,0,20", "220,220,220"))

    plane_length_x = num_x * dist + 0.5
    plane_length_z = num_z * dist + 0.5

    
    mid_rect_width_div = 4
    rect_width_div = 4.5
    
    objects.append(create_tile("25,0,0", "90,0,0", "220,220,220", "14.33", "20"))
    objects.append(create_tile("-25,0,0", "90,0,0", "220,220,220", "14.33", "20"))
    objects.append(create_tile("0,0,0", "90,0,0", "220,220,220", "16.125", "20"))
               
    return objects

def make_balls_in_box_scene():
    num_x = 18
    num_y = 18
    num_z = 19
    dist = 1

    objects = make_ball_cube(num_x, num_y, num_z, dist, 0.3, True, True)

    plane_length_x = num_x * dist + 0.5
    plane_length_y = num_y * dist + 0.5
    plane_length_z = num_z * dist + 0.5
 
    objects.append(create_plane(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}"))
    objects.append(create_plane(f"-{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{0}", f"{0}"))
    
    objects.append(create_plane(f"0,0,{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    objects.append(create_plane(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    
    objects.append(create_plane(f"0,{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{0}", f"{0}"))
    objects.append(create_plane(f"0,-{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}"))
    return objects
    
def make_mixed_box_scene():
    final_count = 100000
    num_x = 85
    num_y = 42
    num_z = 85
    distance = 1
    radius = 0.3
    length = 2
    
    print(num_x * num_y * num_z)

    objects = make_mixed_cube(num_x, num_y, num_z, distance, radius, length,  True, True)
    
    filtered_objects = delete_random_elems(objects, num_x * num_y * num_z - final_count)

    plane_length_x = num_x * distance + 0.5
    plane_length_y = num_y * length * distance + length / 2 + 0.5
    plane_length_z = num_z * distance + 0.5

    filtered_objects.append(create_tile(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}", True))
    filtered_objects.append(create_tile(f"-{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}"))
    
    filtered_objects.append(create_tile(f"0,0,{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}", True))
    filtered_objects.append(create_tile(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    
    filtered_objects.append(create_tile(f"0,{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}", True))
    filtered_objects.append(create_tile(f"0,-{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}"))
    
    return filtered_objects

def make_capsules_in_box_scene():
    final_count = 5000
    num_x = 48
    num_y = 24
    num_z = 48
    distance = 1
    radius = 0.5
    length = 15

    objects = make_capsule_cube_vertical(num_x, num_y, num_z, distance, distance, radius, length, True)
    
    filtered_objects = delete_random_elems(objects, num_x * num_y * num_z - final_count)

    plane_length_x = (num_x + 2) * (2 * radius + distance)
    plane_length_y = (num_y + 2) * (2 * radius + length + distance)
    plane_length_z = (num_z + 2) * (2 * radius + distance)

    filtered_objects.append(create_tile(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}", True))
    filtered_objects.append(create_tile(f"-{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}"))
    
    filtered_objects.append(create_tile(f"0,0,{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}", True))
    filtered_objects.append(create_tile(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    
    filtered_objects.append(create_tile(f"0,{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}", True))
    filtered_objects.append(create_tile(f"0,-{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}"))
    
    return filtered_objects
    

def make_confetti_scene():
    objects = ""
    num_x = 18
    num_y = 10
    num_z = 19
    dist = 1
    rad = 0.3
    len = 2
    
    def create_capsule_vertical(pos, mass, rad, length, vel, color):
        return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"90,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" acceleration=\"0,-9.8,0\" />\n"
        
    def create_ball(pos, mass, rad, vel, color):
        return f"\t\t<sphere pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" acceleration=\"0,-9.8,0\" />\n"

    def get_random_velocity(i):
        posible_velocities = [i/6 for i in range(-30,31,1)]
        return f"{random.choice(posible_velocities)},{random.choice(posible_velocities) + 25 + (num_y / 15) * i},{random.choice(posible_velocities)}"
    possible_disturbances = [i/100 for i in range(-10,11,1)]
    objects = ""
    
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                if 1 == random.choice([1]):
                    random_disturbance_1 = random.choice(possible_disturbances)
                    random_disturbance_2 = random.choice(possible_disturbances)
                    random_disturbance_3 = random.choice(possible_disturbances)
                    vel = get_random_velocity(k)
                    color = get_random_color()
                    pos = f"{-((num_x - 1) * dist / 2) + dist * i + random_disturbance_1},{-((num_y - 1) * (dist + len / 2) / 2) + (dist + len / 2) * j + random_disturbance_2},{-((num_z - 1) * dist / 2) + dist * k + random_disturbance_3}"
                    if ((i + j + k) % 2 == 0):
                        objects.append(create_ball(f"{pos}", "1", f"{rad}", f"{vel}", color))
                    else:
                        objects.append(create_capsule_vertical(f"{pos}", f"{(len / rad + 1)}", f"{rad}", f"{len}", f"{vel}", color))

    plane_length_x = num_x * dist + 0.5
    plane_length_y = num_y * len * dist + len / 2 + 0.5
    plane_length_z = num_z * dist + 0.5

    objects.append(create_tile(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}"))
    objects.append(create_tile(f"-{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}"))
    
    objects.append(create_tile(f"0,0,{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    objects.append(create_tile(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    
    objects.append(create_tile(f"0,-{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}"))
    return objects
    
def make_mixed_box_scene():
    final_count = 100000
    num_x = 85
    num_y = 42
    num_z = 85
    distance = 1
    radius = 0.3
    length = 2
    
    print(num_x * num_y * num_z)

    objects = make_mixed_cube(num_x, num_y, num_z, distance, radius, length,  True, True)
    
    filtered_objects = delete_random_elems(objects, num_x * num_y * num_z - final_count)

    plane_length_x = num_x * distance + 0.5
    plane_length_y = num_y * length * distance + length / 2 + 0.5
    plane_length_z = num_z * distance + 0.5

    filtered_objects.append(create_tile(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}", True))
    filtered_objects.append(create_tile(f"-{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}"))
    
    filtered_objects.append(create_tile(f"0,0,{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}", True))
    filtered_objects.append(create_tile(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}"))
    
    filtered_objects.append(create_tile(f"0,{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}", True))
    filtered_objects.append(create_tile(f"0,-{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}"))

def make_matrix_scene():
    dist_xz = 2.3
    dist_y = 5.5
    num_x = 37
    num_y = 18
    num_z = 38
    objects = make_capsule_cube_vertical(num_x, num_y, num_z, dist_y, dist_xz)
    objects.append(create_ball(f"{num_x / 2 * dist_xz + 10},{num_y / 2 * dist_y + 10},{num_z / 2 * dist_xz + 10}", "1000", "4", "-20,-20,-20", "200,200,200"))
    return objects
    
# Main

objects = make_capsules_in_box_scene()
print(len(objects))

content = """<?xml version="1.0" encoding="utf-8"?>
<config>
	<objects>
""" + '\n'.join(objects) + """
	</objects>
    <camera rad="50" />"
</config>
"""

f = open("scenes\\autogenerated.xml", "w")
f.write(content)
f.close()