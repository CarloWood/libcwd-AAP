# Existing dlclose handling

The old implementation does handle `dlclose`, but only partially. It interposes `dlclose`, tracks handles and reference counts, and on final close removes the object from the active DWARF object-file list and closes DWARF resources. It intentionally does not delete the object record, because existing `location_ct` objects may still point at its public object-file data.

## Repository evidence

- `dwarf.cc` defines the exported `dlopen` wrapper. After a successful load it obtains the `link_map`, loads the object through the existing object-file path, sorts object files, and records handle state in `dlopen_map`.
- `dwarf.cc` defines the exported `dlclose` wrapper. After calling the real `dlclose`, it decrements the tracked refcount; at zero it deinitializes the associated object unless the object was opened with `RTLD_NODELETE`, then erases the map entry.
- `objfile_ct::deinitialize()` removes the object from the active object-file list and closes DWARF state.
- `close_dwarf()` releases the `Dwarf*` handle and DWARF file descriptor.
- A comment in the `dlclose` path explicitly says the object is not deleted because `location_ct` instances may still point to it.
- `location_ct` stores an `object_file_ct const*`, which explains the retained lifetime requirement.
- `ObjectFileBase::s_object_files_` is the analogous new dwarf2 list that will need equivalent unload semantics.

## State updated vs. left stale

Updated on final close:

- `dlopen_map` refcount is decremented and eventually erased.
- The unloaded object is removed from active address lookup.
- DWARF resources and file descriptors are closed.

Intentionally retained:

- The allocated old object-file record remains alive so existing `location_ct` pointers remain valid.
- File/path identity remains available for already-created location objects.

Potentially stale:

- Previously cached `location_ct` values can still report the old object-file identity after unload.
- Symbol data owned by the retained object remains allocated but no longer participates in active lookup.
- Future address lookups must not match unloaded objects merely because their old address range is still known.

## Plan implications for `ObjectFileBase::s_object_files_`

`ObjectFileBase::s_object_files_` should represent currently active objects for address lookup. Non-`RTLD_NODELETE` objects should be removed from that list after the loader's final close, while metadata referenced by public/cached location objects must remain stable through a retained record or tombstone.

The dwarf2 design needs handle/refcount tracking comparable to the old `dlopen_map`, including `RTLD_NODELETE` behavior, and must prevent stale unloaded address ranges from resolving after a different library is later mapped at the same addresses.
