# Member Name Modernization Overview

This file is generated for objective `08/01` and is intentionally restricted to the approved class/struct list from the preceding type-name refactor. It is meant to be easy for humans to edit and easy for agents to consume.

Naming policy used for suggestions:
- private/protected data members: `snake_case_` with a trailing underscore; legacy prefixes such as `M_`, `m_`, `WNS_`, `NS_`, and `S_` are removed.
- public struct members: `snake_case` without a trailing underscore; legacy member prefixes are removed.
- public data members in a `class` are listed as public data and suggested without a trailing underscore; consider whether those types should be structs before applying changes.
- `same` means the current member already follows the policy inferred for that access/type.

## Summary

| Type | Kind | Definition | Member entries |
|---|---|---|---:|
| `AlwaysChannel` | class | `include/libcwd/class_always_channel.h:25` | 1 |
| `Argv` | class | `utils/exec_prog.cc:26` | 1 |
| `Buffer` | class | `src/debug.cc:38` | 3 |
| `BufferStream` | class | `include/libcwd/private_bufferstream.h:19` | 1 |
| `CaptureState` | struct | `tests/support/location_loading_support.h:17` | 6 |
| `Channel` | class | `include/libcwd/class_channel.h:72` | 4 |
| `FatalChannelSetBootstrap` | struct | `include/libcwd/class_channel_set.h:86` | 0 |
| `ChannelSetBootstrap` | struct | `include/libcwd/class_channel_set.h:72` | 0 |
| `ChannelSetData` | struct | `include/libcwd/class_channel_set.h:30` | 4 |
| `ChannelSet` | struct | `include/libcwd/class_channel_set.h:105` | 0 |
| `ContinuedChannel` | class | `include/libcwd/class_continued_channel.h:26` | 1 |
| `ContinuedChannelSet` | struct | `include/libcwd/class_channel_set.h:120` | 0 |
| `DebugChannels` | struct | `include/libcwd/macro_ForAllDebugChannels.h:29` | 1 |
| `DebugChannels::Impl` | struct | `src/debug.cc:436` | 1 |
| `DebugObject` | class | `include/libcwd/class_debug.h:104` | 10 |
| `DebugObjects` | struct | `include/libcwd/macro_ForAllDebugObjects.h:27` | 1 |
| `DebugObjects::Impl` | struct | `src/debug.cc:590` | 1 |
| `DebugString` | class | `include/libcwd/class_debug_string.h:32` | 5 |
| `DebugStringStackElement` | struct | `include/libcwd/class_debug_string.h:72` | 2 |
| `DebugObject_ThreadSpecificData` | struct | `include/libcwd/struct_debug_tsd.h:35` | 16 |
| `Entry` | struct | `include/cwd_demangle.h:1073` | 3 |
| `Environment` | class | `include/environment.h:15` | 1 |
| `FatalChannel` | class | `include/libcwd/class_fatal_channel.h:29` | 2 |
| `OutputState` | class | `src/debug.cc:642` | 5 |
| `Location` | class | `include/libcwd/class_location.h:56` | 12 |
| `location_libtest1_LifeCycle` | class | `tests/support/location_libtest1.cc:35` | 0 |
| `location_libtest2_LifeCycle` | class | `tests/support/location_libtest2.cc:10` | 0 |
| `location_plugin1_LifeCycle` | class | `tests/support/location_plugin1.cc:11` | 0 |
| `location_plugin2_LifeCycle` | class | `tests/support/location_plugin2.cc:9` | 0 |
| `LockInterfaceBase` | class | `include/libcwd/private_lock_interface.h:15` | 0 |
| `OstreamState` | struct | `include/libcwd/class_debug.h:31` | 3 |
| `ParsedLine (tests/colors_debug_output.cc)` | struct | `tests/colors_debug_output.cc:27` | 2 |
| `ParsedLine (tests/threaded_debug_output.cc)` | struct | `tests/threaded_debug_output.cc:119` | 8 |
| `PrintablePollDummy` | class | `utils/exec_prog.cc:36` | 2 |
| `RcFile` | class | `include/libcwd/class_rcfile.h:31` | 6 |
| `RedirectCerr` | class | `tests/support/test_support.h:54` | 1 |
| `RedirectCerrToStringStream` | class | `tests/continued.cc:41` | 1 |
| `RedirectCout` | class | `tests/location_from_global_object.cc:21` | 1 |
| `RedirectStream` | class | `tests/do.cc:34` | 2 |
| `RefCountedCharPtr` | class | `include/libcwd/smart_ptr.h:17` | 2 |
| `Substitution` | struct | `include/cwd_demangle.h:46` | 3 |
| `ThreadSpecificData` | struct | `include/libcwd/private_struct_TSD.h:76` | 9 |
| `TypeInfo` | class | `include/libcwd/type_info.h:51` | 4 |

