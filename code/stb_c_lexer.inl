/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki.toolkit.git
@file    /client.h
@author  Sean Barrett <https://nothings.org> and
         Cale McCollough <https://cale-mccollough.github.io>
@license Copyright (C) 2014-9 Sean Barrett <nothings.org. and Kabuki Starship
<kabukistarship.com; all right reserved (R). This Source Code Form is subject
to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was
not distributed with this file, You can obtain one at
https://mozilla.org/MPL/2.0/. */

#include "stb_c_lexer.h"

void stb_c_lexer_init(stb_lexer *lexer, const char *input_stream,
                      const char *input_stream_end, char *string_store,
                      SIN store_length) {
  lexer->input_stream = (char *)input_stream;
  lexer->eof = (char *)input_stream_end;
  lexer->parse_point = (char *)input_stream;
  lexer->string_storage = string_store;
  lexer->string_storage_len = store_length;
}

void stb_c_lexer_get_location(const stb_lexer *lexer, const char *where,
                              stb_lex_location *loc) {
  char *p = lexer->input_stream;
  SIN line_number = 1;
  SIN char_offset = 0;
  while (*p && p < where) {
    if (*p == '\n' || *p == '\r') {
      p += (p[0] + p[1] == '\r' + '\n' ? 2 : 1);  // skip newline
      line_number += 1;
      char_offset = 0;
    } else {
      ++p;
      ++char_offset;
    }
  }
  loc->line_number = line_number;
  loc->line_offset = char_offset;
}

SIN stb__clex_token(stb_lexer *lexer, SIN token, char *start, char *end) {
  lexer->token = token;
  lexer->where_firstchar = start;
  lexer->where_lastchar = end;
  lexer->parse_point = end + 1;
  return 1;
}

SIN stb__clex_eof(stb_lexer *lexer) {
  lexer->token = CLEX_eof;
  return 0;
}

SIN stb__clex_iswhite(SIN x) {
  return x == ' ' || x == '\t' || x == '\r' || x == '\n' || x == '\f';
}

const char *stb__strchr(const char *str, SIN ch) {
  for (; *str; ++str)
    if (*str == ch) return str;
  return 0;
}

SIN stb__clex_parse_suffixes(stb_lexer *lexer, long tokenid, char *start,
                             char *cur, const char *suffixes) {
#ifdef STB__clex_parse_suffixes
  lexer->string = lexer->string_storage;
  lexer->string_len = 0;

  while ((*cur >= 'a' && *cur <= 'z') || (*cur >= 'A' && *cur <= 'Z')) {
    if (stb__strchr(suffixes, *cur) == 0)
      return stb__clex_token(lexer, CLEX_parse_error, start, cur);
    if (lexer->string_len + 1 >= lexer->string_storage_len)
      return stb__clex_token(lexer, CLEX_parse_error, start, cur);
    lexer->string[lexer->string_len++] = *cur++;
  }
#else
  suffixes = suffixes;  // attempt to suppress warnings
#endif
  return stb__clex_token(lexer, tokenid, start, cur - 1);
}

#ifndef STB__CLEX_use_stdlib
double stb__clex_pow(double base, UIN exponent) {
  double value = 1;
  for (; exponent; exponent >>= 1) {
    if (exponent & 1) value *= base;
    base *= base;
  }
  return value;
}

