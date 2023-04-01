# from https://discourse.cmake.org/t/why-cmake-functions-cant-return-value/1710/3
cmake_minimum_required(VERSION 3.18)

# Tiny library. Extend as desired.

macro(return_)
    set(${ARGS} ${ARGN} PARENT_SCOPE)
    return()
endmacro()

function(fun_expr ARGS)
    unset(args)
    unset(fxns)
    while(ARGN)
        list(POP_FRONT ARGN tok)
        if (tok STREQUAL "(")
            list(APPEND args ${tok})
        elseif (tok STREQUAL ")")
            unset(call)
            unset(arg)
            list(POP_BACK args arg)
            while (args AND NOT arg STREQUAL "(")
                list(PREPEND call "${arg}")
                list(POP_BACK args arg)
            endwhile ()
            list(POP_BACK fxns fn)
            cmake_language(CALL ${fn} RET ${call})
            list(APPEND args ${RET})
        else ()
            list(APPEND fxns ${tok})
        endif ()
    endwhile()
    return_("${args}")
endfunction()

# Definitions from the blog post

function(format_name ARGS first last)
    return_("First: ${first}, last: ${last}")
endfunction()

#from https://stackoverflow.com/questions/22487215/return-a-list-from-the-function-using-out-parameter
function(AddToListFromFunction OutVariable Element )
    if ("${${OutVariable}}" STREQUAL "")
        message(STATUS "1")
        set(${OutVariable} ${Element} PARENT_SCOPE)
        message(STATUS "OutVariable: ${OutVariable} ${${OutVariable}}")
    else ()
        message(STATUS "2")
        set(${OutVariable} "${${OutVariable}}" "${Element}" PARENT_SCOPE)
    endif ()
endfunction()
