# Legacy Doxygen page hierarchy

This is the selected child-to-parent tree for the 38 tags listed in `../01-establish-legacy-group-and-page-structure/legacy-overview.md`. Cross-reference links that do not define the selected parent are ignored. Doxygen/mainpage references to generated nodes outside that overview, such as `group_default_dc`, `group_annotation`, `group_demangle`, and `group_configuration`, are also omitted.

The result is a tree rooted at `mainpage`, with a few top-level synthetic edges where the legacy data exposes a page as top-level but has no parent link to it.

```text
mainpage
  book_writing
    book_writing_intro
      page_why_macro
    group_destination
    group_debug_object
      chapter_custom_do
    group_debug_channels
    group_control_flags
    group_formatting
    group_fatal_output
    chapter_nesting
    group_special
  book_allocations
    book_allocations_intro
    chapter_validation
    chapter_magic_numbers
    group_overview
      group_invisible
      group_alloc_format
    group_finding
    group_markers
  book_symbols
    chapter_symbols_intro
    group_locations
    group_type_info
  book_start
    chapter_custom_debug_h
      preparation
    chapter_alloc_locations
    chapter_environment
  book_runtime
  chapter_rcfile
  chapter_attach_gdb
  chapter_core_dump
  chapter_gdb
  group_function
  downloading
```

## Selected parent edges

| CHILD | PARENT | BASIS |
| --- | --- | --- |
| `book_writing` | `mainpage` | Synthetic top-level group; `mainpage.dox` has an `<h2>` section with the same title, but no `\ref book_writing`. |
| `book_allocations` | `mainpage` | Synthetic top-level group; `mainpage.dox` has an `<h2>` section with the same title, but no `\ref book_allocations`. |
| `book_symbols` | `mainpage` | Synthetic top-level group; `mainpage.dox` has an `<h2>` section with the same title, but no `\ref book_symbols`. |
| `book_start` | `mainpage` | Synthetic top-level group; `mainpage.dox` has an `<h2>` section with the same title, but no `\ref book_start`. |
| `book_runtime` | `mainpage` | Synthetic top-level group; `mainpage.dox` has an `<h2>` section with the same title, but no `\ref book_runtime`. |
| `book_writing_intro` | `book_writing` | `writing_intro.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `group_destination` | `book_writing` | `destination.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `group_debug_object` | `book_writing` | `debug_object.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `chapter_custom_do` | `group_debug_object` | `mainpage.dox` nests `\ref chapter_custom_do` below `\ref group_debug_object`; `debug_object.dox` also links to `chapter_custom_do`. |
| `group_debug_channels` | `book_writing` | `debug_channels.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `group_control_flags` | `book_writing` | `control_flags.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `group_formatting` | `book_writing` | `formatting.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `group_fatal_output` | `book_writing` | `fatal_output.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `chapter_nesting` | `book_writing` | `nested.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `group_special` | `book_writing` | `special.dox` has `\ingroup book_writing`; `group__book__writing.html` links to this child. |
| `page_why_macro` | `book_writing_intro` | `writing_intro.dox` links to `\ref page_why_macro`; `group__book__writing__intro.html` links to `page_why_macro.html`. |
| `book_allocations_intro` | `book_allocations` | `alloc_intro.dox` has `\ingroup book_allocations`; `group__book__allocations.html` links to this child. |
| `chapter_validation` | `book_allocations` | `deallocation_pointer_validation.dox` has `\ingroup book_allocations`; `group__book__allocations.html` links to this child. |
| `chapter_magic_numbers` | `book_allocations` | `magic.dox` has `\ingroup book_allocations`; `group__book__allocations.html` links to this child. |
| `group_overview` | `book_allocations` | `allocated_memory_overview.dox` has `\ingroup book_allocations`; `group__book__allocations.html` links to this child. |
| `group_invisible` | `group_overview` | `invisible.dox` has `\ingroup group_overview`; `group__group__overview.html` links to this child. |
| `group_alloc_format` | `group_overview` | `alloc_format.dox` has `\ingroup group_overview`; `group__group__overview.html` links to this child. |
| `group_finding` | `book_allocations` | `finding.dox` has `\ingroup book_allocations`; `group__book__allocations.html` links to this child. |
| `group_markers` | `book_allocations` | `memory_leak_checking.dox` has `\ingroup book_allocations`; `group__book__allocations.html` links to this child. |
| `chapter_symbols_intro` | `book_symbols` | `symbols_intro.dox` has `\ingroup book_symbols`; `group__book__symbols.html` links to this child. |
| `group_locations` | `book_symbols` | `locations.dox` has `\ingroup book_symbols`; `group__book__symbols.html` links to this child. |
| `group_type_info` | `book_symbols` | `type_info.dox` has `\ingroup book_symbols`; `group__book__symbols.html` links to this child. |
| `chapter_custom_debug_h` | `book_start` | `custom-debug.h.dox` has `\ingroup book_start`; `group__book__start.html` links to this child. |
| `preparation` | `chapter_custom_debug_h` | `custom-debug.h.dox` has `\sa \ref preparation`; `group__chapter__custom__debug__h.html` links to `preparation.html`. |
| `chapter_alloc_locations` | `book_start` | `location.dox` has `\ingroup book_start`; `group__book__start.html` links to this child. |
| `chapter_environment` | `book_start` | `environment.dox` has `\ingroup book_start`; `group__book__start.html` links to this child. |
| `chapter_rcfile` | `mainpage` | `mainpage.dox` links to `\ref chapter_rcfile`; no `\ingroup book_runtime` exists. |
| `chapter_attach_gdb` | `mainpage` | `mainpage.dox` links to `\ref chapter_attach_gdb`; no `\ingroup book_runtime` exists. |
| `chapter_core_dump` | `mainpage` | `mainpage.dox` links to `\ref chapter_core_dump`; no `\ingroup book_runtime` exists. |
| `chapter_gdb` | `mainpage` | `mainpage.dox` links to `\ref chapter_gdb`; no `\ingroup book_runtime` exists. |
| `group_function` | `mainpage` | Synthetic top-level topic; `topics.html` lists it at top level and no `\ref group_function` or parent `\ingroup` was found in `legacy-doc/*.dox`. |
| `downloading` | `mainpage` | Synthetic related page; `pages.html` lists it, but no `\ref downloading` or parent `\ingroup` was found in `legacy-doc/*.dox`. |

