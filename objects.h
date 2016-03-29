#ifndef __RAY_OBJECTS_H
#define __RAY_OBJECTS_H

/* # is can covert a macro argument into string constant

example:

#define str(x) #x

str(string)
===> string

https://gcc.gnu.org/onlinedocs/cpp/Stringification.html */

/* ## it just like a adhesives, it can make "NAME" + "_command" = "NAME_command"

example:

#define COMMAND( NAME) { #NAME , NAME ## _command }

COMMAND( I_want_be_with_u )
===> { "I_want_be_with_u" , I_want_be_with_u_command }


https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html */
#define DECLARE_OBJECT(name) \
    struct __##name##_node; \
    typedef struct __##name##_node *name##_node; \
    struct __##name##_node { \
        name element; \
        name##_node next; \
    }; \
    void append_##name(const name *X, name##_node *list); \
    void delete_##name##_list(name##_node *list);

DECLARE_OBJECT(light)
DECLARE_OBJECT(rectangular)
DECLARE_OBJECT(sphere)

#undef DECLARE_OBJECT

#endif
