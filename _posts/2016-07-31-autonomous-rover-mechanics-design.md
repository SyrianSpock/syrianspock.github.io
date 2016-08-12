---
layout: post
title: Autonomous Rover&#58 Experimenting with mechanical design
categories: ['Autonomous Rover']
tags: ['autonomous rover', 'mechanics', 'design', 'robotics', '3d printing']
---

Since my last post about the motivation behind my autonomous rover project[^0], I have been looking with [@romain_bersier](https://twitter.com/romain_bersier) for the mechanical structure that best fits the needs of an autonomous rover.

We have reviewed a few possibilities, and this article intends to show what we came out with.
Note that we intentionally disregarded two-wheel solutions: differential robots with caster wheels are innapropriate for navigation in non-flat terrain, and Segway-like robots are statically unstable.

# RC cars
The RC market has exploded in recent years, and RC cars can now reach staggering velocities.
You can buy an RC car from one hunder dollars to a few thousands of dollars, covering a wide variety of scales and applications.
For our application, there are three categories that could be interesting: buggy cars, rally cars and rock crawler cars.

## The buggy
<img src="/images/rover/buggy.jpg" alt="Buggy RC car" style="width: 500px;"/>
A buggy car is a lightweight car that can be driven off-road.
This kind of car is suited for hills, sand, grass and clay.
It is lightweight so the motors have less mass to carry, which means it can go fast.
However, it won't handle rocks and big obstacles.

## The rally
<img src="/images/rover/rally.jpg" alt="Rally RC car" style="width: 500px;"/>
A rally car is an off-road car used for [rallying](https://en.wikipedia.org/wiki/Rallying).
This means rally cars are very similar to buggy cars, but they are not as lightweight because they are usually modified consumer cars -- in contrast, buggy cars are built from
scratch.

## The rock crawler
<img src="/images/rover/crawler.jpg" alt="Crawler RC car" style="width: 500px;"/>
A rock crawler car is a niche category of buggy cars.
Their main feature is being able to crawl over rocks, duh.
The trade-off is more complex suspension system and drivetrain.

## Pros and cons of RC cars
Here is a summary of the advantages and disadvantages of RC cars.

### Advantages
- Off-the shelf RTR (ready-to-run) kits available
- Fast (up to 50-100km/h)
- Simple mechanically, with the exception of the rock crawler
- Good off-road performance: sand, clay, grass, and slightly inclined terrain

### Disadvantages
- Can't go arbitrarily slow (<10km/h), because of hysteresis on commercial ESCs (electronic speed controller)
- Poor to medium performance on very rugged terrain, with the exception of the rock crawler
- Can't move in-place: rotation is not possible without forward motion
- Complex kinematics model

# Rocker-bogie structures
Originally designed for the Mars Pathfinder mission, the rocker-bogie is the preferred mechanical suspension system for space rovers.
<img src="https://upload.wikimedia.org/wikipedia/commons/e/ec/PIA15279_3rovers-stand_D2011_1215_D521-crop2-CuriosityRover.jpg" alt="Curiosity rover" style="width: 500px;"/>

It is the ideal choice for navigation in highly unstructured terrain.
As the gif below shows[^2], it's a six wheel design that allows the rover to climb obstacles up to two times the diameter of the wheels.
<img src="https://upload.wikimedia.org/wikipedia/commons/5/55/Rocker_bogie.gif" alt="Rocker-bogie" style="width: 500px;"/>

## Rocker-bogie
The rocker-bogie design is made of six motorized wheels, three on each side[^1].
The wheels are connected with two links: the rocker and the bogie.
The rocker is the bigger link, it is connected to a wheel on one end, to the bogie on the other, and to the rover body in the middle (at the rocker pivot).
The bogie is the smaller link, it has two wheels, and it connects in the middle to the rocker with a pivot.

## Differential
To prevent the rover from tipping and to keep all six wheels on the ground, the rocker-bogie uses a differential to couple the linkages on both sides.
This is performed either with a differential gearbox (eg. on Spirit and Opportunity) or with a differential bar (eg. on Curiosity).

The gearbox is more complex to design, but more compact.
The bar is much simpler, but bulkier.

## Pros and cons of Rocker-bogie structures

### Advantages
- No springs: all wheels are kept on ground with equal pressure
- Good performance on all kinds of unstructured terrain: sand, clay, grass, hills, rocks, stairs, etc.
- Simple kinematics as we can drive it as differential drive
- In-place rotation is possible

### Disadvantages
- Slower than an RC car
- More complex mechanically
- Needs to be built from scratch (with off-the-shelf components)


# Experimenting
We decided to go for a rocker-bogie design as we were privileging navigation in all kinds of terrain to raw speed.
So we started experimenting, by 3D printing structural pieces, and using off-the-shelf parts (ESCs, DC motors, standardized RC wheels), to build a first iteration of the rover.

Here are some pictures of the parts we 3D printed last week.
![3D printed rocker-bogie](/images/rover/3dprinting_rocker_bogie.jpg)
![All 3D printed parts](/images/rover/3dprinting_results.jpg)

We estimated the projected cost of the mechanics to be around 200 dollars, which accounts for 20% of the maximum budget I fixed for this project.
This includes motors, ESCs, and the cost of plastic filament used for printing.


# What's next ?
- Assemble a rover and test it, once the ordered parts arrive (motors, ESCs, wheels).
- Iterate on the design to improve stability and performance.
- Design a body to fit the embedded computer, control boards, and sensors.

# References

[^0]: Autonomous Rover: Motivation [http://syrianspock.github.io/autonomous%20rover/2016/06/30/autonomous-rover-motivation](http://syrianspock.github.io/autonomous%20rover/2016/06/30/autonomous-rover-motivation)
[^1]: A great explanation of how the rocker-bogie suspension works [http://www.alicesastroinfo.com/2012/07/mars-rover-rocker-bogie-differential/](http://www.alicesastroinfo.com/2012/07/mars-rover-rocker-bogie-differential/)
[^2]: Wikipedia article on the rocker-bogie [https://en.wikipedia.org/wiki/Rocker-bogie](https://en.wikipedia.org/wiki/Rocker-bogie)
