---
layout: post
title: Autonomous Rover&#58 Motivation
categories: ['Autonomous Rover']
tags: ['autonomous rover', 'ROS', 'robotics']
---

When I say rover, you probably think about space rovers, like Yutu or Curiosity.
These robots are impressive feats of engineering, but they tend to be semi-autonomous and quite slow -- Curiosity averages 30m/h.
Compared to the 14km/h average of the manned lunar rover, this is a bit underwhelming.

But why can't we make them faster?
Speed is not a requirement for space exploration, for now.
Engineers are asked to build reliable and compact machines, not racing cars.
Scientists want to take time to choose carefully where to go take samples on Mars and analyse them.

In the last decade or so, the miniaturisation of electric motors has allowed electric RC cars to develop among other things.
The speed you can reach with RC cars is quite impressive, [some 1/10<sup>th</sup> scale models can reach up to 100km/h](https://traxxas.com/products/models/electric/74076rally).
This means accessible compact and fast little cars.
Although they won't be very reliable depending on their build quality, but that's OK since they are quite cheap (100$ to 1000$ price range).

With the rising trend of autonomous cars and all the funding in related research, a lot of cool stuff is being developed and made open source.
Combined with affordable and performant RC cars, this has allowed projects such as the [MIT racecar](https://github.com/mit-racecar/) and [Autorally -- the cars that learn to drift --](http://autorally.github.io/) to be developed and shared with us to learn from.
<div class="ytvideo">
<iframe width="640" height="360" src="https://www.youtube.com/embed/p18879Dji4c" frameborder="0" allowfullscreen></iframe>
</div>
<div class="ytvideo">
<iframe width="640" height="360" src="https://www.youtube.com/embed/1AR2-OHCxsQ" frameborder="0" allowfullscreen></iframe>
</div>

Technologies like [ROS](http://www.ros.org/) and [Gazebo](http://gazebosim.org/) have made it easier to develop software for such robots and reuse components.
Simulation allows for thighter feedback loops of trial and error for prototyping and development.

My motivation for this projet is to build a rover that will be able to navigate in all-terrain fast and autonomously, for less than 1000$.