double stb__clex_parse_float(char *p, char **q) {
  char *s = p;
  double value = 0;
  SIN base = 10;
  SIN exponent = 0;

#ifdef STB__clex_hex_floats
  if (*p == '0') {
    if (p[1] == 'x' || p[1] == 'X') {
      base = 16;
      p += 2;
    }
  }
#endif

  for (;;) {
    if (*p >= '0' && *p <= '9') value = value * base + (*p++ - '0');
#ifdef STB__clex_hex_floats
    else if (base == 16 && *p >= 'a' && *p <= 'f')
      value = value * base + 10 + (*p++ - 'a');
    else if (base == 16 && *p >= 'A' && *p <= 'F')
      value = value * base + 10 + (*p++ - 'A');
#endif
    else
      break;
  }

  if (*p == '.') {
    double pow, addend = 0;
    ++p;
    for (pow = 1;; pow *= base) {
      if (*p >= '0' && *p <= '9') addend = addend * base + (*p++ - '0');
#ifdef STB__clex_hex_floats
      else if (base == 16 && *p >= 'a' && *p <= 'f')
        addend = addend * base + 10 + (*p++ - 'a');
      else if (base == 16 && *p >= 'A' && *p <= 'F')
        addend = addend * base + 10 + (*p++ - 'A');
#endif
      else
        break;
    }
    value += addend / pow;
  }
#ifdef STB__clex_hex_floats
  if (base == 16) {
    // exponent required for hex FPC literal
    if (*p != 'p' && *p != 'P') {
      *q = s;
      return 0;
    }
    exponent = 1;
  } else
#endif
    exponent = (*p == 'e' || *p == 'E');

  if (exponent) {
    SIN sign = p[1] == '-';
    UIN exponent = 0;
    double power = 1;
    ++p;
    if (*p == '-' || *p == '+') ++p;
    while (*p >= '0' && *p <= '9') exponent = exponent * 10 + (*p++ - '0');

#ifdef STB__clex_hex_floats
    if (base == 16)
      power = stb__clex_pow(2, exponent);
    else
#endif
      power = stb__clex_pow(10, exponent);
    if (sign)
      value /= power;
    else
      value *= power;
  }
  *q = p;
  return value;
}
#endif

SIN stb__clex_parse_char(char *p, char **q) {
  if (*p == '\\') {
    *q = p + 2;  // tentatively guess we'll parse two characters
    switch (p[1]) {
      case '\\':
        return '\\';
      case '\'':
        return '\'';
      case '"':
        return '"';
      case 't':
        return '\t';
      case 'f':
        return '\f';
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case '0':
        return '\0';  // @TODO ocatal constants
      case 'x':
      case 'X':
        return -1;  // @TODO hex constants
      case 'u':
        return -1;  // @TODO unicode constants
    }
  }
  *q = p + 1;
  return (IUA)*p;
}

SIN stb__clex_parse_string(stb_lexer *lexer, char *p, SIN type) {
  char *start = p;
  char delim = *p++;  // grab the " or ' for later matching
  char *out = lexer->string_storage;
  char *outend = lexer->string_storage + lexer->string_storage_len;
  while (*p != delim) {
    SIN n;
    if (*p == '\\') {
      char *q;
      n = stb__clex_parse_char(p, &q);
      if (n < 0) return stb__clex_token(lexer, CLEX_parse_error, start, q);
      p = q;
    } else {
      // @OPTIMIZE: could speed this up by looping-while-not-backslash
      n = (IUA)*p++;
    }
    if (out + 1 > outend)
      return stb__clex_token(lexer, CLEX_parse_error, start, p);
    // @TODO expand unicode escapes to UTF8
    *out++ = (char)n;
  }
  *out = 0;
  lexer->string = lexer->string_storage;
  lexer->string_len = (SIN)(out - lexer->string_storage);
  return stb__clex_token(lexer, type, start, p);
}

