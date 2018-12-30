---
layout: post
title: Robot Dynamics&#58 Describing motion with classical mechanics
categories: ['Robotics']
tags: ['robotics', 'dynamics', 'mathematics', 'classical mechanics']
math: true
---

This is the start of a series on modeling the dynamics of a robotic system.
It's meant as a crash course on the subject.
The goal is to give enough mathematics and physics background to understand the problem.
We'll also write Python code to simulate the systems studied, so we can get a better grasp on it.

Modeling the dynamics of a robot is the first step in understanding it and is a key building block to simulate it and control its motion.
For that we'll need a mathematical model to describe the motion of our system.

# Starting simple with a pendulum

Let's consider the most basic dynamic system we can think of: a pendulum in 2D.
It's made of a mass $m$ suspended at the end of a rigid massless link of length $l$.
Intuitively, you correctly assume that this pendulum can only oscillate around its fixation point.
Assuming conservation of energy (no friction or other form of energy dissipation), the pendulum will oscillate forever as displayed below.

<figure>
<center>
<img src="https://upload.wikimedia.org/wikipedia/commons/2/24/Oscillating_pendulum.gif">
<figcaption><small>Oscillating pendulum animation - Wikipedia</small></figcaption>
</center>
</figure>

The pendulum has a single degree of freedom: its angle $\pi$
However, in 2D cartesian space, the position $r$ of a particle is defined by 2 coordinates: $x$ (left-right direction) and $y$ (bottom-up direction).
We can express $x$ and $y$ position of the mass at the end of our pendulum in these coordinates from the angle assuming the anchor point of the pendulum is at coordinates $(0, 0)$

$$
r =
\begin{bmatrix}
l \sin{\theta} \\
- l \cos{\theta} \\
\end{bmatrix}
$$

So, given the evolution in time of $\theta$, we can compute the position of the pendulum in cartesian coordinate, and vice versa.
This evolution is fully determined by what we call the equations of motion of the system.
These are second order ordinary differential equations (ODE) in position $r$ of general form

$$
f(r(t), \dot{r}(t), \ddot{r}(t)) = 0
$$

where $f$ is a function of the position $r(t)$, the velocity $\dot{r}(t)$, and the acceleration $\ddot{r}(t)$.
To simplify notation, we'll write $r(t) = r$.

Once we know the equations of motion of our system, we're more or less done.
The dynamics are fully described, and we can predict and simulate our system by numerical integration of this ODE.

Now the question remains, how to obtain the equations of motion of a system?
There are three methods:
- the Classical mechanics approach
- the Lagrangian mechanics approach
- the Hamiltonian mechanics approach

The first one relies on forces and Newton's laws while the latter two are formalisms that deal with energies instead of forces and derive the equations of motions from quantities refered to as the Lagrangian and the Hamiltonian respectively.
In this post, we'll explore classical mechanics and how to apply it to derive the equations of motion of the pendulum.

# Newton's laws of motion

Classical mechanics rely upon Newton's laws of motion[^1]:
- **First law:** In an inertial frame of reference, an object either remains at rest or continues to move at a constant velocity, unless acted upon by a force.

$$
\sum F = 0 \iff \frac{d \dot{r}}{dt} = \ddot{r} = 0
$$

- **Second law:** In an inertial frame of reference, the vector sum of the forces $F$ on an object is equal to the mass $m$ of that object (assumed to be constant) multiplied by the acceleration a of the object.

$$
\sum F = m \ddot{r}
$$

- **Third law:** When one body exerts a force $F_A$ on a second body, the second body simultaneously exerts a force $F_B$ equal in magnitude and opposite in direction on the first body.

$$
F_A = - F_B
$$

