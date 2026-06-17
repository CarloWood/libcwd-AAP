# Expected non-source Doxygen page overview

This table maps the tags listed in the objective description to their input file, observed/generated output, and title. The mapping was derived from `@page` / `@defgroup` directives in `doc/*.md` and `doc/reference-groups.md`, then checked against `$BUILDDIR/doc`.

Finally, it was manually editted.

| TAG | INPUT | OUTPUT | TITLE |
| --- | --- | --- | --- |
| `book_writing_intro` | `writing_intro.md` | `book_writing_intro.html` | Writing Debug Output: Introduction |
| `chapter_attach_gdb` | `attach_gdb.md` | `chapter_attach_gdb.html` | Starting A gdb Session From A Running Program |
| `chapter_core_dump` | `reference-groups.md` | `group__chapter__core__dump.html` | Making The Program Dump Core |
| `chapter_custom_debug_h` | `custom-debug.h.md` | `chapter_custom_debug_h.html` | The Custom debug.h File |
| `chapter_custom_do` | `custom_do.md` | `chapter_custom_do.html` | Custom Debug Objects |
| `chapter_environment` | `environment.md` | `chapter_environment.html` | Environment Variables |
| `chapter_gdb` | `gdb.md` | `chapter_gdb.html` | Helper functions for within gdb |
| `chapter_nesting` | `nested.md` | `chapter_nesting.html` | Nesting Debug Output |
| `chapter_rcfile` | `reference-groups.md` | `group__chapter__rcfile.html` | Runtime Configuration File (rcfile) |
| `chapter_symbols_intro` | `symbols_intro.md` | `chapter_symbols_intro.html` | Program Symbols Access And Interpretation: Introduction |
| `control_flags` | `control_flags.md` | `control_flags.html` | Control Flags |
| `core_dump` | `core_dump.md` | `core_dump.html` | Making The Program Dump Core |
| `debug_channels` | `debug_channels.md` | `debug_channels.html` | Controlling The Output Level (Debug Channels) |
| `debug_object` | `debug_object.md` | `debug_object.html` | The Output Device (Debug Object) |
| `destination` | `destination.md` | `destination.html` | Setting The Output Destination |
| `downloading` | `downloading.md` | `downloading.html` | Downloading |
| `fatal_output` | `fatal_output.md` | `fatal_output.html` | Fatal Debug Output |
| `formatting` | `formatting.md` | `formatting.html` | Format Of The Debug Output |
| `function_objects` | `function.md` | `function_objects.html` | Function objects |
| `group_configuration` | `reference-groups.md` | `group__group__configuration.html` | Configuration Options And Macros |
| `group_control_flags` | `reference-groups.md` | `group__group__control__flags.html` | Control Flags |
| `group_debug_channels` | `reference-groups.md` | `group__group__debug__channels.html` | Controlling The Output Level (Debug Channels) |
| `group_debug_object` | `reference-groups.md` | `group__group__debug__object.html` | The Output Device (Debug Object) |
| `group_default_dc` | `reference-groups.md` | `group__group__default__dc.html` | Predefined Debug Channels |
| `group_demangle` | `reference-groups.md` | `group__group__demangle.html` | demangle_type() and demangle_symbol() |
| `group_destination` | `reference-groups.md` | `group__group__destination.html` | Setting The Output Destination |
| `group_fatal_output` | `reference-groups.md` | `group__group__fatal__output.html` | Fatal Debug Output |
| `group_formatting` | `reference-groups.md` | `group__group__formatting.html` | Format Of The Debug Output |
| `group_function` | `reference-groups.md` | `group__group__function.html` | Function objects |
| `group_locations` | `reference-groups.md` | `group__group__locations.html` | Source-file:Line-number Information |
| `group_special` | `reference-groups.md` | `group__group__special.html` | Special Functions And Utilities |
| `group_type_info` | `reference-groups.md` | `group__group__type__info.html` | Getting Type Information Of Types And Symbols |
| `locations` | `locations.md` | `locations.html` | Source-file:Line-number Information |
| `page_why_macro` | `why_macro.md` | `page_why_macro.html` | Design Consideration Concerning Macros |
| `preparation` | `preparation.md` | `preparation.html` | Preparation |
| `rcfile` | `rcfile.md` | `rcfile.html` | Runtime Configuration File (rcfile) |
| `reference-manual` | `reference-manual.md` | `reference-manual.html` | Reference Manual |
| `reference-manual-getting-started` | `reference-manual-getting-started.md` | `reference-manual-getting-started.html` | Configuration, Installation And Getting Started |
| `reference-manual-runtime` | `reference-manual-runtime.md` | `reference-manual-runtime.html` | Miscellaneous Runtime |
| `reference-manual-symbols` | `reference-manual-symbols.md` | `reference-manual-symbols.html` | Symbols Access And Interpretation |
| `reference-manual-writing-debug-output` | `reference-manual-writing-debug-output.md` | `reference-manual-writing-debug-output.html` | Writing Debug Output |
| `release-notes` | `release-notes.md` | `release-notes.html` | Release notes |
| `special` | `special.md` | `special.html` | Special Functions And Utilities |
| `type_info` | `type_info.md` | `type_info.html` | Getting Type Information Of Types And Symbols |