SIN stb_c_lexer_get_token(stb_lexer *lexer) {
  char *p = lexer->parse_point;

  // skip whitespace and comments
  for (;;) {
#ifdef STB_C_LEX_ISWHITE
    while (p != lexer->stream_end) {
      SIN n;
      n = STB_C_LEX_ISWHITE(p);
      if (n == 0) break;
      if (lexer->eof && lexer->eof - lexer->parse_point < n)
        return stb__clex_token(tok, CLEX_parse_error, p, lexer->eof - 1);
      p += n;
    }
#else
    while (p != lexer->eof && stb__clex_iswhite(*p)) ++p;
#endif

    STB_C_LEX_CPP_COMMENTS(if (p != lexer->eof && p[0] == '/' && p[1] == '/') {
      while (p != lexer->eof && *p != '\r' && *p != '\n') ++p;
      continue;
    })

    STB_C_LEX_C_COMMENTS(if (p != lexer->eof && p[0] == '/' && p[1] == '*') {
      char *start = p;
      p += 2;
      while (p != lexer->eof && (p[0] != '*' || p[1] != '/')) ++p;
      if (p == lexer->eof)
        return stb__clex_token(lexer, CLEX_parse_error, start, p - 1);
      p += 2;
      continue;
    })

#ifdef STB__clex_discard_preprocessor
    // @TODO this discards everything after a '#', regardless
    // of where in the line the # is, rather than requiring it
    // be at the start. (because this parser doesn't otherwise
    // check for line breaks!)
    if (p != lexer->eof && p[0] == '#') {
      while (p != lexer->eof && *p != '\r' && *p != '\n') ++p;
      continue;
    }
#endif

    break;
  }

  if (p == lexer->eof) return stb__clex_eof(lexer);

  switch (*p) {
    default:
      if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_' ||
          (IUA)*p >= 128  // >= 128 is UTF8 char
              STB_C_LEX_DOLLAR_IDENTIFIER(|| *p == '$')) {
        SIN n = 0;
        lexer->string = lexer->string_storage;
        lexer->string_len = n;
        do {
          if (n + 1 >= lexer->string_storage_len)
            return stb__clex_token(lexer, CLEX_parse_error, p, p + n);
          lexer->string[n] = p[n];
          ++n;
        } while ((p[n] >= 'a' && p[n] <= 'z') || (p[n] >= 'A' && p[n] <= 'Z') ||
                 (p[n] >= '0' &&
                  p[n] <= '9')  // allow digits in middle of identifier
                 || p[n] == '_' ||
                 (IUA)p[n] >=
                     128 STB_C_LEX_DOLLAR_IDENTIFIER(|| p[n] == '$'));
        lexer->string[n] = 0;
        return stb__clex_token(lexer, CLEX_id, p, p + n - 1);
      }

      // check for EOF
      STB_C_LEX_0_IS_EOF(if (*p == 0) return stb__clex_eof(tok);)

    single_char:
      // not an identifier, return the character as itself
      return stb__clex_token(lexer, *p, p, p);

    case '+':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_INCREMENTS(if (p[1] == '+') return stb__clex_token(
                                   lexer, CLEX_plusplus, p, p + 1);)
        STB_C_LEX_C_ARITHEQ(if (p[1] == '=') return stb__clex_token(
                                lexer, CLEX_pluseq, p, p + 1);)
      }
      goto single_char;
    case '-':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_INCREMENTS(if (p[1] == '-') return stb__clex_token(
                                   lexer, CLEX_minusminus, p, p + 1);)
        STB_C_LEX_C_ARITHEQ(if (p[1] == '=') return stb__clex_token(
                                lexer, CLEX_minuseq, p, p + 1);)
        STB_C_LEX_C_ARROW(if (p[1] == '>') return stb__clex_token(
                              lexer, CLEX_arrow, p, p + 1);)
      }
      goto single_char;
    case '&':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_LOGICAL(if (p[1] == '&') return stb__clex_token(
                                lexer, CLEX_andand, p, p + 1);)
        STB_C_LEX_C_BITWISEEQ(if (p[1] == '=') return stb__clex_token(
                                  lexer, CLEX_andeq, p, p + 1);)
      }
      goto single_char;
    case '|':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_LOGICAL(if (p[1] == '|') return stb__clex_token(
                                lexer, CLEX_oror, p, p + 1);)
        STB_C_LEX_C_BITWISEEQ(if (p[1] == '=') return stb__clex_token(
                                  lexer, CLEX_oreq, p, p + 1);)
      }
      goto single_char;
    case '=':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_COMPARISONS(
            if (p[1] == '=') return stb__clex_token(lexer, CLEX_eq, p, p + 1);)
        STB_C_LEX_EQUAL_ARROW(if (p[1] == '>') return stb__clex_token(
                                  lexer, CLEX_eqarrow, p, p + 1);)
      }
      goto single_char;
    case '!':
      STB_C_LEX_C_COMPARISONS(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_noteq, p, p + 1);)
      goto single_char;
    case '^':
      STB_C_LEX_C_BITWISEEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_xoreq, p, p + 1));
      goto single_char;
    case '%':
      STB_C_LEX_C_ARITHEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_modeq, p, p + 1));
      goto single_char;
    case '*':
      STB_C_LEX_C_ARITHEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_muleq, p, p + 1));
      goto single_char;
    case '/':
      STB_C_LEX_C_ARITHEQ(
          if (p + 1 != lexer->eof && p[1] == '=') return stb__clex_token(
              lexer, CLEX_diveq, p, p + 1));
      goto single_char;
    case '<':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_COMPARISONS(if (p[1] == '=') return stb__clex_token(
                                    lexer, CLEX_lesseq, p, p + 1);)
        STB_C_LEX_C_SHIFTS(if (p[1] == '<') {
          STB_C_LEX_C_ARITHEQ(
              if (p + 2 != lexer->eof && p[2] == '=') return stb__clex_token(
                  lexer, CLEX_shleq, p, p + 2);)
          return stb__clex_token(lexer, CLEX_shl, p, p + 1);
        })
      }
      goto single_char;
    case '>':
      if (p + 1 != lexer->eof) {
        STB_C_LEX_C_COMPARISONS(if (p[1] == '=') return stb__clex_token(
                                    lexer, CLEX_greatereq, p, p + 1);)
        STB_C_LEX_C_SHIFTS(if (p[1] == '>') {
          STB_C_LEX_C_ARITHEQ(
              if (p + 2 != lexer->eof && p[2] == '=') return stb__clex_token(
                  lexer, CLEX_shreq, p, p + 2);)
          return stb__clex_token(lexer, CLEX_shr, p, p + 1);
        })
      }
      goto single_char;

    case '"':
      STB_C_LEX_C_DQ_STRINGS(
          return stb__clex_parse_string(lexer, p, CLEX_dqstring);)
      goto single_char;
    case '\'':
      STB_C_LEX_C_SQ_STRINGS(
          return stb__clex_parse_string(lexer, p, CLEX_sqstring);)
      STB_C_LEX_C_CHARS({
        char *start = p;
        lexer->int_number = stb__clex_parse_char(p + 1, &p);
        if (lexer->int_number < 0)
          return stb__clex_token(lexer, CLEX_parse_error, start, start);
        if (p == lexer->eof || *p != '\'')
          return stb__clex_token(lexer, CLEX_parse_error, start, p);
        return stb__clex_token(lexer, CLEX_charlit, start, p + 1);
      })
      goto single_char;

    case '0':
