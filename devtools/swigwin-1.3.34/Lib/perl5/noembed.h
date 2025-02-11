/* Workaround perl5 global namespace pollution. Note that undefining library
 * functions like fopen will not solve the problem on all platforms as fopen
 * might be a macro on Windows but not necessarily on other operating systems. */
#ifdef do_open
  #undef do_open
#endif
#ifdef do_close
  #undef do_close
#endif
#ifdef scalar
  #undef scalar
#endif
#ifdef list
  #undef list
#endif
#ifdef apply
  #undef apply
#endif
#ifdef convert
  #undef convert
#endif
#ifdef Error
  #undef Error
#endif
#ifdef form
  #undef form
#endif
#ifdef vform
  #undef vform
#endif
#ifdef LABEL
  #undef LABEL
#endif
#ifdef METHOD
  #undef METHOD
#endif
#ifdef Move
  #undef Move
#endif
#ifdef yylex
  #undef yylex
#endif
#ifdef yyparse
  #undef yyparse
#endif
#ifdef yyerror
  #undef yyerror
#endif
#ifdef invert
  #undef invert
#endif
#ifdef ref
  #undef ref
#endif
#ifdef read
  #undef read
#endif
#ifdef write
  #undef write
#endif
#ifdef eof
  #undef eof
#endif
#ifdef bool
  #undef bool
#endif
#ifdef close
  #undef close
#endif
#ifdef rewind
  #undef rewind
#endif
#ifdef free
  #undef free
#endif
#ifdef malloc
  #undef malloc
#endif
#ifdef calloc
  #undef calloc
#endif
#ifdef Stat
  #undef Stat
#endif
#ifdef check
  #undef check
#endif
#ifdef seekdir
  #undef seekdir
#endif
#ifdef open
  #undef open
#endif
