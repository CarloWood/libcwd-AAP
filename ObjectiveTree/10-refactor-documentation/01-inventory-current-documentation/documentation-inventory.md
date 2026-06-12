# Documentation baseline inventory

Current objective: establish what the existing documentation build consumes and produces before the refactor. This is a behavior/content inventory, not a layout contract.

## Preservation boundary for later work

The chosen modernization direction is to keep Doxygen as the generator and adopt the Doxygen Awesome theme. The baseline below must not be interpreted as requiring preservation of the current visual layout, frames/table-era HTML, CSS class names, JavaScript loader, image placement, link paths, or page structure.

For now, preserve literal user-facing English text and the semantic documentation inventory. Everything related to markup, layout, navigation placement, stylesheet organization, generated file names, and link targets may change as needed for the new theme and information architecture.

Specific `doc/index.html` rule from the analyst/user discussion:

- News dates and release-note prose are content to preserve, e.g. dates such as `28 Jul 2009` and the release-note sentences following them.
- The home-page table of contents must still expose five links with titles `Tutorial`, `Reference Manual`, `Quick Reference`, `github`, and `Download`.
- The exact HTML, CSS classes, ToC location, and link destinations are not preservation requirements.

## Documentation build entry points

- Top-level CMake always enters `doc/` via `add_subdirectory(doc)` in `$REPOROOT/CMakeLists.txt`.
- `doc/CMakeLists.txt` requires `doxygen`, `m4`, and `awk`; if any are missing, the `doc` target prints the missing-tool values and fails.
- Main documentation target: `doc`.
- Subtargets/dependencies:
  - `doc-assets`: copies `doc/index.html`, `doc/doxygen-examples/`, `doc/images/`, `doc/scripts/`, and `doc/www/` into `$BUILDDIR/doc/`.
  - `doc-styles`: generates browser-specific legacy CSS outputs from `doc/styles/*.css` plus `doc/styles/defines.h` using `doc/scripts/generate_style.sh`.
  - `doc-external`: copies `$REPOROOT/INSTALL`, `$REPOROOT/example-project/sys.h`, `$REPOROOT/example-project/debug.h`, and `$REPOROOT/libcwdrc` into `$BUILDDIR/doc/external/` for reference from docs.
  - `doc-reference-manual`: runs Doxygen from `$BUILDDIR/doc` using `$BUILDDIR/doc/doxygen.config`, then runs two post-processing shell snippets over generated reference-manual HTML.
  - `doc-tutorial`: builds tutorial HTML pages and generated tutorial examples through `doc/tutorial/CMakeLists.txt`.

Current tools found during inventory:

- `doxygen`: `/usr/bin/doxygen`, version `1.16.1`.
- `m4`: `/usr/bin/m4`.
- `awk`: `/usr/bin/awk`.
- `cmake`: `/usr/bin/cmake`.
- `ninja`: `/usr/bin/ninja`.

## Doxygen reference-manual baseline

Configuration template: `doc/doxygen.config.in`; configured output: `$BUILDDIR/doc/doxygen.config`.

Important current settings:

- `QUIET = YES`.
- `WARNINGS = YES`.
- `WARN_IF_UNDOCUMENTED = NO`.
- `WARN_IF_DOC_ERROR = YES`.
- `WARN_IF_INCOMPLETE_DOC = YES`.
- `WARN_NO_PARAMDOC = NO`.
- `WARN_AS_ERROR = NO`.
- `INPUT = $REPOROOT/src/debug.cxx`, `$REPOROOT/doc`, `$REPOROOT/include/libcwd`, `$BUILDDIR/include/libcwd`, `$REPOROOT/src/demangle.cxx`.
- `FILE_PATTERNS = *.dox *.h *.inl`.
- `RECURSIVE = NO`.
- `GENERATE_HTML = YES`.
- `HTML_OUTPUT = reference-manual`.
- `HTML_HEADER = $BUILDDIR/doc/html.header`.
- `HTML_FOOTER = $BUILDDIR/doc/html.footer`.
- `HTML_EXTRA_STYLESHEET` and `HTML_EXTRA_FILES` are currently empty.
- `DISABLE_INDEX = YES`.
- `GENERATE_TREEVIEW = NO`.
- `FULL_SIDEBAR = NO`.
- `SEARCHENGINE = NO`.

Current custom header/footer sources are `doc/html.header.in` and `doc/html.footer.in`. They contain legacy HTML 4 transitional markup, direct references to `../scripts/detect_browser.js` and `../scripts/load_style_sheets.js`, and legacy top navigation links. These are layout/theme implementation details, not preservation requirements.

