#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "parse.h"

/*          Get tokens from a line of characters */
/* Return:  new array of pointers to tokens */
/* Effects: token separators in line are replaced with NULL */
/* Storage: Resulting token array points into original line */

#define TOKseparator " \n:"

tok_t *getToks(char *line) {
  int i;
  char *c;

  tok_t *toks = malloc(MAXTOKS*sizeof(tok_t));
  for (i=0; i<MAXTOKS; i++) toks[i] = NULL;     /* empty token array */
  

  c = strtok(line,TOKseparator);	 /* Start tokenizer on line */
  for (i=0; c && (i < MAXTOKS); i++) {
    toks[i] = c;
    c = strtok(NULL,TOKseparator);	/* scan for next token */
  }
  return toks;
}

void freeToks(tok_t *toks) {
  free(toks);
}

void fprintTok(FILE *ofile, tok_t *t) {
  int i;
  for (i=0; i<MAXTOKS && t[i]; i++) {
    fprintf(ofile,"%s ", t[i]);
  }
  fprintf(ofile,"\n");
}

/* Locate special processing character */
int isDirectTok(tok_t *t, char *R) {
  int i;
  for (i=0; i<MAXTOKS-1 && t[i]; i++) {
    if (strncmp(t[i],R,1) == 0) return i;
  }
  return 0;
}