#if defined(STB__clex_hex_ints) || defined(STB__clex_hex_floats)
      if (p + 1 != lexer->eof) {
        if (p[1] == 'x' || p[1] == 'X') {
          char *q;

#ifdef STB__clex_hex_floats
          for (q = p + 2; q != lexer->eof && ((*q >= '0' && *q <= '9') ||
                                              (*q >= 'a' && *q <= 'f') ||
                                              (*q >= 'A' && *q <= 'F'));
               ++q)
            ;
          if (q != lexer->eof) {
            if (*q ==
                '.' STB_C_LEX_FLOAT_NO_DECIMAL(|| *q == 'p' || *q == 'P')) {
#ifdef STB__CLEX_use_stdlib
              lexer->real_number = strtod((char *)p, (char **)&q);
#else
              lexer->real_number = stb__clex_parse_float(p, &q);
#endif

              if (p == q) return stb__clex_token(lexer, CLEX_parse_error, p, q);
              return stb__clex_parse_suffixes(lexer, CLEX_floatlit, p, q,
                                              STB_C_LEX_FLOAT_SUFFIXES);
            }
          }
#endif  // STB__CLEX_hex_floats

#ifdef STB__clex_hex_ints
#ifdef STB__CLEX_use_stdlib
          lexer->int_number = strtol((char *)p, (char **)&q, 16);
#else
          {
            stb__clex_int n = 0;
            for (q = p + 2; q != lexer->eof; ++q) {
              if (*q >= '0' && *q <= '9')
                n = n * 16 + (*q - '0');
              else if (*q >= 'a' && *q <= 'f')
                n = n * 16 + (*q - 'a') + 10;
              else if (*q >= 'A' && *q <= 'F')
                n = n * 16 + (*q - 'A') + 10;
              else
                break;
            }
            lexer->int_number = n;
          }
#endif
          if (q == p + 2)
            return stb__clex_token(lexer, CLEX_parse_error, p - 2, p - 1);
          return stb__clex_parse_suffixes(lexer, CLEX_intlit, p, q,
                                          STB_C_LEX_HEX_SUFFIXES);
#endif
        }
      }