The most important law, for us, among the three is the second one.
Given all the forces acting upon our system, we can sum them and that fully describes the acceleration of the system.
Notice that we are adding up forces like vectors, this is an implicit law also described by Newton: forces obey the [superposition principle](https://en.wikipedia.org/wiki/Superposition_principle).

These laws also apply for torques, with the exception that the second law becomes

$$
\sum \tau = I \ddot{\theta}
$$

where
- $\tau$ represents torques as opposed to forces $F$
- $I$ represents inertia, as opposed to mass $m$
- $\theta$ represents angular position, as opposed to linear position $r$

# Balance of forces

Let's look at the pendulum and describe the external forces acting on it.
Our pendulum is subject to gravity, this is a force proportional to the mass of the pendulum and pull it downwards (negative y direction).
So we can express in 2D cartesian space as

$$
F_{gravity} =
\begin{bmatrix}
0 \\
- m g
\end{bmatrix}
$$

where $g$ is the gravity constant ($g = 9.80665 \frac{m}{s^2}$ on Earth).

We can also express this gravity force in polar coordinates instead of cartesian, i.e. in terms of radial $\rho$ (from anchor point outwards) and angular $\theta$ (from x to y axis) components.
This will make the equations easier later

$$
\hat{F}_{gravity} =
\begin{bmatrix}
m g \cos{\theta} \\
- m g \sin{\theta}
\end{bmatrix}
$$

Here's a force diagram from [Wikipedia](https://upload.wikimedia.org/wikipedia/commons/thumb/6/66/Pendulum_gravity.svg/450px-Pendulum_gravity.svg.png) if you're lost

<figure>
<center>
<img height="600" src="https://upload.wikimedia.org/wikipedia/commons/thumb/6/66/Pendulum_gravity.svg/450px-Pendulum_gravity.svg.png">
</center>
</figure>

Remember that our pendulum is anchored to a non-moving structure.
Newton's third law implies that the anchor will exert a force equal in magnitude and opposite to the radial component of the gravity force acting on the pendulum.
This matches our intuitive judgement that motion along the pendulum's radius is impossible as the link is rigid.
So in the radial direction forces cancel out, and the pendulum won't move along that axis.

We can write the sum of forces exerted on the pendulum in polar coordinates

$$
\sum \hat{F}
=
\hat{F}_{gravity} + \hat{F}_{anchor}
=
\begin{bmatrix}
m g \cos{\theta} \\
- m g \sin{\theta}
\end{bmatrix}
+
\begin{bmatrix}
- m g \cos{\theta} \\
0
\end{bmatrix}
=
\begin{bmatrix}
0 \\
- m g \sin{\theta}
\end{bmatrix}
$$

from here, it's easier to switch to torques.
[Torque](https://en.wikipedia.org/wiki/Torque) is defined as the cross product of the position vector $r$ and the force applied $F$.

$$
\tau = r \times F
$$

Torque in 2D is a 1D vector.
Total torque applied on the pendulum is is trivially derived from the total force

$$
\sum \tau
=
r \times \sum F
=
\begin{bmatrix}
l \\
0
\end{bmatrix}
\times
\begin{bmatrix}
0 \\
- m g \sin{\theta}
\end{bmatrix}
= - m g l \sin{\theta}
$$

# Deriving the equations of motion

Recall the angular system version of Newton's second law

$$
\sum \tau = I \ddot{\theta}
$$

The inertia of our pendulum is $I = m l^2$[^2] and the total torque we derived earlier, so we can write

$$
- m g l \sin{\theta} = m l^2 \ddot{\theta}
$$

That can be simplified to

$$
\ddot{\theta} + \frac{g}{l} \sin{\theta} = 0
$$

which has the expected form of a second order ODE in position coordinates.
Using this ODE, we can model the evolution of its angle $\theta$ over time.
This fully describes our pendulum's motion in either polar coordinates as

$$
\hat{r} =
\begin{bmatrix}
l \\
\theta
\end{bmatrix}
$$

or cartesian coordinates since

$$
r =
\begin{bmatrix}
l \sin{\theta} \\
- l \cos{\theta} \\
\end{bmatrix}
$$

Solving this ODE numerically will allows us to simulate the pendulum.
This is the subject of next episode.

## References

[^1]: Newton's laws of motion on [Wikipedia](https://en.wikipedia.org/wiki/Newton%27s_laws_of_motion).
[^2]: Moments of inertia table on [Wikipedia](https://en.wikipedia.org/wiki/List_of_moments_of_inertia#Moments_of_inertia)
