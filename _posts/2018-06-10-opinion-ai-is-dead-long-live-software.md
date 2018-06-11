---
layout: post
title: Opinion&#58 AI is dead, long live Software!
categories: ['Software', 'Opinion']
tags: ['functional', 'programming', 'machine learning']
image: /images/John-McCarthy.jpg
---

"As soon as it works, no one calls it AI any more." John McCarthy

<figure>
    <center>
    <img src="/images/John-McCarthy.jpg" alt="John McCarthy (1927-2011), the father of AI">
    <figcaption>
        <small>John McCarthy (1927-2011), the father of AI</small>
    </figcaption>
    </center>
</figure>

# Winter is coming
Recent advances in Machine Learning research have sparked a global debate over AI once again.
We went from unusable image segmentation algorithms in the 2000s, to amazing accuracy in image classification and segmentation tasks today thanks to ML.

Unfortunately, these advances have spurred unrealistic promises from everyone: philosophers, journalists, entrepreneurs, and even ML researchers.
Everyone seems hypnotized by the recent successes of the field, leading to speculation about AI taking our jobs or the singularity coming sooner than expected.
They may be right, but they most likely are not (read ["Chill: Robots Won't Take All Our Jobs"](https://www.wired.com/2017/08/robots-will-not-take-your-job/)).
Even if the field has delivered a lot of astonishing results, the expectations of the general public are set too high to make the hype sustainable.

This all sounds too familiar.
AI as a field has already lived through two winters: [1974–1980 and 1987–1993](https://en.wikipedia.org/wiki/AI_winter).
These were periods following the collapse of a hype after research reached a plateau in performance, and the unrealistic promises were not fulfilled.
The public got disinterested and funding to research became scarce.
Now the early signs of an imminent winter are revealing, see [Piękniewski's blog "AI winter is well on its way"](https://blog.piekniewski.info/2018/05/28/ai-winter-is-well-on-its-way/).

# The elephant in the room
_Disclaimer: By Machine Learning I mean Deep Learning and other recent derivatives (e.g. Wide Learning), not to be confused with statistical learning which forms the older chunk of Machine Learning algorithms (Linear Regression, Support Vector Machines, etc)._

Machine Learning is often depicted as the silver bullet to solve all problems.
I'm sorry to tell you it's not.
As with any technology, there are pros and cons.
Many articles out there explain the pros, but let's examine the cons.

- ML needs a lot of data and a lot of computational power to work (see [OpenAI's review](https://blog.openai.com/ai-and-compute/)).
ML relies on old research from the last century: [Neural networks are from 1943](https://link.springer.com/article/10.1007/BF02478259), [Backpropagation goes back to 1974](https://www.researchgate.net/publication/35657389_Beyond_regression_new_tools_for_prediction_and_analysis_in_the_behavioral_sciences), and convolutional neural networks find their roots in [the Neocognitron from 1980](https://link.springer.com/article/10.1007/BF00344251).
The development of ML in this decade is due to a combination of circumstances:
    1. the huge amounts of data collected as we increasingly use digital devices in our daily lives,
    2. the wide-spread availability of GPUs that can perform a lot of multiplications and additions in parallel,
    3. the structure of ML models that require tremendous amounts of data to converge and can leverage GPUs to speed up the process.

- ML models perform only as good as the training data is.
Thus, they are subject to many problems, one of which is bias.
One of the projects exploring the effects of bias in training an ML model is [Norman from MIT Media Lab](http://norman-ai.mit.edu/).
Such failure is trivial in the case of Norman, but it can happen silently on rare corner cases, and sometimes failure can lead to embarassing or catastrophic outcomes (think of recent autonomous car crashes).

- Heuristics for building models remain an art for some and black magic for others.
We can explain the choice of layers adhoc, but upfront design is still impossible.
First, we started with perceptrons, and quickly moved to convolutional networks.
Then, we started adding more layers, so we called it Deep Learning (see [AlexNet](https://papers.nips.cc/paper/4824-imagenet-classification-with-deep-convolutional-neural-networks.pdf), [VGG](https://arxiv.org/pdf/1409.1556.pdf)).
We realized that deeper networks are harder to train, so we started propagating residuals (see [ResNet](https://arxiv.org/pdf/1512.03385.pdf)).
Then, we went for wider layers (see [Wide ResNet](https://arxiv.org/pdf/1605.07146.pdf)).
What's next? [Stacking networks](https://arxiv.org/pdf/1805.08559.pdf)? [Capsule networks](https://arxiv.org/pdf/1710.09829.pdf)? [Geometric deep learning](https://arxiv.org/pdf/1611.08097.pdf)? [Bayesian deep learning](http://bayesiandeeplearning.org/)?

- ML models are hard to debug.
When classical software fails, we can step through the code, and try to make sense of the execution path, looking for the root cause.
When an ML model fails, good luck stepping through the layers.
The field still needs to mature before efficient debugging strategies develop.

- Recent research suggests that most papers are [overfitting standard datasets](https://arxiv.org/pdf/1806.00451.pdf).
Which means these models don't generalize as well as we might think.

- Current ML models lack the ability to handle uncertainty.
This is why some advocate for Bayesian approaches to deep learning (see [Alex Kendall's blog on the subject](https://alexgkendall.com/computer_vision/bayesian_deep_learning_for_safe_ai/)).

- [Machine Learning brings technical debt](https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/43146.pdf) to your software, that's a big deal for software maintainability.
The quick fantastic results brought by a machine learned solution comes at a cost, there is no free lunch.
As the field is still not mature, one needs to be extremely careful when shipping ML in their products, and take time to build solid foundations for data collection, data cleaning, and model deployment.

- ML will not replace all software, but everyone wants to believe the opposite.
Some self-driving car companies have chosen end-to-end approaches to build autopilots since the ML hype started.
Neglecting classical statistics, uncertainty modeling and decision making methods known since decades.
Instead, ML models are designed, data is thrown at them from simulations or recorded human driving sessions, and the pot is stirred until convergence.

<figure>
    <center>
    <img src="https://imgs.xkcd.com/comics/machine_learning.png" alt="XKCD on Machine Learning">
    </center>
</figure>

# Playing with fire
As software engineers, we are lazy.
We see this technology and wonder if we just could throw data at it and let it figure out the solution for us.
It's a legitimate question to ask.
However, when building safety critical systems such as car autopilots, some upfront system design and thinking needs to go into it.
We know from previous decades of research about control systems, state estimation, optimization methods, and many more ways to solve subproblems relevant to building a car autopilot.
And throwing data at an ML model until it figures out how to fly a plane, is not necessarily a good approach for an engineer.
It might make a good publication, but it probably won't pass certification and safety testing.

So why do we overlook decades of research and proven technology?
Then we wonder why an autonomous car kills a pedestrian after spending 5 seconds trying to figure out what object it was seeing (i.e. the pedestrian), before deciding to break 1 second before impact (see [Uber accident report](https://www.ntsb.gov/news/press-releases/Pages/NR20180524.aspx)).
According to the crash report, the car identified the pedestrian successively as an unknown object, a vehicle, and a bicycle.
All of these sound like obstacles: as in objects you don't want to crash into.
And that's not the first loss due to a faulty car autopilot, remember [Tesla's accident back in 2016](https://www.tesla.com/en_GB/blog/tragic-loss).

A safer way to program autonomous navigation is by cascading two tasks:
- a high-level path planning task responsible for computing a path from A to B, and
- a low-level task for obstacle avoidance responsible for braking when an obstacle is detected on a collision path, then letting the path planning task compute a new path around it.
So why not adopt a similar strategy?
Instead of letting the machine learn it.. hopefully.. some day.. maybe?

# Step back and look at the bigger picture
ML won't solve all our problems, but it sure does solve many that were unsolved until now (e.g. image classification).
Now what? Are you telling me to throw all of that away?
No.
Consider ML at it's real value: a powerful tool in your toolbox as a software engineer.
Just like other forgotten tools from the "past": graph-based decision making, hidden Markov models, Bayesian approaches, probabilistic graphs, etc.

The end of the hype means we can take a step back, and consider all the tools at hand to solve real-world problems.
None of them solve all our problems.
None of them are completely useless.
We should consider all our tools, weigh their pros and cons, and decide on a case-by-case basis which one fits solves best the problem at hand.

# The future of programming
There are still many problems out there to be solved using software.
To ML or not to ML becomes an irrelevant question to ask, it's a matter of implementation.
One thing is sure: if we want to solve problems in the real world we will need good abstractions and powerful tools.

As we build more complex software to solve more difficult problems, we need to compose various mathematical models in our software.
Erik Meijer has been recently talking at length about introducing type constructors to define probabilities (probability monads) in our code (see [talk at KotlinConf 2017](https://www.youtube.com/watch?v=NKeHrApPWlo), and talk at IC Research day EPFL 2018).
He argues this is needed to pave way for [Software 2.0](https://medium.com/@karpathy/software-2-0-a64152b37c35).

<figure>
    <center>
    <img src="https://pbs.twimg.com/media/DfGO1cvXcAApoyb.jpg" alt="Erik Meijer and the Bayes theorem">
    <figcaption>
        <small>Erik Meijer and Bayes' theorem at EPFL</small>
    </figcaption>
    </center>
</figure>

Probabilistic programming more than ML is more crucial to modern software.
Probabilities are at the core of many models we implement to solve problems, from expressing uncertainty of results to accounting for noisy sensor measurements.
To tackle this:
- we need more math
- we need category theory to structure and generalize our programs
- we need language support for [automatic differentiation](http://www.robots.ox.ac.uk/~tvg/publications/talks/autodiff.pdf) as gradient-based methods become ubiquitous  (see Differentiable Programming)

From that we can compose models in our code: plugging in probabilistic models (e.g. ML, state estimation) with deterministic code (e.g. control systems, database queries, UI interactions) to build great software applications.

But why would you listen to a random guy on the internet?
