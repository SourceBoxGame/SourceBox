/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * std_string.i
 *
 * SWIG typemaps for std::string types
 * ----------------------------------------------------------------------------- */

// ------------------------------------------------------------------------
// std::string is typemapped by value
// This can prevent exporting methods which return a string
// in order for the user to modify it.
// However, I think I'll wait until someone asks for it...
// ------------------------------------------------------------------------

%include <exception.i>

%{
#include <string>
%}

namespace std {

    %naturalvar string;

    class string;

    %typemap(typecheck,precedence=SWIG_TYPECHECK_STRING) string %{
      $1 = ( Z_TYPE_PP($input) == IS_STRING ) ? 1 : 0;
    %}

    %typemap(in) string %{
        convert_to_string_ex($input);
        $1.assign(Z_STRVAL_PP($input), Z_STRLEN_PP($input));
    %}

    %typemap(typecheck,precedence=SWIG_TYPECHECK_STRING) const string& %{
      $1 = ( Z_TYPE_PP($input) == IS_STRING ) ? 1 : 0;
    %}

    %typemap(out) string %{
        ZVAL_STRINGL($result, const_cast<char*>($1.data()), $1.size(), 1);
    %}

    %typemap(out) const string & %{
        ZVAL_STRINGL($result, const_cast<char*>($1->data()), $1->size(), 1);
    %}

    %typemap(throws) string %{
      SWIG_PHP_Error(E_ERROR, (char *)$1.c_str());
    %}

    %typemap(throws) const string& %{
      SWIG_PHP_Error(E_ERROR, (char *)$1.c_str());
    %}

    /* These next two handle a function which takes a non-const reference to
     * a std::string and modifies the string. */
    %typemap(in) string & (std::string temp) %{
        convert_to_string_ex($input);
        temp.assign(Z_STRVAL_PP($input), Z_STRLEN_PP($input));
        $1 = &temp;
    %}

    %typemap(argout) string & %{
	ZVAL_STRINGL(*($input), const_cast<char*>($1->data()), $1->size(), 1);
    %}

    /* SWIG will apply the non-const typemap above to const string& without
     * this more specific typemap. */
    %typemap(argout) const string & "";
}