#endif  // defined(STB__clex_hex_ints) || defined(STB__clex_hex_floats)
        // can't test for octal because we might parse '0.0' as FPC or as '0'
      // '.' '0', so have to do FPC first

      /* FALL THROUGH */
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
#ifdef STB__clex_decimal_floats
    {
      char *q = p;
      while (q != lexer->eof && (*q >= '0' && *q <= '9')) ++q;
      if (q != lexer->eof) {
        if (*q == '.' STB_C_LEX_FLOAT_NO_DECIMAL(|| *q == 'e' || *q == 'E')) {
#ifdef STB__CLEX_use_stdlib
          lexer->real_number = strtod((char *)p, (char **)&q);
#else
          lexer->real_number = stb__clex_parse_float(p, &q);
#endif

          return stb__clex_parse_suffixes(lexer, CLEX_floatlit, p, q,
                                          STB_C_LEX_FLOAT_SUFFIXES);
        }
      }
    }
#endif  // STB__clex_decimal_floats

#ifdef STB__clex_octal_ints
      if (p[0] == '0') {
        char *q = p;
#ifdef STB__CLEX_use_stdlib
        lexer->int_number = strtol((char *)p, (char **)&q, 8);
#else
        stb__clex_int n = 0;
        while (q != lexer->eof) {
          if (*q >= '0' && *q <= '7')
            n = n * 8 + (*q - '0');
          else
            break;
          ++q;
        }
        if (q != lexer->eof && (*q == '8' || *q == '9'))
          return stb__clex_token(lexer, CLEX_parse_error, p, q);
        lexer->int_number = n;
#endif
        return stb__clex_parse_suffixes(lexer, CLEX_intlit, p, q,
                                        STB_C_LEX_OCTAL_SUFFIXES);
      }
#endif  // STB__clex_octal_ints

#ifdef STB__clex_decimal_ints
      {
        char *q = p;
#ifdef STB__CLEX_use_stdlib
        lexer->int_number = strtol((char *)p, (char **)&q, 10);
#else
        stb__clex_int n = 0;
        while (q != lexer->eof) {
          if (*q >= '0' && *q <= '9')
            n = n * 10 + (*q - '0');
          else
            break;
          ++q;
        }
        lexer->int_number = n;
#endif
        return stb__clex_parse_suffixes(lexer, CLEX_intlit, p, q,
                                        STB_C_LEX_OCTAL_SUFFIXES);
      }
