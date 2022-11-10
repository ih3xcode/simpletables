# SimpleTables
SimpleTables is a simple, easy to use, and lightweight (6.5kb header only) library for creating ncurses tables in pure C.

## Features
- Easy to use
- Lightweight
- Header only
- No dependencies
- Customizable styles
- Auto-calculates column widths

## Usage
Obtain the header file and include it in your project. Then, create a table and add rows to it. Finally, call `st_print_table` to print the table to the screen.

```c
#include "simpletables.h"

int main() {
    // Initialize ncurses (skipped for brevity)
    /// ...

    st_table *table = st_create_table("My Table", 60, 2);
    st_set_column(table, 0, "Column 1", 0.5);
    st_set_column(table, 1, "Column 2", 0.5);
    st_add_row(table, "Row 1", "Row 1");
    st_add_row(table, "Row 2", "Row 2");
    st_add_row(table, "Row 3", "Row 3");

    st_print_table(table);
    st_free_table(table);

    // End ncurses (skipped for brevity)
    // ...
    return 0;
}
```

For detailed example, see the [example.c](example.c) file.

## Compiling
To compile the example (and any other single-file programs using SimpleTables), simply run the following command:

```bash
gcc example.c -lncurses -o example
```

## Styles
SimpleTables has two predefined styles: `star` and `unicode`. They can be set using `ST_STYLE` macro (1 for star, 2 for unicode, 0 for custom). The default style is `star`.

```c
#define ST_STYLE 2 // use unicode style
#include "simpletables.h"
```

## Custom Styles
There are two ways to create a custom style. The first is to use the `st_style` struct and the `st_create_style` function. The second is to use the `ST_STYLE` macro. The `ST_STYLE` macro is a bit more convenient, but it is limited to the predefined styles.

### Using macros
To use the macros style, first set `ST_STYLE` to 0. Then, define the following macros:

`ST_SULC` - The character to use for the top left corner of the table.

`ST_SURC` - The character to use for the top right corner of the table.

`ST_SLLC` - The character to use for the bottom left corner of the table.

`ST_SLRC` - The character to use for the bottom right corner of the table.

`ST_SHL` - The character to use for the horizontal line.

`ST_SVL` - The character to use for the vertical line.

`ST_SUT` - The character to use for the upper T symbol.

`ST_SLT` - The character to use for the lower T symbol.

`ST_SLL` - The character to use for the left T symbol.

`ST_SRL` - The character to use for the right T symbol.

`ST_SC` - The character to use for the cross symbol.

### Using `st_style` struct
`st_style` is needed to use different styles for different tables. To create a style, use the `st_create_style` function. The function takes the same arguments as required in macros method.

```c
st_style *style = st_create_style(
    ".", ".", ".", ".", ".", ".",
    ".", ".", ".", ".", ".", false); // false is for bold header
```

Then, use *_styled functions to print the table with the style.

```c
st_print_styled(table, style);
```

After you are done with the style, free it using `st_free_style`.

```c
st_free_style(style);
```
