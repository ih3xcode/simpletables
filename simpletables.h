// Copyright 2022 h3xcode
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SIMPLETABLES_H
#define SIMPLETABLES_H

#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef ST_STYLE
#define ST_STYLE 1
#endif

#if ST_STYLE == 1
#define _ST_SULC "*"
#define _ST_SURC "*"
#define _ST_SLLC "*"
#define _ST_SLRC "*"
#define _ST_SHL "*"
#define _ST_SVL "*"
#define _ST_SUT "*"
#define _ST_SLT "*"
#define _ST_SLL "*"
#define _ST_SRL "*"
#define _ST_SC "*"
#elif ST_STYLE == 2
#define _ST_SULC "┌"
#define _ST_SURC "┐"
#define _ST_SLLC "└"
#define _ST_SLRC "┘"
#define _ST_SHL "─"
#define _ST_SVL "│"
#define _ST_SUT "┬"
#define _ST_SLT "┴"
#define _ST_SLL "├"
#define _ST_SRL "┤"
#define _ST_SC "┼"
#elif ST_STYLE == 0
// Custom style
#ifndef ST_SULC
#error "ST_STYLE is set to 0, but ST_SULC is not defined"
//
#endif
#ifndef ST_SURC
#error "ST_STYLE is set to 0, but ST_SURC is not defined"
#endif
#ifndef ST_SLLC
#error "ST_STYLE is set to 0, but ST_SLLC is not defined"
#endif
#ifndef ST_SLRC
#error "ST_STYLE is set to 0, but ST_SLRC is not defined"
#endif
#ifndef ST_SHL
#error "ST_STYLE is set to 0, but ST_SHL is not defined"
#endif
#ifndef ST_SVL
#error "ST_STYLE is set to 0, but ST_SVL is not defined"
#endif
#ifndef ST_SUT
#error "ST_STYLE is set to 0, but ST_SUT is not defined"
#endif
#ifndef ST_SLT
#error "ST_STYLE is set to 0, but ST_SLT is not defined"
#endif
#ifndef ST_SLL
#error "ST_STYLE is set to 0, but ST_SLL is not defined"
#endif
#ifndef ST_SRL
#error "ST_STYLE is set to 0, but ST_SRL is not defined"
#endif
#ifndef ST_SC
#error "ST_STYLE is set to 0, but ST_SC is not defined"
#endif
#define _ST_SULC ST_SULC
#define _ST_SURC ST_SURC
#define _ST_SLLC ST_SLLC
#define _ST_SLRC ST_SLRC
#define _ST_SHL ST_SHL
#define _ST_SVL ST_SVL
#define _ST_SUT ST_SUT
#define _ST_SLT ST_SLT
#define _ST_SLL ST_SLL
#define _ST_SRL ST_SRL
#define _ST_SC ST_SC
#else
#error "Invalid ST_STYLE"
#endif

struct st_row {
  int columns_count;
  char **columns;
};

typedef struct {
  char *sulc;
  char *surc;
  char *sllc;
  char *slrc;
  char *shl;
  char *svl;
  char *sut;
  char *slt;
  char *sll;
  char *srl;
  char *sc;
  bool bold_header;
} st_style;

st_style st_default_style = {_ST_SULC, _ST_SURC, _ST_SLLC, _ST_SLRC,
                             _ST_SHL,  _ST_SVL,  _ST_SUT,  _ST_SLT,
                             _ST_SLL,  _ST_SRL,  _ST_SC,   true};

typedef struct {
  char *title;
  int width;
  int usable_width;
  int columns_count;
  float *columns_width_modifiers;
  char **columns;
  int rows_count;
  struct st_row *rows;
} st_table;

st_table *st_create_table(char *title, int width, int columns_count) {
  st_table *table = malloc(sizeof(st_table));

  table->title = title;
  table->width = width;
  table->usable_width = width - (columns_count);
  table->columns_count = columns_count;
  table->columns_width_modifiers = malloc(sizeof(float) * columns_count);
  table->columns = malloc(sizeof(char *) * columns_count);
  table->rows_count = 0;
  table->rows = malloc(sizeof(struct st_row));
  table->rows[0] = (struct st_row){0, NULL};

  float default_width_modifier = 1.0 / columns_count;
  for (int i = 0; i < columns_count; i++) {
    table->columns_width_modifiers[i] = default_width_modifier;
  }

  return table;
}

void st_set_column(st_table *table, int column, char *name,
                   float width_modifier) {
  table->columns[column] = name;
  table->columns_width_modifiers[column] = width_modifier;
}

void st_add_row(st_table *table, char **row) {
  table->rows_count++;
  table->rows = realloc(table->rows, sizeof(struct st_row) * table->rows_count);
  table->rows[table->rows_count - 1] =
      (struct st_row){table->columns_count, row};
}

char *_st_center_text(char *text, int width) {
  int text_length = strlen(text);
  int right_padding = (width - text_length) / 2;
  int left_padding = width - text_length - right_padding;
  char *result = malloc(sizeof(char) * (width + 1));
  for (int i = 0; i < left_padding; i++) {
    result[i] = ' ';
  }
  for (int i = 0; i < text_length; i++) {
    result[i + left_padding] = text[i];
  }
  for (int i = 0; i < right_padding; i++) {
    result[i + left_padding + text_length] = ' ';
  }
  result[width] = '\0';
  return result;
}