## Detailed inventory

### `AlwaysChannel`

- kind: `class`
- definition: `include/libcwd/class_always_channel.h:25`
- ctags scope: `libcwd::AlwaysChannel`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `label` | `label` | 27 | same; static; public class data |

### `Argv`

- kind: `class`
- definition: `utils/exec_prog.cc:26`
- ctags scope: `libcwd::__anond056c8950111::Argv`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `__argv` | `argv_` | 29 | rename |

### `Buffer`

- kind: `class`
- definition: `src/debug.cc:38`
- ctags scope: `libcwd::Buffer`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `position` | `position_` | 42 | rename |
| `private` | `unfinished_already_printed` | `unfinished_already_printed_` | 44 | rename |
| `private` | `continued_needed` | `continued_needed_` | 45 | rename |

### `BufferStream`

- kind: `class`
- definition: `include/libcwd/private_bufferstream.h:19`
- ctags scope: `libcwd::_private_::BufferStream`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `M_stringbuf` | `stringbuf` | 32 | rename; public class data |

### `CaptureState`

- kind: `struct`
- definition: `tests/support/location_loading_support.h:17`
- ctags scope: `libcwd_ctest::location_loading::CaptureState`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `captured` | `captured` | 19 | same |
| `public` | `active_libraries` | `active_libraries` | 20 | same |
| `public` | `final_output_check` | `final_output_check` | 21 | same |
| `public` | `finalized` | `finalized` | 22 | same |
| `public` | `logging_initialized` | `logging_initialized` | 23 | same |
| `public` | `pending_messages` | `pending_messages` | 24 | same |

### `Channel`

- kind: `class`
- definition: `include/libcwd/class_channel.h:72`
- ctags scope: `libcwd::Channel`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `WNS_index` | `index_` | 75 | rename |
| `private` | `WNS_label` | `label_` | 78 | rename |
| `private` | `WNS_initialized` | `initialized_` | 83 | rename |
| `private` | `off_channel` | `off_channel_` | 86 | rename; static |

### `FatalChannelSetBootstrap`

- kind: `struct`
- definition: `include/libcwd/class_channel_set.h:86`
- ctags scope: `libcwd::FatalChannelSetBootstrap`

No member variables found in the definition file.

### `ChannelSetBootstrap`

- kind: `struct`
- definition: `include/libcwd/class_channel_set.h:72`
- ctags scope: `libcwd::ChannelSetBootstrap`

No member variables found in the definition file.

### `ChannelSetData`

- kind: `struct`
- definition: `include/libcwd/class_channel_set.h:30`
- ctags scope: `libcwd::ChannelSetData`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `label` | `label` | 32 | same |
| `public` | `mask` | `mask` | 35 | same |
| `public` | `on` | `on` | 38 | same |
| `public` | `do_tsd_ptr` | `debug_object_tsd_ptr` | 41 | same |

### `ChannelSet`

- kind: `struct`
- definition: `include/libcwd/class_channel_set.h:105`
- ctags scope: `libcwd::ChannelSet`

No member variables found in the definition file.

### `ContinuedChannel`

- kind: `class`
- definition: `include/libcwd/class_continued_channel.h:26`
- ctags scope: `libcwd::ContinuedChannel`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `WNS_maskbit` | `maskbit_` | 28 | rename |

### `ContinuedChannelSet`

- kind: `struct`
- definition: `include/libcwd/class_channel_set.h:120`
- ctags scope: `libcwd::ContinuedChannelSet`

No member variables found in the definition file.

### `DebugChannels`

- kind: `struct`
- definition: `include/libcwd/macro_ForAllDebugChannels.h:29`
- ctags scope: `libcwd::_private_::DebugChannels`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `impl_` | `impl` | 34 | rename |

