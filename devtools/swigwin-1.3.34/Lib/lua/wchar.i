/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * wchar.i
 *
 * Typemaps for the wchar_t type
 * These are mapped to a Lua string and are passed around by value.
 *
 * ----------------------------------------------------------------------------- */

// note: only support for pointer right now, not fixed length strings
// TODO: determine how long a const wchar_t* is so we can write wstr2str() 
// & do the output typemap

%{
#include <stdlib.h>
	
wchar_t* str2wstr(const char* str, int len)
{
  wchar_t* p;
  if (str==0 || len<1)  return 0;
  p=(wchar*)malloc((len+1)*sizeof(wchar_t));
  if (p==0)	return 0;
  if (mbstowcs(p, str, len)==-1)
  {
    free(p);
    return 0;
  }
  p[len]=0;
  return p;
}
%}

%typemap( in, checkfn="lua_isstring" ) wchar_t*
%{
$1 = str2wstr(lua_tostring( L, $input ),lua_strlen( L, $input ));
if ($1==0) {lua_pushfstring(L,"Error in converting to wchar (arg %d)",$input);goto fail;}
%}

%typemap( freearg ) wchar_t*
%{
free($1);
%}

%typemap(typecheck) wchar_t * = char *;