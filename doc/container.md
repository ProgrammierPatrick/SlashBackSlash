# Container

## General
Container are Functionals that can contain multiple Functionals

## Tuple
Tuples are container which conatains 2 Elements

`_T h b a`  
This function returns a tuple  
&nbsp;&nbsp;-*h*: the first Functional of the Tuple  
&nbsp;&nbsp;-*b*: the second Functional of the Tuple  
&nbsp;&nbsp;-*a*: a Boolesn controling the access to the Elements  
&nbsp;&nbsp;-*returns*: *h* if *a* is true else *b*

`first p`  
returns the first element of an tuple  
&nbsp;&nbsp;-*p*: a Tuple  
&nbsp;&nbsp;-*returns*: the first element of the tuple

`second p`  
returns the first element of an tuple  
&nbsp;&nbsp;-*p*: a Tuple  
&nbsp;&nbsp;-*returns*: the second element of the tuple

## List
`<list> f x`  
Lists are container which can contain any amount of Elements  
&nbsp;&nbsp;-*f*: a functional with two arguments that should be applied to each Element of the List  
&nbsp;&nbsp;-*x*: the inital value  
&nbsp;&nbsp;-*returns*: *f* applied to each Element of the list and *x*


`_L`  
The function returns an empty list  
&nbsp;&nbsp;-*returns*:an empty List

`. a l` `append a l`  
This function appends a Element as the head of an List  
&nbsp;&nbsp;-*a*: a Functional that should be appended to the List  
&nbsp;&nbsp;-*l*: the List where the new Element should be added  
&nbsp;&nbsp;-*returns*: a List with head *a* and tail *l*  
&nbsp;&nbsp;-*note*: `a . b` can be used to concatenate 2 Lists

`len l`  
This function returns the length of the List  
&nbsp;&nbsp;-*l*: a List  
&nbsp;&nbsp;-*returns* the length of *l*

--TODO add missing