`doc/mainpage.dox` defines the reference manual landing page with four semantic sections:

- `Writing Debug Output`.
- `Program Symbols Access And Interpretation`.
- `Miscellaneous Runtime`.
- `Configuration, Installation And Getting Started`.

Reference-manual source pages currently in `doc/*.dox`:

- `attach_gdb.dox`
- `control_flags.dox`
- `core_dump.dox`
- `custom-debug.h.dox`
- `custom_do.dox`
- `debug_channels.dox`
- `debug_object.dox`
- `destination.dox`
- `downloading.dox`
- `environment.dox`
- `fatal_output.dox`
- `formatting.dox`
- `function.dox`
- `gdb.dox`
- `locations.dox`
- `mainpage.dox`
- `modules.dox`
- `namespaces.dox`
- `nested.dox`
- `preparation.dox`
- `rcfile.dox`
- `special.dox`
- `symbols_intro.dox`
- `type_info.dox`
- `why_macro.dox`
- `writing_intro.dox`

Observed Doxygen grouping/page declarations include:

- Top-level groups from `doc/modules.dox`: `book_writing`, `book_runtime`, `book_symbols`, `book_start`.
- Main page from `doc/mainpage.dox`: `Reference Manual: Table Of Contents`.
- Standalone Doxygen pages: `preparation`, `downloading`, `page_why_macro`.
- Chapter/group pages for debug output, symbols, runtime, environment, custom debug header, destination, debug object, debug channels, control flags, formatting, fatal output, nesting, special functions, gdb, rcfile, core dumps, attach-gdb, and related topics.

## Static home/download/quick-reference assets

Static files copied by `doc-assets`:

- `doc/index.html`: current home page and news archive. Preserve user-facing prose/dates for now; do not preserve the current table-based layout as a constraint.
- `doc/www/quickreference.html`: quick reference page.
- `doc/www/exclude`: copied as part of `www/`.
- `doc/doxygen-examples/main-examples.cc`: copied for Doxygen/example use.
- `doc/images/`: `alinoe.png`, `ball_neon_gimp.gif`, `css.gif`, `libcwd_logo.png`, `sflogo.png`, `toc.png`, `wink.gif`, plus `buttons/lr_index.png`, `buttons/lr_next.png`, `buttons/lr_prev.png`, and `lines/cat.png`, `lines/caterpil.png`, `lines/ghost.png`, `lines/hippo.png`, `lines/mouse.png`, `lines/owl.png`, `lines/snail.png`.
- `doc/scripts/`: `break_out_of_frame.js`, `detect_browser.js`, `load_style_sheets.js`, `xbDebug.js`, `extract_tutorial_block.sh`, `generate_style.sh`.

Legacy visual/browser assets, including old browser CSS variants, JavaScript browser detection, frame-breaking script, SourceForge-era images, and table-layout images, are baseline implementation details only. They may be removed, replaced, or reorganized if their user-facing content/intent is covered by the new Doxygen Awesome layout.

## Legacy stylesheet generation

Current style templates:

- `doc/styles/main.css`
- `doc/styles/doxygen.css`
- `doc/styles/tutorial.css`
- `doc/styles/tag-cw.css`
- `doc/styles/defines.h`

`doc/scripts/generate_style.sh` preprocesses each CSS template with the C compiler for these browser symbols:

- `netscape4`
- `netscape6`
- `mozilla`
- `konqueror`
- `ie`

This currently produces four CSS files under each browser directory in `$BUILDDIR/doc/styles/`. This browser matrix is not a preservation requirement for the Doxygen Awesome refactor.

## Tutorial source and generation baseline

Tutorial source directory: `doc/tutorial/`.

Static m4 tutorial pages:

- `index.m4`
- `faq.m4`
- `intro.m4`
- `tut1.m4`
- `tut2.m4`
- `tut3.m4`
- `tut4.m4`
- `tut6.m4`

Shared m4 definitions: `doc/definitions.m4`. These contain legacy HTML header/footer/page macros and old copyright/header markup; treat them as current implementation, not as required future layout.

Special tutorial page: `doc/tutorial/tut5.in`.

`tut5.in` contains named `<!-- START CODE ... -->` / `<!-- END CODE ... -->` blocks. `doc/scripts/extract_tutorial_block.sh` extracts those blocks, decodes HTML entities, strips highlighting spans, writes generated `.cc`/`.h` files under `$BUILDDIR/doc/tutorial/examples5/`, and the CMake target compiles the generated examples.

