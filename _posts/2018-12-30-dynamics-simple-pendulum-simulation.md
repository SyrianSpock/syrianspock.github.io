---
layout: post
title: Robot Dynamics&#58 Numerical simulation in Python
categories: ['Robotics']
tags: ['robotics', 'dynamics', 'mathematics', 'simulation', 'python']
math: true
---

Recall from last episode, we were studying the dynamics of a simple pendulum in 2D.
We derived the equations of motion of the pendulum

$$
\ddot{\theta} + \frac{g}{l} \sin{\theta} = 0
$$

And the position of the pendulum is fully described by the angle

$$
r =
\begin{bmatrix}
l \sin{\theta} \\
- l \cos{\theta} \\
\end{bmatrix}
$$

Our goal now is to solve the ODE given by the equations of motion, so we can compute the position $r(t)$ of the pendulum at any point in time starting from a know arbitrary position $r_0$.

# Analytic solution

The equation of motion we're dealing with is not linear because of the $\sin{\theta}$ component.
In practice, you never solve this kind of system analytically unless the system is simple (like our pendulum).
Even for the is simple problem, the general solution is very tedious to derive analytically.

To make the problem easier to solve analytically, let's assume the oscillations of the pendulum will be small.
Which translates mathematically as

$$
\sin{\theta} \approx \theta
$$

Our ODE becomes linear

$$
\ddot{\theta} + \frac{g}{l} \theta = 0
$$

Intuitively, the pendulum will oscillate over time.
One way to model oscillation mathematically is with a $\sin{}$ function.
We can write a candidate solution $\tilde{\theta}$

$$
\tilde{\theta}(t) = \alpha \sin{(\beta t + \gamma)}
$$

where $\alpha$, $\beta$, and $\gamma$ are unknown parameters.
Computing the first and second derivatives of the candidate $\tilde{\theta}$ we get

$$
\dot{\tilde{\theta}}(t) = \frac{d \tilde{\theta}}{dt} = \alpha \beta \cos{(\beta t + \gamma)}
$$

$$
\ddot{\tilde{\theta}}(t) = \frac{d^2 \tilde{\theta}}{dt^2} = - \alpha \beta^2 \sin{(\beta t + \gamma)}
$$

Plugging this back in our ODE, we can solve for unknown parameters.
We'll also need an initial condition constraint: $\theta(t=0) = \theta_0$

$$
\begin{align}
\begin{cases}
\ddot{\theta} + \frac{g}{l} \theta = 0 \\
\theta(0) = \theta_0 \\
\end{cases}
&\iff
\begin{cases}
- \alpha \beta^2 \sin{(\beta t + \gamma)} + \frac{g}{l} \alpha \sin{(\beta t + \gamma)} = 0 \\
\alpha \sin{\gamma} = \theta_0 \\
\end{cases}
\\ &\iff
\begin{cases}
\alpha \neq 0 \\
\beta^2 \sin{(\beta t + \gamma)} = \frac{g}{l} \sin{(\beta t + \gamma)} \\
\alpha \sin{(\gamma)} = \theta_0 \\
\end{cases}
\\ &\iff
\begin{cases}
\alpha = \theta_0 \neq 0 \\
\beta = \sqrt{\frac{g}{l}} \\
\gamma = \frac{\pi}{2} \\
\end{cases}
\end{align}
$$

So we have solved our equation of motion and can express $\theta$ explicity as function of time $t$

$$
\theta(t)
= \theta_0 \sin\left({\sqrt{\frac{g}{l}} t + \frac{\pi}{2}}\right)
= \theta_0 \cos\left({\sqrt{\frac{g}{l}} t}\right)
$$

That's nice, and easy to use to simulate our pendulum.
But it only holds for small angles ($\sin{\theta} \approx \theta$).

# Numerical solution

Another popular approach that scales better to solving complex differential equations is numerical integration.
The basic idea behind it is that the future position $x(t + \Delta t)$ of a particle can be approximated as the sum of the current position $x(t)$ and a small displacement proportional to the velocity $v(t)$ and the time increment $\Delta t$.

$$
x(t + \Delta t) \approx x(t) + v(t) \Delta t
$$

This is called forward integration, and it's one of the most basic ways we can solve an ODE by numerical integration.
We can solve first order ODEs this way, but the equation of motion of our pendulum is a second order ODE.
Now I'm going to attempt a magic trick to transform the second order ODE into first order: a change of variable.

Let's write our state $s = \begin{bmatrix} \theta \\ \dot{\theta} \end{bmatrix}$.
The first order time derivative of $s$ is $\dot{s} = \begin{bmatrix} \dot{\theta} \\ \ddot{\theta} \end{bmatrix}$.
How convenient!
Using our beloved equation of motion, we can rewrite our problem as first order ODE in $s$

$$
\dot{s}
= \begin{bmatrix} \dot{\theta} \\ \ddot{\theta} \end{bmatrix}
= \begin{bmatrix} \dot{\theta} \\ - \frac{g}{l} \sin{\theta} \end{bmatrix}
= f(s)
$$

where $f$ is again an non arbitrary function of the state $s$.
Great success!
Let's simulate it!

This is the [Jupyter notebook](https://nbviewer.jupyter.org/github/SyrianSpock/syrianspock.github.io/blob/master/resources/robot-dynamics/pendulum.ipynb) used for simulation.
With the numerical approach, we replicated the expected pendulum oscillation also obtained analytically (although with small angle approximation).

<figure>
<center>
<img src="/resources/robot-dynamics/pendulum.gif">
<figcaption><small>Our simulation of an oscillating pendulum</small></figcaption>
</center>
</figure>

That's all folks!
In the next episode, we'll look at the Lagrangian formalism and how it makes it easier to derive the dynamics of a system from energies instead of forces.
