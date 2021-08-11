import pathlib
import os
import random

def create_capsule_vertical(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"90,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />\n"
    
def create_capsule_horizontal(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />\n"
 
def create_ball(pos, mass, rad, vel, color):
    return f"\t\t<sphere pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"1\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" />\n"
    
def create_plane(pos, ang, color, drawLength, drawWidth):
    return f"\t\t<plane pos=\"{pos}\" mass=\"{1}\" elasticityCoef=\"1\" ang=\"{ang}\" color=\"{color}\" static=\"true\" drawLength=\"{drawLength}\" drawWidth=\"{drawWidth}\" />\n"

def create_tile(pos, ang, color, length, width):
    return f"\t\t<tile pos=\"{pos}\" mass=\"{1}\" elasticityCoef=\"1\" ang=\"{ang}\" color=\"{color}\" static=\"true\" length=\"{length}\" width=\"{width}\" />\n"

def get_random_velocity():
    #posible_velocities = [i/10 for i in range(-30,31,1)]
    posible_velocities = [i/6 for i in range(-30,31,1)]
    return f"{random.choice(posible_velocities)},{random.choice(posible_velocities)},{random.choice(posible_velocities)}"
    
def get_random_color():
    posible_color_values = [i for i in range(0,256,1)]
    return f"{random.choice(posible_color_values)},{random.choice(posible_color_values)},{random.choice(posible_color_values)}"

def make_capsule_cube_horizontal(num_x, num_y, num_z, dist_1, dist_2):
    objects = ""
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                objects += create_capsule_horizontal(f"{-12 + dist_1 * i},{-15 + dist_2 * j},{-50 + dist_1 * k}", "1", "0.5", "4", "0,0,15", "220,0,0")
    return objects

def make_capsule_cube_vertical(num_x, num_y, num_z, dist_1, dist_2):
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                objects += create_capsule_vertical(f"{-12.5 + dist_1 * i},{-16 + dist_1 * j},{50 - dist_2 * k}", "1", "0.5", "4", "0,0,-15", "0,0,220")
    return objects

def make_ball_cube(num_x, num_y, num_z, dist, rad, use_velocities, use_random_colors):
    possible_disturbances = [i/100 for i in range(-10,11,1)]
    objects = ""
    for i in range(0, num_x):
        for j in range(0, num_y):
            for k in range(0, num_z):
                if 1 == random.choice([1]):
                    random_disturbance_1 = random.choice(possible_disturbances)
                    random_disturbance_2 = 0
                    random_disturbance_3 = random.choice(possible_disturbances)
                    vel = get_random_velocity() if use_velocities else "0,0,0"
                    color = get_random_color() if use_random_colors else "220,0,0"
                    objects += create_ball(f"{-((num_x - 1) * dist / 2) + dist * i + random_disturbance_1},{-((num_y - 1) * dist / 2) + dist * j + random_disturbance_2},{-((num_z - 1) * dist / 2) + dist * k + random_disturbance_3}", "1", f"{rad}", f"{vel}", color)
    return objects


def make_two_waves_scene():
    objects = ""
    num_x = 80
    num_y = 1
    num_z = 40
    dist = 0.8

    objects += make_ball_cube(num_x, num_y, num_z, dist, 0.3, False, False)

    objects += create_ball("13,0,-30", "25", "6", "0,0,20", "220,220,220")
    objects += create_ball("-13,0,-30", "25", "6", "0,0,20", "220,220,220")

    plane_length_x = num_x * dist + 0.5
    plane_length_z = num_z * dist + 0.5

    objects += create_plane(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", "4", f"{plane_length_z}")
    objects += create_plane(f"0,0,{plane_length_z / 2}", "-90,0,0", "220,220,220", f"{plane_length_x}", "4")
    objects += create_plane(f"-{plane_length_x / 2},0,0", "0,0,-90", "220,220,220", "4", f"{plane_length_z}")
    
    mid_rect_width_div = 4
    rect_width_div = 4.5
    
    objects += create_tile(f"{plane_length_x / 2 - plane_length_x / rect_width_div + plane_length_x / rect_width_div / 2},0,-{plane_length_z / 2}",
               "90,0,0", "220,220,220", f"{plane_length_x / rect_width_div}", "20")
    objects += create_tile(f"{-plane_length_x / 2 + plane_length_x / rect_width_div - plane_length_x / rect_width_div / 2 },0,-{plane_length_z / 2}",
               "90,0,0", "220,220,220", f"{plane_length_x / rect_width_div}", "20")
               
    objects += create_tile(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x / mid_rect_width_div}", "20")
               
    return objects

def make_balls_in_box_scene():
    objects = ""
    num_x = 18
    num_y = 18
    num_z = 19
    dist = 1

    objects += make_ball_cube(num_x, num_y, num_z, dist, 0.3, True, True)

    plane_length_x = num_x * dist + 0.5
    plane_length_y = num_y * dist + 0.5
    plane_length_z = num_z * dist + 0.5

    objects += create_plane(f"{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{plane_length_y}", f"{plane_length_z}")
    objects += create_plane(f"-{plane_length_x / 2},0,0", "0,0,90", "220,220,220", f"{0}", f"{0}")
    
    objects += create_plane(f"0,0,{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}")
    objects += create_plane(f"0,0,-{plane_length_z / 2}", "90,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_y}")
    
    objects += create_plane(f"0,{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{0}", f"{0}")
    objects += create_plane(f"0,-{plane_length_y / 2},0", "0,0,0", "220,220,220", f"{plane_length_x}", f"{plane_length_z}")
    return objects

objects = make_two_waves_scene()

content = """<?xml version="1.0" encoding="utf-8"?>
<config>
	<objects>
""" + objects + """
	</objects>
</config>
"""

f = open("scenes\\autogenerated.xml", "w")
f.write(content)
f.close()