Generated tutorial code blocks currently include:

- `examples5/test5.1.1.cc`
- `examples5/debug.h`
- `examples5/debug.cc`
- `examples5/test5.3.1.cc`
- `examples5/test5.3.2.cc`
- `examples5/test5.4.1.cc`
- `examples5/test5.4.2.cc`
- `examples5/test5.4.3.cc`
- `examples5/test5.4.4.cc`
- `examples5/test5.4.5.cc`
- `examples5/test5.4.6.cc`

Tutorial example build behavior:

- `doc-tutorial-examples5-debug` builds generated `examples5/debug.cc` as an object library with `CWDEBUG`.
- Each generated `test*.cc` source becomes an executable target named `doc-tutorial-test...`, linked against `cwd`, with runtime output under `$BUILDDIR/doc/tutorial/examples5/` and output name `test...-bin`.
- `doc/tutorial/moo.awk` expands `<!-- START OUTPUT ... -->` blocks by executing the command, escaping HTML-sensitive characters, and inserting the output into `tut5.m4`.
- `tut5.html` is generated from the generated `$BUILDDIR/doc/tutorial/tut5.m4` through `m4`.

Additional tutorial source files present but not generated from `tut5.in`:

- `channel.cc`
- `hello_world.cc`
- `log_file.cc`
- `examples5/sys.h`
- `examples5/sysinclude/libbooster/debug.h`
- `examples5/sysinclude/libturbo/debug.h`

## Generated-output baseline

After a clean documentation build, `$BUILDDIR/doc/` contains:

- Copied static home page: `index.html`.
- Copied static/support directories: `doxygen-examples/`, `external/`, `images/`, `scripts/`, `styles/`, `www/`.
- Configured Doxygen files: `doxygen.config`, `html.header`, `html.footer`.
- Generated reference manual: `reference-manual/` with 149 `.html` files and 236 entries total at inventory time.
- Generated tutorial: `tutorial/` with 9 `.html` files (`index.html`, `faq.html`, `intro.html`, `tut1.html` through `tut6.html`, including generated `tut5.html`) and 14 entries total at inventory time.
- Legacy generated style outputs: 5 browser directories (`ie`, `konqueror`, `mozilla`, `netscape4`, `netscape6`) with 4 CSS files each.

These generated paths/counts are an inventory snapshot for comparison, not a requirement that the refactor retain the same output tree.

## Current warning baseline

Commands run during inventory:

```sh
cmake --build "$BUILDDIR" --target doc --clean-first
```

Result: completed successfully through 115 build steps and printed no warnings.

```sh
cd "$BUILDDIR/doc" && doxygen doxygen.config
```

Result: completed successfully and printed no output/warnings.

```sh
cmake --build "$BUILDDIR" --target doc
```

Result: completed successfully. Because `doc-assets` is a custom target without declared outputs, it ran again and copied static assets; it printed no warnings.

## Known stale/risky content to revisit later

This section identifies content or implementation areas that look stale or fragile. It does not require preserving the current presentation.

- `doc/index.html` is legacy HTML 4 transitional with table layout, browser-detection JavaScript, and old style-loader assumptions. Preserve news dates/prose for now; freely modernize markup/layout later.
- `doc/html.header.in`, `doc/html.footer.in`, and `doc/definitions.m4` embed old HTML and navigation conventions that likely conflict with Doxygen Awesome defaults.
- Legacy browser-specific CSS generation for Netscape, Konqueror, Mozilla, and IE is obsolete as a visual strategy.
- `doc/scripts/detect_browser.js`, `load_style_sheets.js`, and `break_out_of_frame.js` are layout/theme-era support scripts; later objectives should decide whether Doxygen Awesome makes them unnecessary.
- Static images such as `sflogo.png`, `css.gif`, old buttons, and decorative line images may be historical/layout artifacts rather than documentation content.
- Some user-facing prose mentions old build/distribution history, Autotools options, CVS/SVN, GCC versions, and old library releases. Preserve the literal English text until a later content-refresh objective explicitly updates it.
- `doc/tutorial/moo.awk` executes commands embedded in tutorial comments to capture output.
- Doxygen post-processing in `doc/CMakeLists.txt` edits generated HTML with shell/sed after Doxygen runs. This may be incompatible with changed generated markup and should be re-evaluated during theme modernization.
