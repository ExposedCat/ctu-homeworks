//----------------------------------------------------------------------------------------
/**
 * \file       my_string.h
 * \author     Jiri Ulrich
 * \date       2022/10/25
 * \brief      Simplified string library.
 *
 *  Simplified string library. Only a selected subset of string manipulation
 * functions. They differ from the standard in certain parts like return vals
 * and mutability of args.
 *
 */
//----------------------------------------------------------------------------------------

#ifndef MY_STRING_H
#define MY_STRING_H

/// Length of the maximum initial segment
/**
  Returns the length of the maximum initial segment (span) of the
  null-terminated byte string pointed to by dest, that consists of only the
  characters found in the null-terminated byte string pointed to by src.

  \param[in] dest    pointer to the null-terminated byte string to be analyzed
  \param[in] src     pointer to the null-terminated byte string that contains
  the characters to search for \return The length of the maximum initial segment
  that contains only characters from the null-terminated byte string pointed to
  by src.
*/
unsigned long strspn(const char *dest, const char *src);

/// Finds the next token in a byte string
/**
  Finds the next token in a null-terminated byte string pointed to by str. The
  separator characters are identified by null-terminated byte string pointed to
  by delim. This function is designed to be called multiple times to obtain
  successive tokens from the same string.

    - If str is not a null pointer, the call is treated as the first call to
  strtok for this particular string. The function searches for the first
  character which is not contained in delim.
      - If no such character was found, there are no tokens in str at all, and
  the function returns a null pointer. If such character was found, it is the
  beginning of the token. The function then searches from that point on for the
  first character that is contained in delim.
        - If no such character was found, str has only one token, and future
  calls to strtok will return a null pointer
        - If such character was found, it is replaced by the null character '\0'
  and the pointer to the following character is stored in a static location for
  subsequent invocations.
      - The function then returns the pointer to the beginning of the token

    - If str is a null pointer, the call is treated as a subsequent call to
  strtok: the function continues from where it left in previous invocation. The
  behavior is the same as if the previously stored pointer is passed as str.

  \param[in] str    pointer to the null-terminated byte string to tokenize
  \param[in] delim         pointer to the null-terminated byte string to copy
  from \return Returns pointer to the beginning of the next token or a null
  pointer if there are no more tokens.
*/
char *strtok(char *str, const char *delim);

/// String concatenation
/**
  Appends a copy of the null-terminated byte string pointed to by src to the end
  of the null-terminated byte string pointed to by dest. The character src[0]
  replaces the null terminator at the end of dest. The resulting byte string is
  null-terminated. The dest string will be reallocated if needed to fit the src
  string.

  \param[in,out] dest    pointer to the null-terminated byte string to append to
  \param[in] src         pointer to the null-terminated byte string identifying
  delimiters
*/
void strcat(char **dest, const char *src);

/// Substring matching
/**
  Finds the first occurrence of the null-terminated byte string pointed to by
  substr in the null-terminated byte string pointed to by str. The terminating
  null characters are not compared. The substr may contain the following subset
  of the regex syntax:
    []        - character groups
    [^]       - group prohibition

  \param[in] str         pointer to the null-terminated byte string to examine
  \param[in] substr      pointer to the null-terminated byte string to search
  for \param[out] begin      pointer to the beginning of the found substring or
  null pointer if not found \return The length of the found substring segment.
*/
unsigned long strstr_match(                                  //
    const char *str, const char *substr, const char **begin  //
);

#endif  // MY_STRING_H