### `DebugChannels::Impl`

- kind: `struct`
- definition: `src/debug.cc:436`
- ctags scope: `libcwd::_private_::DebugChannels::Impl`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `channel_sets_` | `channel_sets` | 439 | rename |

### `DebugObject`

- kind: `class`
- definition: `include/libcwd/class_debug.h:104`
- ctags scope: `libcwd::DebugObject`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `WNS_index` | `index` | 112 | rename; public class data |
| `public` | `S_index_count` | `index_count` | 113 | rename; static; public class data |
| `protected` | `ostream_state_` | `ostream_state_` | 121 | same |
| `protected` | `unfinished_oss` | `unfinished_oss_` | 124 | rename |
| `protected` | `newlineless_tsd` | `newlineless_tsd_` | 125 | rename |
| `private` | `WNS_initialized` | `initialized_` | 132 | rename |
| `private` | `NS_being_initialized` | `being_initialized_` | 135 | rename |
| `private` | `init_magic` | `init_magic_` | 139 | rename |
| `private` | `interactive` | `interactive_` | 143 | rename |
| `private` | `m_always_flush` | `always_flush_` | 146 | rename |

### `DebugObjects`

- kind: `struct`
- definition: `include/libcwd/macro_ForAllDebugObjects.h:27`
- ctags scope: `libcwd::_private_::DebugObjects`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `impl_` | `impl` | 32 | rename |

### `DebugObjects::Impl`

- kind: `struct`
- definition: `src/debug.cc:590`
- ctags scope: `libcwd::_private_::DebugObjects::Impl`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `debug_objects_` | `debug_objects` | 593 | rename |

### `DebugString`

