# DocGen

Simple documentation generator.

## Overview

Documents use [GitHub flavoured markdown](https://guides.github.com/features/mastering-markdown/).

DocGen extracts documentation from a given list of files, and produces an equivalent markdown file for each.

Only lines beginning with /// (or any custom sequence) are included into the document.

DocGen can automatically generate links between headers in files.

## Example

```
/// # {{Header}}
///
/// Now a link to this header can be created with [[Header]].
///
/// # {{Longer Title}} With Spaces
///
/// Link to [[Longer Title]].
///
/// The link can contain spaces and doesn't need to include the entire header. It is case sensitive though.
```

## Platforms

Currently only compatible with Linux.

## Building

DocGen uses [Tup](http://gittup.org/tup/) to build. "tup" in a terminal in the project directory to build.

## Running

Run the generated bin/docgen executable in a terminal to see help and available commands

The documents for this project where generated with "./bin/docgen -d $(find src/ -iregex '.+\\..pp')"

## Possible features to add

#### Invisible link deceleration

```
# Title with hidden link !{{title.hidden}}

Possibly referenced with something like (hidden)![[title.hidden]].
```

#### HTML generation

Generate plain HTML files, instead of markdown, which can be stylised with custom css.

#### Easier to use syntax

Lines including \*, \>, and \\ can be easy to get wrong as these are special symbols and need to be
preceded with a \\ to be turned into normal characters.

Could invert the logic, such that symbols only become special when preceded with a \\, or if they are
at the start of the line.

```
\*bold\*

* list item \*1\*
* list item \*2\*
* list item \*3\*
```

This isn't much simpler than the current system though.
