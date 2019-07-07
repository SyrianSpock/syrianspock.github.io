---
layout: post
title: Robot Dynamics&#58 Lagrangian formalism
categories: ['Robotics']
tags: ['robotics', 'dynamics', 'mathematics', 'lagrangian mechanics']
math: true
---

As we have seen in the [first post of this series](/robotics/2018/12/29/dynamics-simple-pendulum/), deriving the equations of motion, even for a system as simple as a pendulum, can be tedious.
Classical mechanics (Newtonian mechanics) requires us to describe all the forces acting upon the system, including constraint forces.
For complex systems this quickly becomes time consuming.

However, in the 18th century, Joseph-Louis Lagrange introduced a very interesting reformulation of classical mechanics.
The Lagrangian formalism allows us to derive the equations of motion from energies.
Thus not dealing with constraints at all provided that we choose our generalized coordinates properly.

# Lagrange's promise

In Lagrangian formalism[^1], we define a quantity called Lagrangian $L$ as the difference between the kinetic energy $T$ of the system and its potential energy $U$

$$
L = T - U
$$

Given that this quantity is expressed with respect to a set of generalized coordinates $q_i$ and their associated time derivatives $\dot{q_i}$, the Euler-Lagrange equation

$$
\frac{\partial{L}}{\partial{q_i}} = \frac{d}{dt} \frac{\partial{L}}{\partial{\dot{q_i}}}
$$

allows us to derive the equations of motion of our system.
So let's go through this step by step for the pendulum.

# Generalized coordinates

We call generalized coordinates any arbitrary set of parameters that describe the configuration of a system.
We don't have to stick to convential Cartesian coordinates or roll with exotic polar, cylindrical, or spherical coordinates.
We want to make our life easy, so we select a minimal set that has enough parameters to fully describe the dynamics of our system.
So the number of generalized coordinates will be equal to the number of degrees of freedom.

For our pendulum, it's clear that we have a single degree of freedom: the angle $\theta$.
We'll chose this as generalized coordinate.
The pendulum's position in Cartesian coordinates can be computed at any time given $\theta$

$$
r =
\begin{bmatrix}
l \sin{\theta} \\
- l \cos{\theta} \\
\end{bmatrix}
$$

# Kinetic energy

The kinetic energy[^2] $T$ of any system can be computed as the sum of the kinetic energies of all its moving parts with masses $m_i$ along all axes of motion $x_i$.
This mathematically translates to

$$
T = \sum_i{\frac{1}{2} m_i \dot{x_i}^2}
$$

We can compute the velocity in Cartesian coordinates by deriving the equations seen in last section with respect to time, yielding

$$
\dot{r} =
\begin{bmatrix}
l \dot{\theta} \cos{\theta} \\
l \dot{\theta} \sin{\theta} \\
\end{bmatrix}
$$

Now we plug this back in the formula for kinetic energy, assuming a pendulum of mass $m$ to get the kinetic energy as a function of the angular velocity $\dot{\theta}$

$$
T
= \frac{1}{2} m (l \dot{\theta} \cos{\theta})^2 + \frac{1}{2} m (l \dot{\theta} \sin{\theta})^2
= \frac{1}{2} m l^2 \dot{\theta}^2
$$

# Potential energy

We assumed our pendulum to only be subject to gravity, no dissipative forces apply to it.
Thus its potential energy[^3] $U$ can be defined as a function of its position in the direction of gravity (i.e. $y$ the second Cartesian axis)

$$
U = m g y = - m g l \cos{\theta}
$$

# Invoking Euler-Lagrange

Now our Lagrangian is expressed as function of our generalized coordinate $\theta$ and its associated time derivative $\dot{\theta}$

$$
L(\theta, \dot{\theta}) = \frac{1}{2} m l^2 \dot{\theta}^2 + m g l \cos{\theta}
$$

We start by computing the left side of the Euler-Lagrange equation for our pendulum

$$
\frac{\partial{L}}{\partial{\theta}} = - m g l \sin{\theta}
$$

then we compute the right side

$$
\frac{d}{dt} \frac{\partial{L}}{\partial{\dot{\theta}}}
= \frac{d}{dt} \left( m l^2 \dot{\theta} \right)
= m l^2 \ddot{\theta}
$$

And now for the final act, I will merge both sides to reconstitute the Euler-Lagrange equation

$$
- m g l \sin{\theta} = m l^2 \ddot{\theta}
$$

we rearrange the terms a bit

$$
\ddot{\theta} = - \frac{g}{l} \sin{\theta}
$$

...and tadam!
That looks familiar.
We end up with the same equation of motion previously computed using Newtonian mechanics in the [first episode of this series](/robotics/2018/12/29/dynamics-simple-pendulum/).

# But does it scale?

Now this is all fun and simple, but what happens for a more complex system?
Say a double or a triple pendulum?
Well.. things are simpler in Langrange's world, but it's still a long and dull process of deriving expressions by hand.
Surely we can make computers do some of the work, if not most of it.
Haven't you heard of [automatic differentiation](https://en.wikipedia.org/wiki/Automatic_differentiation), can't we use that?
Sadly not with the Lagrangian approach, because of the explicit time derivative in the Euler-Lagrange equation.
Remember that our generalized coordinate $q$ and their associated time derivatives $\dot{q}$ are both functions of time: $q = q(t)$ and $\dot{q} = \dot{q}(t)$.

In the next episode, we'll dive into Hamilton's improvement on Lagrange's approach and see how we can leverage it to compute the equations of motion of any system with a minimal set of input expressions defining it.

## References

[^1]: Lagrangian mechanics on [Wikipedia](https://en.wikipedia.org/wiki/Lagrangian_mechanics).
[^2]: Kinetic energy on [Wikipedia](https://en.wikipedia.org/wiki/Kinetic_energy)
[^3]: Potential energy on [Wikipedia](https://en.wikipedia.org/wiki/Potential_energy)
