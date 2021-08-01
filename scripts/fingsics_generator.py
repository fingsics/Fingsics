import pathlib
import os


def create_capsule(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"0.4\" ang=\"90,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />\n"
    
def create_capsule2(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"0.4\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />\n"

def create_capsule2_static(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"0.4\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" static=\"true\" length=\"{length}\" />\n"
 

def create_capsule3(pos, mass, rad, length, vel, color):
    return f"\t\t<capsule pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"0.4\" ang=\"70,0,0\" color=\"{color}\" radius=\"{rad}\" length=\"{length}\" />\n"
    
def create_ball(pos, mass, rad, vel, color):
    return f"\t\t<sphere pos=\"{pos}\" vel=\"{vel}\" mass=\"{mass}\" elasticityCoef=\"0.4\" ang=\"0,0,0\" color=\"{color}\" radius=\"{rad}\" />\n"
    
def horizontal(x, y, z, color):
    objects = ""
    for i in range(0,3):
            objects += create_capsule2(f"{x},{y + i},{z}", "1", "0.5", "3", "0,0,40", color)
            objects += create_capsule2(f"{x-1},{y + i},{z}", "1", "0.5", "3", "0,0,40", color)
    return objects

def vertical3(x, y, z, color):
    objects = ""
    for i in range(0,3):
        for j in range(0,3):
                objects += create_capsule(f"{x},{y + 4*j},{z + i}", "1", "0.5", "3", "0,0,40", color)
                objects += create_capsule(f"{x-1},{y + 4*j},{z + i}", "1", "0.5", "3", "0,0,40", color)
    return objects

def vertical2(x, y, z, color):
    objects = ""
    for i in range(0,3):
        for j in range(0,2):
                objects += create_capsule(f"{x},{y + 4*j},{z + i}", "1", "0.5", "3", "0,0,40", color)
                objects += create_capsule(f"{x-1},{y + 4*j},{z + i}", "1", "0.5", "3", "0,0,40", color)
    return objects

def vertical1(x, y, z, color):
    objects = ""
    for i in range(0,3):
        for j in range(0,1):
                objects += create_capsule(f"{x},{y + 4*j},{z + i}", "1", "0.5", "3", "0,0,40", color)
                objects += create_capsule(f"{x-1},{y + 4*j},{z + i}", "1", "0.5", "3", "0,0,40", color)
    return objects


def tilted(x, y, z, color):
    objects = ""
    for i in range(0,2):
        for j in range(0,3):
                objects += create_capsule3(f"{x},{y + 4*j},{z + i - j}", "1", "0.5", "3", "0,0,40", color)
                objects += create_capsule3(f"{x-1},{y + 4*j},{z + i - j}", "1", "0.5", "3", "0,0,40", color)
    return objects  


objects = ""

xInit = 36
yInit = -5
zInit = -140

blue_color = "0,96,255"
white_color = "220,220,220"

#F
objects += vertical3(xInit, yInit, zInit, blue_color)
objects += horizontal(xInit, yInit + 7.5, zInit + 4.5, blue_color)
objects += horizontal(xInit, yInit + 3.5, zInit + 4.5, blue_color)

#i
objects += vertical2(xInit, yInit, zInit + 9, blue_color)
objects += create_ball(f"{xInit},{yInit + 9},{zInit + 10}", "1", "1.7", "0,0,40", blue_color)

#n
objects += vertical3(xInit, yInit, zInit + 14, blue_color)
objects += tilted(xInit, yInit, zInit + 19.7, blue_color)
objects += vertical3(xInit, yInit, zInit + 22.4, blue_color)

#g
objects += vertical3(xInit, yInit, zInit + 27, blue_color)
objects += horizontal(xInit, yInit + 7.5, zInit + 31.5, blue_color)
objects += horizontal(xInit, yInit + 2.5, zInit + 34, blue_color)
objects += horizontal(xInit, yInit + -1.5, zInit + 31.5, blue_color)
objects += vertical1(xInit, yInit, zInit + 34, blue_color)

#s
objects += vertical2(xInit, yInit + 4, zInit + 39, white_color)
objects += horizontal(xInit, yInit + 7.5, zInit + 43.5, white_color)
objects += horizontal(xInit, yInit + -1.5, zInit + 40.5, white_color)
objects += vertical2(xInit, yInit, zInit + 43, white_color)

#i
objects += vertical2(xInit, yInit, zInit + 48, white_color)
objects += create_ball(f"{xInit},{yInit + 9},{zInit + 49}", "1", "1.7", "0,0,40", white_color)

#c
objects += vertical3(xInit, yInit, zInit + 53, white_color)
objects += horizontal(xInit, yInit + 7.5, zInit + 57.5, white_color)
objects += horizontal(xInit, yInit + -1.5, zInit + 57.5, white_color)

#s
objects += vertical2(xInit, yInit + 4, zInit + 62, white_color)
objects += horizontal(xInit, yInit + 7.5, zInit + 66.5, white_color)
objects += horizontal(xInit, yInit + -1.5, zInit + 63.5, white_color)
objects += vertical2(xInit, yInit, zInit + 66, white_color)


objects += create_capsule2_static(f"{xInit},{yInit + 6},{zInit + 280}", "1000", "2", "10", "0,0,-40", "70,70,70")
objects += create_capsule2_static(f"{xInit},{yInit + 1},{zInit + 280}", "1000", "2", "10", "0,0,-40", "70,70,70")

content = """<?xml version="1.0" encoding="utf-8"?>
<config>
	<objects>
""" + objects + """
	</objects>
</config>
"""

f = open("scenes\\fingsics.xml", "w")
f.write(content)
f.close()