st_style *st_create_style(char *sulc, char *surc, char *sllc, char *slrc,
                          char *shl, char *svl, char *sut, char *slt, char *sll,
                          char *srl, char *sc, bool bold_header) {
  st_style *style = malloc(sizeof(st_style));
  style->sulc = malloc(sizeof(char) * (strlen(sulc) + 1));
  strcpy(style->sulc, sulc);
  style->surc = malloc(sizeof(char) * (strlen(surc) + 1));
  strcpy(style->surc, surc);
  style->sllc = malloc(sizeof(char) * (strlen(sllc) + 1));
  strcpy(style->sllc, sllc);
  style->slrc = malloc(sizeof(char) * (strlen(slrc) + 1));
  strcpy(style->slrc, slrc);
  style->shl = malloc(sizeof(char) * (strlen(shl) + 1));
  strcpy(style->shl, shl);
  style->svl = malloc(sizeof(char) * (strlen(svl) + 1));
  strcpy(style->svl, svl);
  style->sut = malloc(sizeof(char) * (strlen(sut) + 1));
  strcpy(style->sut, sut);
  style->slt = malloc(sizeof(char) * (strlen(slt) + 1));
  strcpy(style->slt, slt);
  style->sll = malloc(sizeof(char) * (strlen(sll) + 1));
  strcpy(style->sll, sll);
  style->srl = malloc(sizeof(char) * (strlen(srl) + 1));
  strcpy(style->srl, srl);
  style->sc = malloc(sizeof(char) * (strlen(sc) + 1));
  strcpy(style->sc, sc);

  style->bold_header = bold_header;
  return style;
}

void st_print_header_styled(st_table *table, st_style *style) {
  char *title = _st_center_text(table->title, table->width);
  if (style->bold_header) attron(A_BOLD);
  printw("%s\n", title);
  if (style->bold_header) attroff(A_BOLD);
  free(title);

  printw("%s", style->sulc);
  for (int i = 0; i < table->columns_count; i++) {
    int column_width = table->columns_width_modifiers[i] * table->usable_width;
    for (int j = 0; j < column_width; j++) {
      printw("%s", style->shl);
    }
    if (i < table->columns_count - 1) {
      printw("%s", style->sut);
    }
  }
  printw("%s\n", style->surc);

  printw("%s", style->svl);
  for (int i = 0; i < table->columns_count; i++) {
    int column_width = table->columns_width_modifiers[i] * table->usable_width;
    char *column = _st_center_text(table->columns[i], column_width);
    if (style->bold_header) attron(A_BOLD);
    printw("%s", column);
    if (style->bold_header) attroff(A_BOLD);
    free(column);
    printw(style->svl);
  }
  printw("\n");

  printw("%s", style->sll);
  for (int i = 0; i < table->columns_count; i++) {
    int column_width = table->columns_width_modifiers[i] * table->usable_width;
    for (int j = 0; j < column_width; j++) {
      printw("%s", style->shl);
    }
    if (i < table->columns_count - 1) {
      printw("%s", style->sc);
    }
  }
  printw("%s\n", style->srl);
}

void st_print_row_styled(st_table *table, int row, st_style *style) {
  printw("%s", style->svl);
  for (int i = 0; i < table->columns_count; i++) {
    int column_width = table->columns_width_modifiers[i] * table->usable_width;
    char *column = _st_center_text(table->rows[row].columns[i], column_width);
    printw("%s", column);
    free(column);
    printw(style->svl);
  }
  printw("\n");
}

void st_print_rows_styled(st_table *table, st_style *style) {
  for (int i = 0; i < table->rows_count; i++) {
    st_print_row_styled(table, i, style);
  }
}

void st_print_styled(st_table *table, st_style *style) {
  st_print_header_styled(table, style);
  st_print_rows_styled(table, style);

  printw("%s", style->sllc);
  for (int i = 0; i < table->columns_count; i++) {
    int column_width = table->columns_width_modifiers[i] * table->usable_width;
    for (int j = 0; j < column_width; j++) {
      printw("%s", style->shl);
    }
    if (i < table->columns_count - 1) {
      printw("%s", style->slt);
    }
  }
  printw("%s\n", style->slrc);
}

void st_free_style(st_style *style) {
  free(style->sulc);
  free(style->surc);
  free(style->sllc);
  free(style->slrc);
  free(style->shl);
  free(style->svl);
  free(style->sut);
  free(style->slt);
  free(style->sll);
  free(style->srl);
  free(style->sc);
  free(style);
}

void st_free(st_table *table) {
  free(table->columns_width_modifiers);
  free(table->columns);
  free(table->rows);
  free(table);
}

#define st_print_header(table) st_print_header_styled(table, &st_default_style)
#define st_print_row(table, row) \
  st_print_row_styled(table, row, &st_default_style)
#define st_print_rows(table) st_print_rows_styled(table, &st_default_style)
#define st_print(table) st_print_styled(table, &st_default_style)

#endif
