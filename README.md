# libtree

A tool that:
- :deciduous_tree: turns `ldd` into a tree
- :point_up: explains how shared libraries are found or why they cannot be located

![Screenshot of libtree](doc/screenshot.png)

## Output

By default, certain standard dependencies are not shown. For more verbose output use

-  `libtree -v`             Show libraries skipped by default
-  `libtree -vv`            Show dependencies of libraries skipped by default
-  `libtree -vvv`           Show dependencies of already encountered libraries

Use the `--path` or `-p` flags to show paths rather than sonames:

- `libtree -p $(which tar)`

## Building from sources

`libtree` requires a C compiler that understands c99

```
git clone https://github.com/haampie/libtree.git
cd libtree
make # recommended: LDFLAGS=-static
```

<details>
<summary>Or use the following unsafe quick install instructions</summary>

```
curl -Lfs https://raw.githubusercontent.com/haampie/libtree/master/libtree.c | ${CC:-cc} -o libtree -x c - -std=c99 -D_FILE_OFFSET_BITS=64
```
</details>