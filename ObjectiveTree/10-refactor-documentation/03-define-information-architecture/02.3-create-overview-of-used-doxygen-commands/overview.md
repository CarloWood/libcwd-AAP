# Overview of Doxygen commands used in libcwd

Scope: `doc/`, `include/`, `src/`, `nodebug.h`, and `example-project/`.

The project uses **both** the `@` and `\` command prefixes — structural commands
in `.md`/headers lean `@`, while inline formatting inside source comments leans
`\`. Counts are approximate (raw `[@\\]<word>` matches, de-duplicated by command
name).

The following apparent matches were excluded as noise:

- CMake/config substitution variables: `@srcdir@`, `@top_srcdir@`,
  `@PACKAGE_NAME@`, `@PACKAGE_VERSION@`, `@PROJECT_NAME@`, `@CWDEBUG@`, etc.
- C escape sequences (`\t \r \f \b \e`) that only appear *documented* inside
  `\c \\X` constructs in `buf2str.h` / `char2str.h`.
- Source-string `\n` escapes (the genuine doxygen `\n` newline command is
  nevertheless used in docs, e.g. `fatal_output.md`).

## Structural / scope commands

What is being documented, and where it sits in the group/page hierarchy.

| command          | ~count | example                                           |
|------------------|--------|---------------------------------------------------|
| `defgroup`       | 35     | `@defgroup reference-manual …` (reference-groups.md) |
| `ingroup`        | 54     | `@ingroup reference-manual`                       |
| `addtogroup`     | 42     | `@addtogroup group_debug_channels`                |
| `page`           | 1      | `@page release-notes Release notes`               |
| `section`        | 3      | `\section section_nesting_calling …` (nested.md)  |
| `subsection`     | 4      | `\subsection applications Applications`           |
| `anchor`         | 2      | `\anchor preparation_step2`                       |
| `file`           | 32     | per-header file documentation                     |
| `class`          | 7      | `\class buf2str`                                  |
| `namespace`      | 3      | `@namespace libcwd::channels::dc` (namespaces.md) |
| `fn`             | 3      | `\fn Channel* find_channel(…)`                    |
| `def`            | 5      | `\def Debug` (debug.h)                            |

## Block / conditional

- `internal` / `endinternal` (10 / 1)
- `cond` / `endcond` (1 / 1)
- `code` / `endcode` (1 / 1) — note: code samples mostly use markdown
  ```` ```cpp ```` fences rather than this command.

## Content / cross-reference

- `brief` (153) — by far the most used command.
- `ref` (102).
- `link` / `endlink` (35 / 35).
- `sa` (28) — "see also".
- `return` / `returns` (1 / 2).
- `param` (1 — only in `example-project/cwds/debug.cxx`).
- `par` (18) — paragraph with an optional heading.
- `n` — forced newline (used in docs; count inflated by `\n` escapes in source
  strings).

## Inline formatting

- `c` (41) — typewriter, e.g. `\c ostream`.
- `p` (12) — typewriter, e.g. `\p input`.
- `a` — italic emphasis, e.g. `\a label`.
- `em` (5) — emphasis, e.g. `\em mangled`.

## Raw passthrough

- `htmlonly` / `endhtmlonly` (21 / 21).
- `latexonly` / `endlatexonly` (1 / 1).
- `include` (2).

## Custom aliases (defined in `doc/doxygen.config.in`)

- `exampleoutput` / `endexampleoutput` (10) — wraps example output in
  `<DIV class=example-output>`.
- `shellcommand` / `endshellcommand` (6) — wraps inline shell commands in
  `<SPAN class=shell-command>`.
- `filename` / `endfilename` (3) — wraps a filename in
  `<SPAN class=filename>`.

## Notably absent

Common Doxygen commands **not** used in the project: `note`, `warning`, `todo`,
`deprecated`, `throws` / `throw`, `tparam`, and `see` (the project consistently
uses `sa` instead of `see`).

## Observation

`\param` is used only once and only in the example project. The library headers
document parameters through prose and `\a` / `\p` rather than `\param`, so
Doxygen's generated parameter tables are not really being produced.
