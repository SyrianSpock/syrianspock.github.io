---
layout: post
title: From markdown to reveal.js slides with pandoc
categories: ['Software']
tags: ['software', 'programming', 'tools']
---

[Pandoc](https://pandoc.org/) is a universal converter for markup languages (markdown, HTML, LaTeX, etc).
It's one of those tools that change your life when you discover them.
I use it regularly to create various documents from LaTex/docx reports to beamer/reveal.js slides, usually from markdown.
[Pandoc](https://github.com/jgm/pandoc) is also one of the biggest open-source Haskell projects, what's not to love about that?

This post describes my workflow for generating reveal.js slides from markdown, because writing HTML by hand in 2018 is boring.

# Starting with vanilla slides

Producing slides from markdown is straightforward with pandoc.
You don't have to take my word for it, just search for "pandoc markdown slides" or read the [pandoc manual](http://pandoc.org/MANUAL.html#producing-slide-shows-with-pandoc).

We start with a `slides.md` file containing a header of the form

```md
---
author: John Smith
title: Some slides
date: 01.01.1970
---
```

that defines some metadata used to generate the first slide.
Then we can add more slides using `---` or by simply starting a new section with a title `#`.
Filling in our slides with some dummy content, we end up with this markdown

```md
---
author: John Smith
title: Some slides
date: 01.01.1970
---

# Let's start a section

## Then put subsections

with some text

# Start a new section

## Make a list

- of items
- that have no meaning
- or coherence
- with **bold text**
- or _emphasized text_

## Show pictures

![Wikipedia FTW](https://www.wikipedia.org/portal/wikipedia.org/assets/img/Wikipedia-logo-v2.png)

from local path or from the internet

# {data-background-image="https://upload.wikimedia.org/wikipedia/commons/e/ea/Thats_all_folks.svg"}
```

We can already generate slides

```bash
pandoc -t revealjs \
    --standalone \
    --variable revealjs-url="https://revealjs.com" \
    slides.md -o slides.html
```

We get a very simple that can be viewed [here](/resources/pandoc-slides/vanilla.html).

# Customizing slides with templates

This is all nice, but how can I change that title font size? or change the theme of my slides?
Customization is well documented in [pandoc's user manual](http://pandoc.org/MANUAL.html#variables-for-slides).
These go through variables

You can also define a template to be applied to your slides and apply it using the `--template` when running pandoc.
My current template is [on github](https://github.com/SyrianSpock/grimoire/blob/master/pandoc-slides/template-revealjs.html) if you want to reuse it.

Now running pandoc again with my customized workflow becomes

```bash
pandoc -t html5 \
    --standalone \
    --section-divs \
    --template=template-revealjs.html \
    --variable theme="black" \
    --variable transition="linear" \
    --variable revealjs-url="https://revealjs.com" \
    --from markdown-markdown_in_html_blocks+raw_html \
    slides.md -o slides.html
```

Which yields [these slides](/resources/pandoc-slides/customized.html).

Let's break it down
- `-t html5` tells pandoc to produce html5, since our template will enforce this to be revealjs slides
- `--standalone` produces a single HTML file that is standalone (not counting images linked or resources like revealjs source code)
- `--section-divs` will properly wrap our sections with `<div>`
- `--template=template-revealjs.html` sets our custom template
- `--variable theme="black"` sets the theme
- `--variable transition="linear"` sets the transition style between slides
- `--variable revealjs-url="https://revealjs.com"` gives the URL or path where to find the revealjs source code
- `--from markdown-markdown_in_html_blocks+raw_html` allows us to write raw HTML in our slides in addition to regular markdown

The full example is also documented in my personal grimoire [on github](https://github.com/SyrianSpock/grimoire/tree/master/pandoc-slides#generating-slides-with-pandoc).

That's all folks!
For real this time.
As you can see my workflow is pretty simple, thanks to pandoc.
As the metaphor of the dwarfs standing on the shoulder of giants goes _nanos gigantum humeris insidentes_.
We get the best of both worlds: simple yet beautiful slides with minimal effort and a fully reproducible process.