## Notes on non-selected links

- `custom_do.dox` declares `\ingroup group_destination`, so Doxygen's group hierarchy can also place `chapter_custom_do` under `group_destination`. The selected navigation parent is `group_debug_object`, because `mainpage.dox` nests it there and `debug_object.dox` links to it as the custom-debug-object expansion of the debug-object concept.
- `book_runtime` is a generated group page from `modules.dox`, but it is empty and does not link to `chapter_rcfile`, `chapter_attach_gdb`, `chapter_core_dump`, or `chapter_gdb`; those chapters are therefore direct `mainpage` children in the selected tree.
- `downloading`, `group_function`, and the five `book_*` group pages have no inbound parent link from another overview page. They are attached to `mainpage` to keep the overview single-rooted.

The following is manually editted and take precedence over the above:
```text
mainpage
  book_writing                  // Has title "Writing Debug Output", same as <h2> header in mainpage.dox
    book_writing_intro          // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
      page_why_macro
    group_destination           // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
    group_debug_object          // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
      chapter_custom_do         // <ul><li> under group_debug_object
    group_debug_channels        // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
      group_default_dc          // <ul><li> under group_debug_channels
    group_control_flags         // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
    group_formatting            // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
    group_fatal_output          // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
    chapter_nesting             // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
    group_special               // <li> under <h2>Writing Debug Output</h2> in mainpage.dox
  book_allocations              // Has title "Memory Allocation Debug Support", same as <h2> header in mainpage.dox.
    book_allocations_intro      // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
    chapter_validation          // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
    chapter_magic_numbers       // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
    group_annotation            // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
    group_overview              // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
      group_invisible           // <ul><li> under group_overview
      group_alloc_format
    group_finding               // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
    group_markers               // <li> under <h2>Memory Allocation Debug Support</h2> in mainpage.dox
  book_symbols                  // Has title "Program Symbols Access And Interpretation", same as <h2> header in mainpage.dox.
    chapter_symbols_intro       // <li> under <h2>Program Symbols Access And Interpretation</h2> in mainpage.dox
    group_locations             // <li> under <h2>Program Symbols Access And Interpretation</h2> in mainpage.dox
    group_type_info             // <li> under <h2>Program Symbols Access And Interpretation</h2> in mainpage.dox
      group_demangle            // <ul><li> under group_type_info
  book_start                    // Has title "Configuration, Installation And Getting Started", same as <h2> header in mainpage.dox.
    downloading
    preparation
    chapter_custom_debug_h      // <li> under <h2>Configuration, Installation And Getting Started</h2> in mainpage.dox
    chapter_alloc_locations     // <li> under <h2>Configuration, Installation And Getting Started</h2> in mainpage.dox
    group_configuration         // <li> under <h2>Configuration, Installation And Getting Started</h2> in mainpage.dox
      enable_alloc              // These are linked to as href="$group__enable__alloc.html" etc in group__group__configuration.map
      enable_debug
      enable_debugm
      enable_debug_output
      enable_glibcxx_debug
      enable_libbfd
      enable_location
      enable_magic
      enable_marker
    chapter_environment         // <li> under <h2>Configuration, Installation And Getting Started</h2> in mainpage.dox
  book_runtime                  // Parent because the title is "Miscellaneous Runtime", same as <h2> header in mainpage.dox.
    chapter_rcfile              // <li> under <h2>Miscellaneous Runtime</h2> in mainpage.dox
    chapter_attach_gdb          // <li> under <h2>Miscellaneous Runtime</h2> in mainpage.dox
    chapter_core_dump           // <li> under <h2>Miscellaneous Runtime</h2> in mainpage.dox
    chapter_gdb                 // <li> under <h2>Miscellaneous Runtime</h2> in mainpage.dox
  group_function
```
