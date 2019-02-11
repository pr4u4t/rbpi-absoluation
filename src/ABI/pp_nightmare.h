#ifndef _PP_NIGHTMARE_H_
#define _PP_NIGHTMARE_H_

/* -- standard map macro -- */

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...)  EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_END(...)
#define MAP_OUT

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0(test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, udata, x, peek, ...) f(udata, x) MAP_NEXT(peek, MAP1)(f, udata, peek, __VA_ARGS__)
#define MAP1(f, udata, x, peek, ...) f(udata, x) MAP_NEXT(peek, MAP0)(f, udata, peek, __VA_ARGS__)

#define MAP(f, udata, ...) EVAL(MAP1(f, udata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/* -- map macro used in map macro -- */

#define XEVAL0(...) __VA_ARGS__
#define XEVAL1(...) XEVAL0(XEVAL0(XEVAL0(__VA_ARGS__)))
#define XEVAL2(...) XEVAL1(XEVAL1(XEVAL1(__VA_ARGS__)))
#define XEVAL3(...) XEVAL2(XEVAL2(XEVAL2(__VA_ARGS__)))
#define XEVAL4(...) XEVAL3(XEVAL3(XEVAL3(__VA_ARGS__)))
#define XEVAL(...)  XEVAL4(XEVAL4(XEVAL4(__VA_ARGS__)))

#define XMAP_END(...)
#define XMAP_OUT

#define XMAP_GET_END2() 0, XMAP_END
#define XMAP_GET_END1(...) XMAP_GET_END2
#define XMAP_GET_END(...) XMAP_GET_END1
#define XMAP_NEXT0(test, next, ...) next XMAP_OUT
#define XMAP_NEXT1(test, next) XMAP_NEXT0(test, next, 0)
#define XMAP_NEXT(test, next)  XMAP_NEXT1(XMAP_GET_END test, next)

#define XMAP0(f, udata, x, peek, ...) f(udata, x) XMAP_NEXT(peek, XMAP1)(f, udata, peek, __VA_ARGS__)
#define XMAP1(f, udata, x, peek, ...) f(udata, x) XMAP_NEXT(peek, XMAP0)(f, udata, peek, __VA_ARGS__)

#define XMAP(f, udata, ...) XEVAL(XMAP1(f, udata, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/* -- macro argument count -- */

#define PP_NARG(...) PP_NARG_(_,##__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(_, ...) PP_ARG_N(__VA_ARGS__)

#define PP_ARG_N( \
     _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
    _61,_62,_63,  N, ...) N
#define PP_RSEQ_N() \
    63,62,61,60,                   \
    59,58,57,56,55,54,53,52,51,50, \
    49,48,47,46,45,44,43,42,41,40, \
    39,38,37,36,35,34,33,32,31,30, \
    29,28,27,26,25,24,23,22,21,20, \
    19,18,17,16,15,14,13,12,11,10, \
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0

/* -- conditionals -- */

#define SECOND(a, b, ...) b
#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1
#define CAT(a,b) a ## b
#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()
#define BOOL(x) NOT(NOT(x))

#define IF_ELSE(condition) _IF_ELSE(BOOL(condition))
#define _IF_ELSE(condition) CAT(_IF_, condition)
#define _IF_1(...) __VA_ARGS__ _IF_1_ELSE
#define _IF_0(...)             _IF_0_ELSE

#define _IF_1_ELSE(...)
#define _IF_0_ELSE(...) __VA_ARGS__

#define IFF(condition) _IFF(BOOL(condition))
#define _IFF(condition) CAT(_IFF_,condition)
#define _IFF_1(...) __VA_ARGS__
#define _IFF_0(...) 

#define CODE_SECTION(...) _CODE_SECTION(PP_NARG(__VA_ARGS__))
#define _CODE_SECTION(condition) CAT(_CODE_SECTION_,condition)
#define _CODE_SECTION_1(...) 
#define _CODE_SECTION_0(...) __VA_ARGS__


/* -- utils -- */

#define ENCAPSULE(...)				(__VA_ARGS__) 
#define DECAPSULE(...)				__VA_ARGS__
#define stringify(a) #a
#define static_cast(T,O) ((T) O)
#define cat(a,b) a ## b

#define EXPAND1(...) 				__VA_ARGS__
#define EXPAND0(...) 				EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__)))
#define EXPAND(...) 				EXPAND0(EXPAND0(EXPAND0(__VA_ARGS__)))

#endif
