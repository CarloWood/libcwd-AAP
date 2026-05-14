# APIs for enumerating loaded ELF object files

`dl_iterate_phdr(3)` is sufficient to enumerate loaded ELF objects and obtain the information needed for `ObjectFileBase::s_object_files_`:

- object path: `dl_phdr_info::dlpi_name`;
- load base: `dl_phdr_info::dlpi_addr`;
- loaded address range: computed from `dlpi_phdr[0..dlpi_phnum)` entries whose `p_type == PT_LOAD`.

`libdw` is not needed for enumeration, file paths, load bases, or loaded address bounds. `libdw` is needed later for DWARF/debug-info parsing. `libelf` is also not needed for runtime enumeration or address bounds, though it can still be useful later for reading ELF file contents such as notes/build IDs.

## Documentation evidence

- Linux `dl_iterate_phdr(3)` documents that it walks the loaded shared-object list and provides `dlpi_addr`, `dlpi_name`, `dlpi_phdr`, and `dlpi_phnum`: <https://man7.org/linux/man-pages/man3/dl_iterate_phdr.3.html>
- The same man page states that runtime segment addresses are computed as `dlpi_addr + p_vaddr`.
- The same man page notes that the first object is the main program and that the main program's `dlpi_name` is an empty string.
- Linux `dlinfo(3)` documents `RTLD_DI_LINKMAP`, which exposes `struct link_map` with `l_addr`, `l_name`, and loaded-object links, but this is GNU-specific and handle-oriented rather than the primary enumeration API: <https://man7.org/linux/man-pages/man3/dlinfo.3.html>
- elfutils/libelf `elf_begin(3)` is for opening and reading ELF files. That is file inspection, not runtime loaded-object enumeration.

## Repository evidence

- `include/cwd_dwarf2.h` defines `ObjectFileBase::s_object_files_` as a read-write locked `std::list<ObjectFile*>`.
- `ObjectFileBase` stores an `ObjectFileName`, which is the public object-file identity/path plus debug-info status.
- `dwarf2.cc` defines `ObjectFile` with `lbase_`, `start_addr_`, and `end_addr_` fields.
- `dwarf2.cc` logging already treats those fields as the loaded-object base/range.

## Recommended data model

Create one `ObjectFile` per relevant `dl_iterate_phdr` callback result:

- `path`: copy `dlpi_name` into stable owned storage; handle the main executable's empty name separately, for example with `/proc/self/exe` in a later goal.
- `load_base`: `uintptr_t(info->dlpi_addr)`.
- `start_addr`: minimum `uintptr_t(info->dlpi_addr + phdr.p_vaddr)` over all `PT_LOAD` program headers.
- `end_addr`: maximum `uintptr_t(info->dlpi_addr + phdr.p_vaddr + phdr.p_memsz)` over all `PT_LOAD` program headers.
- Treat ranges as half-open: `[start_addr, end_addr)`.

Use `p_memsz`, not `p_filesz`, because runtime memory includes zero-filled/BSS parts of loadable segments.

## Follow-up uncertainties

- Decide how to resolve the main executable path when `dlpi_name` is empty.
- Decide whether `end_addr` should be the exact maximum loaded segment end or page-rounded mapping end.
- Decide how to handle pseudo objects such as `linux-vdso.so.1`.
- Decide whether `dlclose`/reload updates should be incremental or implemented by refreshing the object list.
