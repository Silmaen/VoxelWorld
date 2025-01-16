# Coding Guide lines

## indentation

* indent with spaces characters
    * 4-chars spaces
* no indent the preprocessor directives.

## braces

Almost always on the same line as the statement.

## Naming conventions

If no particular rule, use lowerCameCase convention.

### namespaces

* only lowercase characters
    * no digits, no special characters
* no more than 8 characters
* nested namespaces
* no indentation

### classes & structs

* UpperCamelCase name
* no indentation for public, private, protected.
* private member naming
    * prefix `m_` member

### Method and function parameter

* UpperCamelCase name with prefix
* prefix `i` for input parameter
* prefix `io` for input & output parameter
* prefix `o` for output parameter
