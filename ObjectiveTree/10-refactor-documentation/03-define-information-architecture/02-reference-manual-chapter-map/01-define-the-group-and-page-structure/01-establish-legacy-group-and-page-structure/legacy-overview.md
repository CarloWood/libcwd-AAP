# Legacy Doxygen group and page overview

Scope: legacy manual outputs generated from `legacy-doc/*.dox` `\defgroup`, `\addtogroup`, `\page`, and `\mainpage` commands. Grouped `\page` blocks are folded into their containing group output; they are therefore listed by the group tag, not by the hidden page tag.

| TAG | INPUT | OUTPUT | TITLE |
| --- | --- | --- | --- |
| `group_alloc_format` | `alloc_format.dox` | `group__group__alloc__format.html` | Format Of The Overview Of Allocated Memory |
| `book_allocations_intro` | `alloc_intro.dox` | `group__book__allocations__intro.html` | Memory Allocation Debug Support: Introduction |
| `group_overview` | `allocated_memory_overview.dox` | `group__group__overview.html` | Overview Of Allocated Memory |
| `chapter_attach_gdb` | `attach_gdb.dox` | `group__chapter__attach__gdb.html` | Starting A gdb Session From A Running Program |
| `group_control_flags` | `control_flags.dox` | `group__group__control__flags.html` | Control Flags |
| `chapter_core_dump` | `core_dump.dox` | `group__chapter__core__dump.html` | Making The Program Dump Core |
| `chapter_custom_debug_h` | `custom-debug.h.dox` | `group__chapter__custom__debug__h.html` | The Custom debug.h File |
| `chapter_custom_do` | `custom_do.dox` | `group__chapter__custom__do.html` | Custom Debug Objects |
| `chapter_validation` | `deallocation_pointer_validation.dox` | `group__chapter__validation.html` | De-Allocation Pointer Validation |
| `group_debug_channels` | `debug_channels.dox` | `group__group__debug__channels.html` | Controlling The Output Level (Debug Channels) |
| `group_debug_object` | `debug_object.dox` | `group__group__debug__object.html` | The Output Device (Debug Object) |
| `group_destination` | `destination.dox` | `group__group__destination.html` | Setting The Output Destination |
| `downloading` | `downloading.dox` | `downloading.html` | Downloading |
| `chapter_environment` | `environment.dox` | `group__chapter__environment.html` | Environment Variables |
| `group_fatal_output` | `fatal_output.dox` | `group__group__fatal__output.html` | Fatal Debug Output |
| `group_finding` | `finding.dox` | `group__group__finding.html` | Finding (Information About) Memory Allocations |
| `group_formatting` | `formatting.dox` | `group__group__formatting.html` | Format Of The Debug Output |
| `group_function` | `function.dox` | `group__group__function.html` | Function objects |
| `chapter_gdb` | `gdb.dox` | `group__chapter__gdb.html` | Helper functions for within gdb |
| `group_invisible` | `invisible.dox` | `group__group__invisible.html` | Invisible Allocations |
| `chapter_alloc_locations` | `location.dox` | `group__chapter__alloc__locations.html` | Memory Allocation Debugging And Locations |
| `group_locations` | `locations.dox` | `group__group__locations.html` | Source-file:Line-number Information |
| `chapter_magic_numbers` | `magic.dox` | `group__chapter__magic__numbers.html` | Overflow/Underflow Protection (Red Zones) |
| `mainpage` | `mainpage.dox` | `index.html` | Reference Manual: Table Of Contents |
| `group_markers` | `memory_leak_checking.dox` | `group__group__markers.html` | Memory Allocation Markers: Memory Leak Checking |
| `book_writing` | `modules.dox` | `group__book__writing.html` | Writing Debug Output |
| `book_allocations` | `modules.dox` | `group__book__allocations.html` | Memory Allocation Debug Support |
| `book_runtime` | `modules.dox` | `group__book__runtime.html` | Miscellaneous Runtime |
| `book_symbols` | `modules.dox` | `group__book__symbols.html` | Program Symbols Access And Interpretation |
| `book_start` | `modules.dox` | `group__book__start.html` | Configuration, Installation And Getting Started |
| `chapter_nesting` | `nested.dox` | `group__chapter__nesting.html` | Nesting Debug Output |
| `preparation` | `preparation.dox` | `preparation.html` | Preparation |
| `chapter_rcfile` | `rcfile.dox` | `group__chapter__rcfile.html` | Runtime Configuration File (rcfile) |
| `group_special` | `special.dox` | `group__group__special.html` | Special Functions And Utilities |
| `chapter_symbols_intro` | `symbols_intro.dox` | `group__chapter__symbols__intro.html` | Program Symbols Access And Interpretation: Introduction |
| `group_type_info` | `type_info.dox` | `group__group__type__info.html` | Getting Type Information Of Types And Symbols |
| `page_why_macro` | `why_macro.dox` | `page_why_macro.html` | Design Consideration Concerning Macros |
| `book_writing_intro` | `writing_intro.dox` | `group__book__writing__intro.html` | Writing Debug Output: Introduction |

## Unmapped related page observed

`reference-manual/pages.html` also lists `gpl_and_qpl.html` with title `Using GPL-ed code together with QPL-ed code.` No matching input text or Doxygen page command was found under `legacy-doc/`, so it is not linked to a unique legacy-doc input file here.
