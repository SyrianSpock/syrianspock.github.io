---
layout: post
title: Robot Dynamics&#58 Hamiltonian formalism &#38; AutoDiff
categories: ['Robotics']
tags: ['robotics', 'dynamics', 'mathematics', 'hamiltonian mechanics', 'automatic differentiation', 'AutoDiff']
math: true
---

As promised in this series, we dive into the last piece: how to use Hamiltonian mechanics[^1] and automatic differentiation to simulate any mechanical system given its description and assuming no loss of energy (no dissipation modeled).

We need to define three components for our system to be automagically solved using Hamiltonian equations and automatic differentiation:
- the dynamics in Cartesian space expressed as a function of a minimal generalized coordinates set,
- the inertia matrix in Cartesian space,
- the potential energy of the system as a function of the same generalized coordinates set used above.

# Dynamics

Let's consider again our toy problem of describing the motion of a N-link planar pendulum in 2D Cartesian space.
Its dynamics in Cartesian space are a function of the pivot link angle such that:

$$
f(q) =
\begin{bmatrix}
\sum_{1}^{N}{l_i \cos{\theta_i}} \\
\sum_{1}^{N}{l_i \sin{\theta_i}} \\
\end{bmatrix}
$$

where $l_n$ is the link of that pendulum's link and $\theta_i$ is the link's pivot angle.
So the set of angles $\theta_i$ gives us our set of generalized coordinates $q_i$.

# Inertia Matrix

For a perfect pendulum its mass is given by each link's masses such that only the diagonal is non-zero:

$$
M(q) =
\begin{bmatrix}
    m_{1} & & \\
    & \ddots & \\
    & & m_{N}
\end{bmatrix}
$$

where $m_i$ is the mass of a given link in the pendulum.

# Potential Energy

Assuming the pendulum rests at zero angles in the direction of gravity, the potential energy is given by:

$$
U(q) =
\begin{bmatrix}
    m_i g y(q)
\end{bmatrix}_i
$$

where $g$ is the Earth's gravity constant and y(q) is the position of the pendulum in the y-axis as described by its dynamics f(q) above.

# Combining it all together

Mixing it all together we end up with

- the jacobian of the dynamics by AD:

$$
J_f(q) =
\nabla_{q}{f(q)}
$$

- and the associated Hessian by one more AD step:

$$
H_f(q) =
\nabla^2_{q}{f(q)}
$$

- from that we can derive the evolution of the momentum of our system over time:

$$
\dot{p} = p^T (J_f^T M J_f)^{-1} J_f^T M H_f (J_f^T M J_f)^{-1} p - \nabla_{q}{U(q)}
$$

- and we can deduce the evolution of the system over time since:

$$
\dot{q} = (J_f^T M J_f)^{-1} p
$$

by integrating those 1^{st} order differential equations, we can simulate our system given its initial conditions.

In Python, this can be expressed as follows:

{% gist 4918275ff45c5e2898cf821c611e6142 %}

This is exactly what I implemented for [CVRA](https://www.cvra.ch/) in a [custom package](https://github.com/cvra/robot-software/blob/master/eurobot/arm-simulator/arm_simulator/hamilton.py)[^2] to [simulate the 3-link pendulum of our robot's arm](https://github.com/cvra/robot-software/blob/master/eurobot/arm-simulator/examples/pendulum-triple.ipynb).

# Conclusion

So in this series, we've learned how to simulate any mechanical conservative system with minimal amount of math derived by hand.
This was useful to simulate a 3-link pendulum in Python.
This article also concludes my series until I have figured out the maths to simulate dissipative energies in a system with Hamiltonian mechanics.

## References

[^1]: Hamiltonian mechanics on [Wikipedia](https://en.wikipedia.org/wiki/Hamiltonian_mechanics).
[^2]: [Custom Python package leveraging Hamiltonian mechanics](https://github.com/cvra/robot-software/blob/master/eurobot/arm-simulator/arm_simulator/hamilton.py)
