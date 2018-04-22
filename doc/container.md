# Container

## General
Container are Functionals that can contain multiple Functionals

## Tuple
`<tuple> a`  
Tuples are container which conatains 2 Elements  
&nbsp;&nbsp;-*a*: a Boolean controling the access to the Elements  
&nbsp;&nbsp;-*returns*: if true returns the first ELement els the second

`_T h b`  
This function returns a tuple  
&nbsp;&nbsp;-*h*: the first Functional of the Tuple  
&nbsp;&nbsp;-*b*: the second Functional of the Tuple  
&nbsp;&nbsp;-*returns*: the Tuple (*h*, *b*)

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

`head \l`  
this function returns the first Element of the list  
&nbsp;&nbsp;-*l*: a List  
&nbsp;&nbsp;-*returns*: the head of the List

`tail \l`  
this function returns a copy of a List without the first Element  
&nbsp;&nbsp;-*l*: a List  
&nbsp;&nbsp;-*returns*: the tail of the List

`isEmpty l`
this function checks if the List is Empty  
&nbsp;&nbsp;-*l*: a List  
&nbsp;&nbsp;-*returns*: true if List is a Empty List else return false

`get l n`  
this Function returns the n-th Element of List  
&nbsp;&nbsp;-*l*: a List  
&nbsp;&nbsp;-*n*: a number  
&nbsp;&nbsp;-*returns*: returns the *n*-th element from *n*

