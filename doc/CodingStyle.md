# Coding Guide lines

## Tools and configuration.

Most of the style guidelines are defined in the `.clang-*` files
at the root of the sources. Please activate `clang-format` &
`clang-tidy` in your preferred IDE, in particular please use
`format on save` functions of your IDE.

## Indentation

* indent with spaces characters
    * 4-chars spaces
* no indent the preprocessor directives.
* no indent namespaces.

## Empty line

* avoid 2 consecutive empty lines.
* one empty line before function or method definition or declaration
* one empty line between scopes of block.

## Braces

Almost always on the same line as the statement.

## Naming conventions

If no particular rule, use lowerCameCase convention.

### Global variables

* Global constants (any type of constant):
    * prefix `g_`
* Static variable
    * prefix `gs_`

### Macros

Macros should be named with `All_UPPER` convention.

Avoid all constant declared as `#define` prefer define global constant.
in particular with `constexpr`.

### namespaces

* only lowercase characters
    * no digits, no special characters
* no more than 8 characters
* nested namespaces
* no indentation

* use anonymous namespace instead of `static` everywhere it is possible.

### classes & structs

* UpperCamelCase name
* no indentation for public, private, protected.
* private member naming
    * prefix `m_` for regular member
    * prefix `mu_` for unique pointer member
    * prefix `mp_` for pointer member
    * prefix `s_` for static members
* methods naming:
    * lowerCamelCase.
    * Getter, setter: `[get,set]Property` (for member like `m_property`)
    * Accessors (for member like `T m_property;`)
        * Const: `const T& property()const {return m_property;}`
        * Non-const: `T& property() {return m_property;}`

### Method and function parameter

* lowerCamelCase naming.
* getter & setters
    * prefer to avoid [get,set]Property

### Method & function parameters

* UpperCamelCase name with prefix
* prefix `i` for input parameter
* prefix `io` for input & output parameter
* prefix `o` for output parameter