#endif  // STB__clex_decimal_ints
      goto single_char;
  }
}
#endif  // STB_C_LEXER_IMPLEMENTATION

#ifdef STB_C_LEXER_SELF_TEST
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void print_token(stb_lexer *lexer) {
  switch (lexer->token) {
    case CLEX_id:
      printf("_%s", lexer->string);
      break;
    case CLEX_eq:
      printf("==");
      break;
    case CLEX_noteq:
      printf("!=");
      break;
    case CLEX_lesseq:
      printf("<=");
      break;
    case CLEX_greatereq:
      printf(">=");
      break;
    case CLEX_andand:
      printf("&&");
      break;
    case CLEX_oror:
      printf("||");
      break;
    case CLEX_shl:
      printf("<<");
      break;
    case CLEX_shr:
      printf(">>");
      break;
    case CLEX_plusplus:
      printf("++");
      break;
    case CLEX_minusminus:
      printf("--");
      break;
    case CLEX_arrow:
      printf("->");
      break;
    case CLEX_andeq:
      printf("&=");
      break;
    case CLEX_oreq:
      printf("|=");
      break;
    case CLEX_xoreq:
      printf("^=");
      break;
    case CLEX_pluseq:
      printf("+=");
      break;
    case CLEX_minuseq:
      printf("-=");
      break;
    case CLEX_muleq:
      printf("*=");
      break;
    case CLEX_diveq:
      printf("/=");
      break;
    case CLEX_modeq:
      printf("%%=");
      break;
    case CLEX_shleq:
      printf("<<=");
      break;
    case CLEX_shreq:
      printf(">>=");
      break;
    case CLEX_eqarrow:
      printf("=>");
      break;
    case CLEX_dqstring:
      printf("\"%s\"", lexer->string);
      break;
    case CLEX_sqstring:
      printf("'\"%s\"'", lexer->string);
      break;
    case CLEX_charlit:
      printf("'%s'", lexer->string);
      break;
#if defined(STB__clex_int_as_double) && !defined(STB__CLEX_use_stdlib)
    case CLEX_intlit:
      printf("#%g", lexer->real_number);
      break;
#else
    case CLEX_intlit:
      printf("#%ld", lexer->int_number);
      break;
#endif
    case CLEX_floatlit:
      printf("%g", lexer->real_number);
      break;
    default:
      if (lexer->token >= 0 && lexer->token < 256)
        printf("%c", (SIN)lexer->token);
      else {
        printf("<<<UNKNOWN TOKEN %ld >>>\n", lexer->token);
      }
      break;
  }
}

/* Force a test
of parsing
multiline comments */

/*/ comment /*/
/**/ extern /**/

    void
    dummy(void) {
  double some_floats[] = {
    1.0501,
    -10.4e12,
    5E+10,
#if 0  // not supported in C++ or C-pre-99, so don't try to compile it, but let
       // our parser test it
      0x1.0p+24, 0xff.FP-8, 0x1p-23,
#endif
    4.
  };
  (void)sizeof(some_floats);
  (void)some_floats[1];

  printf("test %d", 1);  // https://github.com/nothings/stb/issues/13
}

SIN main(SIN argc, char **argv) {
  FILE *f = fopen("stb_c_lexer.h", "rb");
  char *text = (char *)malloc(1 << 20);
  SIN len = f ? (SIN)fread(text, 1, 1 << 20, f) : -1;
  stb_lexer lex;
  if (len < 0) {
    fprintf(stderr, "Error opening file\n");
    free(text);
    fclose(f);
    return 1;
  }
  fclose(f);

  stb_c_lexer_init(&lex, text, text + len, (char *)malloc(0x10000), 0x10000);
  while (stb_c_lexer_get_token(&lex)) {
    if (lex.token == CLEX_parse_error) {
      printf("\n<<<PARSE ERROR>>>\n");
      break;
    }
    print_token(&lex);
    printf("  ");
  }
  return 0;
}
#endif
