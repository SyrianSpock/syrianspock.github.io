---
layout: post
title: From git submodule to subtree, rocky road to monorepo
categories: ['Software']
tags: ['software', 'programming', 'tools']
---

Over my short but intensive ongoing life as a programmer, I have worked on different projects with different source management strategies.
From a highly modular collection of dozens of small repos to monolithic monorepos.
My opinion today is that monorepos work best for small to medium teams, and allow them to move fast with low overhead.
They can scale reasonably well provided you keep your software decoupled and you build tools to ease collaboration.

One of the main projects I've been working on, for the last few years, is the robot software stack at [CVRA](https://www.cvra.ch).
We've been through many phases:

- We started off with a collection of 10+ repositories in 2014 that quickly grew to 30+ repositories over the course of a year. This was getting out of hand, and it became more and more painful to work on as we had to merge multiple pull-requests across repositories to get a bug fixed.
- After that we split the effort in two teams, one of which was working with a monorepo (also a partially different hardware configuration, read more on [CVRA's blog](https://www.cvra.ch/blog/2016/goldorak-post-mortem)). I was on team monorepo.
- From that experiment, we learned that a monorepo worked best for our small team (3 to 6 devs) and moved entirely to that scheme two years ago. See [our main repo](https://github.com/cvra/robot-software).

However the transition to monorepo is still not fully done.
We still maintain many dependencies as submodules.
Some of which were libraries we developed that could simply be merged in our monorepo.
Doing that naively would cause history loss and render bugfix upstreaming a headache.
Fortunately, git has got us covered with [subtrees](https://git-scm.com/book/en/v1/Git-Tools-Subtree-Merging).

# submodule vs subtree

When bringing an external dependency into your git repo, git offers you two solutions:
- [submodule](https://git-scm.com/docs/git-submodule)
- [subtree](https://git-scm.com/book/en/v1/Git-Tools-Subtree-Merging)

What's the difference you may ask?
Here's my short take on it, for more details, you can refer to the [official git documentation](https://git-scm.com/docs).

With **git submodule**, the external repository is not stored in your repo.
Only its metadata is (in a `.gitmodules` file).
That will tell git where to find the external repository in the directory tree, from what URL to fetch it, and which commit to checkout.
The external repository remains external, so you have to manage it separately.
When you are not maintaining that external repository, git submodule is perfectly fine.
When you are maintaining that external repository, you'll have to deal with the added overhead of managing that external repository or consider merging repos, you decide.

With **git subtree**, the external repository is merged with your repo.
All the changes are brought in, keeping history or not, up to you.
The workflow is simpler as everything is now part of the same repo.
Note: here I'm talking about the subtree merging strategy, not the contributed git subcommand also called `subtree` which was written to facilitate subtree merging and splitting (See [git subtree command](https://github.com/git/git/tree/master/contrib/subtree)).

Why prefer subtree to submodule?
- It's simpler to reason about a single repo.
- There's nothing new to learn (apart from doing the actual merge), classic git workflow applies here.
- Modifying the monorepo is enough, no need to manipulate multiple repos to fix dependencies.
- All code comes with `git clone`, no need to reach for arcane `git submodule` commands.

Additionally, merging any dependency as subtree is still feasible and surprisingly manageable.
By keeping a certain discipline in the way you commit changes to those dependencies, you can later on extract your changes back into an isolated history that can be upstreamed as bugfixes/contributions to the original repo.

# Problem

The problem I had a few days ago was the following.
We have a few libraries in [our monorepo at CVRA](https://github.com/cvra/robot-software) that are brought in as submodules.
We decided we want to reduce the complexity of our codebase, and the solution is to merge them as subtrees instead.

The common limitation of a git subtree merge is the loss of history.
Tutorials usually recommend squashing all commits into one.

Another smaller challenge, is the fact that we may have submodules spread all around our monorepo, and they have arbitrary remotes.
Nobody wants to look for information known to git manually.
So let's script a solution.

# Solution

The solution detailed here was used to merge submodules to our robot software monorepo in [this PR](https://github.com/cvra/robot-software/pull/180).
The final script is generic enough, I think, to be reused on other projects.

## Getting the remote URL

First, let's see how to retrieve the remote of a git submodule.
Remember, the default remote of a git repository is `origin`.
So we can do

```bash
$ git remote get-url origin
git@github.com:cvra/robot-software.git
```

Great, now we have the URL of our monorepo, but we want the URL of our submodule instead.
We can either run the command from the submodule's directory, which can be achieved by wrapping the `git remote` call with `pushd` + `popd` commands (or old style `cd` + `cd -`).
Or with a bit more advanced git wizardry, we can use the infamous `GIT_DIR` environment variable to make git behave as if it were in the submodule's directory (read more on [git environment variables](https://git-scm.com/book/id/v2/Git-Internals-Environment-Variables)).

So our command becomes `GIT_DIR=${LIBRARY}/.git git remote get-url origin`.
Considering an example where `LIBRARY=lib/crc` we can check for correct behavior

```bash
$ GIT_DIR=lib/crc/.git git remote get-url origin
https://github.com/cvra/crc
```

Great success!
Next challenge.

## Merging a subtree without losing history

Following the usually recommended subtree merge, one way of achieving our goal is to

```bash
# Get the submodule remote
_GIT_REMOTE=$(GIT_DIR=${LIBRARY}/.git git remote get-url origin)

# Add the submodule remote to the monorepo
git remote add -f ${LIBRARY} ${_GIT_REMOTE}
git fetch ${LIBRARY}

# Merge the subtree and commit
git merge -s subtree --no-commit --allow-unrelated-histories --squash ${LIBRARY}/${BRANCH}
git commit -m "Subtree merged in ${LIBRARY}"
```

where `LIBRARY` points to the relative path of the submodule, and `BRANCH` is the submodule branch we want to merge in (hopefully master).

With this approach, we end up with a single commit that checks in the whole submodule in our repo.
Commit history of the submodule is lost in the abyss.

Not pleased with this compromise, I dug git documentation and stackoverflow, ending up with the following approach

```bash
# Get the submodule remote
_GIT_REMOTE=$(GIT_DIR=${LIBRARY}/.git git remote get-url origin)

# Add the submodule remote to the monorepo
git remote add -f ${LIBRARY} ${_GIT_REMOTE}
git fetch ${LIBRARY}

# Merge the subtree and commit
git merge -s ours --no-commit --allow-unrelated-histories ${LIBRARY}/${BRANCH}
git read-tree --prefix=${LIBRARY}/ -u ${LIBRARY}/${BRANCH}
git commit -m "Subtree merged in ${LIBRARY}"
```

The difference is in the penultimate and antepenultimate lines.
Where we drop the `--squash` option from the merge command and perform a proper `read-tree` to complement it.
The history is now properly merged in as an unrelated branch of the main tree.

## Wrapping up

So we've solved the two subproblems stated earlier.
We can proceed to replace submodules with subtrees without losing history and minimal user input.
With minor cleanups we end up with the following `submodule_to_subtree.sh` script

```bash
#!/bin/bash

LIBRARY=$1
BRANCH=$2
BRANCH=${BRANCH:="master"}

set -eu

_GIT_SUBMODULE_SHA=$(git ls-tree master ${LIBRARY} | sed "s/\t/ /" | cut -d " " -f 3)
_GIT_REMOTE=$(GIT_DIR=${LIBRARY}/.git git remote get-url origin)

git rm -f ${LIBRARY}
git commit -m "Remove submodule ${LIBRARY}"

git remote add -f ${LIBRARY} ${_GIT_REMOTE}
git merge -s ours --no-commit --allow-unrelated-histories ${LIBRARY}/${BRANCH}
git read-tree --prefix=${LIBRARY}/ -u ${LIBRARY}/${BRANCH}
git commit -m "Subtree merged in ${LIBRARY}"

git remote rm ${LIBRARY}
```

That we can call passing two arguments:
- `LIBRARY`: the path to the submodule to be merged
- `BRANCH`: the branch of the submodule we want to merge in

That's all folks!
