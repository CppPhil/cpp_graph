#ifndef INCG_G_PRETTY_FUNCTION_HPP
#define INCG_G_PRETTY_FUNCTION_HPP

/*!
 * \def G_PRETTY_FUNCTION
 * \brief Expands to the current function's name.
 **/

#ifdef _MSC_VER
#define G_PRETTY_FUNCTION __FUNCSIG__
#else
#define G_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#endif // INCG_G_PRETTY_FUNCTION_HPP
