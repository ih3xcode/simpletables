#include <locale.h>   // for setlocale
#include <ncurses.h>  // for ncurses

#define ST_STYLE 2         // 0 for custom, 1 for simple, 2 for unicode
#include "simpletables.h"  // for simpletables

int main(void) {
  // Initialize ncurses
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  wbkgd(stdscr, COLOR_PAIR(1));
  refresh();

  // Create a table
  // "Example" is the title of the table
  // 80 is the width of the table
  // 3 is the number of columns
  st_table *table = st_create_table("Example", 80, 5);

  // Check if table is ok
  if (!table) {
    endwin();
    perror("Failed to create table");
    return 1;
  }

  // Set the column names
  // table is the table
  // 0 is the column number
  // "C1" is the name of the column
  // 0.2 is the width modifier of the column
  st_set_column(table, 0, "C1", 0.2);
  st_set_column(table, 1, "C2", 0.2);
  st_set_column(table, 2, "C3", 0.2);
  st_set_column(table, 3, "C4", 0.2);
  st_set_column(table, 4, "C5", 0.2);

  // Add a rows
  // table is the table
  // {"1", "2", "3", ...} is the row
  st_add_row(table, (char *[]){"1", "2", "3", "4", "5"});
  st_add_row(table, (char *[]){"6", "7", "8", "9", "10"});

  // Create style
  st_style *style = st_create_style(".", ".", ".", ".", ".", ".", ".", ".", ".",
                                    ".", ".", false);
  // Print the table with default style
  st_print(table);

  // Print the table with custom style
  st_print_styled(table, style);

  // Free the table
  st_free(table);

  // Free the style
  st_free_style(style);

  // End ncurses
  printw("Press any key to exit...");
  getch();
  endwin();
  return 0;
}
