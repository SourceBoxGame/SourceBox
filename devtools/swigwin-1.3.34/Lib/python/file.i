/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * file.i
 *
 * Typemaps for FILE*
 * From the ideas of Luigi Ballabio
 * ----------------------------------------------------------------------------- */

%types(FILE *);

/* defining basic methods */
%fragment("SWIG_AsValFilePtr","header") {
SWIGINTERN int
SWIG_AsValFilePtr(PyObject *obj, FILE **val) {
  static swig_type_info* desc = 0;
  void *vptr = 0;
  if (!desc) desc = SWIG_TypeQuery("FILE *");
  if ((SWIG_ConvertPtr(obj, &vptr, desc, 0)) == SWIG_OK) {
    if (val) *val = (FILE *)vptr;
    return SWIG_OK;
  } 
  if (PyFile_Check(obj)) {
    if (val) *val =  PyFile_AsFile(obj);
    return SWIG_OK;
  }
  return SWIG_TypeError;
}
}


%fragment("SWIG_AsFilePtr","header",fragment="SWIG_AsValFilePtr") {
SWIGINTERNINLINE FILE*
SWIG_AsFilePtr(PyObject *obj) {
  FILE *val = 0;
  SWIG_AsValFilePtr(obj, &val);
  return val;
}
}

/* defining the typemaps */
%typemaps_asval(%checkcode(POINTER), SWIG_AsValFilePtr, "SWIG_AsValFilePtr", FILE*);