- kind: `class`
- definition: `include/libcwd/class_debug_string.h:32`
- ctags scope: `libcwd::DebugString`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_str` | `str_` | 36 | rename |
| `private` | `M_size` | `size_` | 37 | rename |
| `private` | `M_capacity` | `capacity_` | 38 | rename |
| `private` | `M_default_capacity` | `default_capacity_` | 39 | rename |
| `private` | `min_capacity_c` | `min_capacity_c_` | 40 | rename; static |

### `DebugStringStackElement`

- kind: `struct`
- definition: `include/libcwd/class_debug_string.h:72`
- ctags scope: `libcwd::DebugStringStackElement`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `next` | `next` | 74 | same |
| `public` | `debug_string` | `debug_string` | 75 | same |

### `DebugObject_ThreadSpecificData`

- kind: `struct`
- definition: `include/libcwd/struct_debug_tsd.h:35`
- ctags scope: `libcwd::DebugObject_ThreadSpecificData`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `tsd_initialized` | `tsd_initialized` | 39 | same |
| `public` | `first_time` | `first_time` | 45 | same |
| `public` | `current_` | `current` | 48 | rename |
| `public` | `current_bufferstream` | `current_bufferstream` | 51 | same |
| `public` | `output_state_stack` | `output_state_stack` | 55 | same |
| `public` | `start_expected` | `start_expected` | 58 | same |
| `public` | `unfinished_expected` | `unfinished_expected` | 61 | same |
| `public` | `off_count` | `off_count` | 64 | same |
| `public` | `continued_stack` | `continued_stack` | 67 | same |
| `public` | `color_on` | `color_on` | 70 | same |
| `public` | `color_off` | `color_off` | 73 | same |
| `public` | `margin` | `margin` | 76 | same |
| `public` | `marker` | `marker` | 79 | same |
| `public` | `M_margin_stack` | `margin_stack` | 82 | rename |
| `public` | `M_marker_stack` | `marker_stack` | 85 | rename |
| `public` | `indent` | `indent` | 88 | same |

### `Entry`

- kind: `struct`
- definition: `include/cwd_demangle.h:1073`
- ctags scope: `__gnu_cxx::demangler::Entry`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `opcode` | `opcode` | 1075 | same |
| `public` | `symbol_name` | `symbol_name` | 1076 | same |
| `public` | `type` | `type` | 1077 | same |

### `Environment`

- kind: `class`
- definition: `include/environment.h:15`
- ctags scope: `libcwd::Environment`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `__envp` | `envp_` | 17 | rename |

### `FatalChannel`

- kind: `class`
- definition: `include/libcwd/class_fatal_channel.h:29`
- ctags scope: `libcwd::FatalChannel`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `WNS_label` | `label_` | 33 | rename |
| `private` | `WNS_maskbit` | `maskbit_` | 38 | rename |

### `OutputState`

- kind: `class`
- definition: `src/debug.cc:642`
- ctags scope: `libcwd::OutputState`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `buffer` | `buffer` | 645 | same; public class data |
| `public` | `bufferstream` | `bufferstream` | 648 | same; public class data |
| `public` | `mask` | `mask` | 651 | same; public class data |
| `public` | `label` | `label` | 654 | same; public class data |
| `public` | `err` | `err` | 657 | same; public class data |

### `Location`

- kind: `class`
- definition: `include/libcwd/class_location.h:56`
- ctags scope: `libcwd::Location`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `protected` | `M_object_file` | `object_file_` | 58 | rename |
| `protected` | `M_func` | `func_` | 59 | rename |
| `protected` | `M_filepath` | `filepath_` | 60 | rename |
| `protected` | `M_filename` | `filename_` | 62 | rename; anonymous union member not emitted by ctags |
| `protected` | `M_initialization_delayed` | `initialization_delayed_` | 63 | rename; anonymous union member not emitted by ctags |
| `protected` | `M_unknown_pc` | `unknown_pc_` | 64 | rename; anonymous union member not emitted by ctags |
| `protected` | `M_line` | `line_` | 66 | rename |
| `protected` | `M_known` | `known_` | 67 | rename |
| `protected` | `S_uninitialized_location_ct_c` | `uninitialized_location_ct_c_` | 73 | rename; static |
| `protected` | `S_pre_ios_initialization_c` | `pre_ios_initialization_c_` | 74 | rename; static |
| `protected` | `S_pre_libcwd_initialization_c` | `pre_libcwd_initialization_c_` | 75 | rename; static |
| `protected` | `S_cleared_location_ct_c` | `cleared_location_ct_c_` | 76 | rename; static |

### `location_libtest1_LifeCycle`

- kind: `class`
- definition: `tests/support/location_libtest1.cc:35`
- ctags scope: `__anon9727d5270111::location_libtest1_LifeCycle`

No member variables found in the definition file.

### `location_libtest2_LifeCycle`

- kind: `class`
- definition: `tests/support/location_libtest2.cc:10`
- ctags scope: `__anon972861880111::location_libtest2_LifeCycle`

No member variables found in the definition file.

### `location_plugin1_LifeCycle`

- kind: `class`
- definition: `tests/support/location_plugin1.cc:11`
- ctags scope: `__anon4200ff5f0111::location_plugin1_LifeCycle`

No member variables found in the definition file.

### `location_plugin2_LifeCycle`

- kind: `class`
- definition: `tests/support/location_plugin2.cc:9`
- ctags scope: `__anon42018bc00111::location_plugin2_LifeCycle`

No member variables found in the definition file.

### `LockInterfaceBase`

- kind: `class`
- definition: `include/libcwd/private_lock_interface.h:15`
- ctags scope: `libcwd::_private_::LockInterfaceBase`

No member variables found in the definition file.

### `OstreamState`

- kind: `struct`
- definition: `include/libcwd/class_debug.h:31`
- ctags scope: `libcwd::_private_::OstreamState`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `state_mutex_` | `state_mutex_` | 34 | same |
| `private` | `real_os_` | `real_os_` | 35 | same |
| `private` | `mutex_` | `mutex_` | 36 | same |

### `ParsedLine (tests/colors_debug_output.cc)`

- kind: `struct`
- definition: `tests/colors_debug_output.cc:27`
- ctags scope: `__anon04fec9430111::ParsedLine`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `color` | `color` | 29 | same |
| `public` | `text` | `text` | 30 | same |

### `ParsedLine (tests/threaded_debug_output.cc)`

- kind: `struct`
- definition: `tests/threaded_debug_output.cc:119`
- ctags scope: `ParsedLine`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `margin` | `margin` | 121 | same |
| `public` | `label` | `label` | 122 | same |
| `public` | `stars` | `stars` | 123 | same |
| `public` | `marker` | `marker` | 124 | same |
| `public` | `indent` | `indent` | 125 | same |
| `public` | `text` | `text` | 126 | same |
| `public` | `end_on_unfinished` | `end_on_unfinished` | 127 | same |
| `public` | `starts_with_continued` | `starts_with_continued` | 128 | same |

### `PrintablePollDummy`

- kind: `class`
- definition: `utils/exec_prog.cc:36`
- ctags scope: `libcwd::__anond056c8950111::PrintablePollDummy`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_pollfds` | `pollfds_` | 39 | rename |
| `private` | `M_number_of_fds` | `number_of_fds_` | 40 | rename |

