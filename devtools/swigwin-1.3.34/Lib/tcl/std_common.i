/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * std_common.i
 *
 * SWIG typemaps for STL - common utilities
 * ----------------------------------------------------------------------------- */

%include <std/std_except.i>

%types(std::size_t);
%apply size_t { std::size_t };
%apply const unsigned long& { const std::size_t& };

%types(std::ptrdiff_t);
%apply long { std::ptrdiff_t };
%apply const long& { const std::ptrdiff_t& };