### `RcFile`

- kind: `class`
- definition: `include/libcwd/class_rcfile.h:31`
- ctags scope: `libcwd::RcFile`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_konsole_command` | `konsole_command_` | 33 | rename |
| `private` | `M_gdb_bin` | `gdb_bin_` | 34 | rename |
| `private` | `M_rcname` | `rcname_` | 36 | rename |
| `private` | `M_env_set` | `env_set_` | 37 | rename |
| `private` | `M_read_called` | `read_called_` | 38 | rename |
| `private` | `M_elfutils_on` | `elfutils_on_` | 41 | rename |

### `RedirectCerr`

- kind: `class`
- definition: `tests/support/test_support.h:54`
- ctags scope: `libcwd_ctest::RedirectCerr`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_original` | `original_` | 56 | rename |

### `RedirectCerrToStringStream`

- kind: `class`
- definition: `tests/continued.cc:41`
- ctags scope: `__anon512b89640111::RedirectCerrToStringStream`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_original` | `original_` | 44 | rename |

### `RedirectCout`

- kind: `class`
- definition: `tests/location_from_global_object.cc:21`
- ctags scope: `__anon75fad26d0111::RedirectCout`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `original_` | `original_` | 24 | same |

### `RedirectStream`

- kind: `class`
- definition: `tests/do.cc:34`
- ctags scope: `__anon62fb75ce0111::RedirectStream`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_stream` | `stream_` | 37 | rename |
| `private` | `M_original` | `original_` | 38 | rename |

### `RefCountedCharPtr`

- kind: `class`
- definition: `include/libcwd/smart_ptr.h:17`
- ctags scope: `libcwd::_private_::RefCountedCharPtr`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `private` | `M_reference_count` | `reference_count_` | 19 | rename |
| `private` | `M_ptr` | `ptr_` | 20 | rename |

### `Substitution`

- kind: `struct`
- definition: `include/cwd_demangle.h:46`
- ctags scope: `__gnu_cxx::demangler::Substitution`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `M_start_pos` | `start_pos` | 48 | rename |
| `public` | `M_type` | `type` | 49 | rename |
| `public` | `M_number_of_prefixes` | `number_of_prefixes` | 50 | rename |

### `ThreadSpecificData`

- kind: `struct`
- definition: `include/libcwd/private_struct_TSD.h:76`
- ctags scope: `libcwd::_private_::ThreadSpecificData`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `public` | `format` | `format` | 79 | same |
| `public` | `lock_interface_is_locked` | `lock_interface_is_locked` | 81 | same |
| `public` | `recursive_fatal` | `recursive_fatal` | 82 | same |
| `public` | `recursive_assert` | `recursive_assert` | 84 | same |
| `public` | `do_off_array` | `debug_object_off_array` | 86 | same |
| `public` | `do_array` | `debug_object_array` | 87 | same |
| `public` | `off_cnt_array` | `off_cnt_array` | 94 | same |
| `private` | `initialized` | `initialized_` | 102 | rename |
| `private` | `cleaning_up` | `cleaning_up_` | 103 | rename |

### `TypeInfo`

- kind: `class`
- definition: `include/libcwd/type_info.h:51`
- ctags scope: `libcwd::TypeInfo`

| Access | Current | Suggested | Line | Notes |
|---|---|---|---:|---|
| `protected` | `M_type_size` | `type_size_` | 53 | rename |
| `protected` | `M_type_ref_size` | `type_ref_size_` | 54 | rename |
| `protected` | `M_name` | `name_` | 55 | rename |
| `protected` | `M_dem_name` | `dem_name_` | 56 | rename |

