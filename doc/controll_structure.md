# Control Structure

## General
Control Structures control the execution flow of programs

## Functions
`Y y`  
The Y-Combinator can be used to write recursive functions  
&nbsp;&nbsp;-*y*: a Functional that should be called recursivly  
&nbsp;&nbsp;-*returns*: `y (Y y)`

`if b t f`  
Calls a different function depending on a Boolean  
&nbsp;&nbsp;-*b*: the Boolean that chooses the function  
&nbsp;&nbsp;-*t*: the Functional that should be returned if *b* is true  
&nbsp;&nbsp;-*f*: the Functional that should be returned if *b* is false  
&nbsp;&nbsp;-*returns*: either *t* or *b*  
&nbsp;&nbsp;-*note*: `if b` does the same as `b`

`while b f i`  
Applies *f* to *i* until `b i`is true  
&nbsp;&nbsp;-*b*: a Functional that takes one Argument and returns a Boolean  
&nbsp;&nbsp;-*f*: a Functional that takes one Argument that should be applied multiple times to *i*  
&nbsp;&nbsp;-*i*: a Functional that repressents the inital state of the loop  
&nbsp;&nbsp;-*returns*: *f* applied multiple times to *i*  
 
&nbsp;&nbsp;**public childs**  
&nbsp;&nbsp;`R r b f i`  
&nbsp;&nbsp;This function will be called from while with the Y-Combinator  
&nbsp;&nbsp;&nbsp;&nbsp;-*r*: this should be `Y R` for the recursion  
&nbsp;&nbsp;&nbsp;&nbsp;-*b*: a Functional that takes one Argument and returns a Boolean  
&nbsp;&nbsp;&nbsp;&nbsp;-*f*: a Functional that takes one Argument that should be applied to *i*  
&nbsp;&nbsp;&nbsp;&nbsp;-*i*: a Functional that repressents the state of the loop  
&nbsp;&nbsp;&nbsp;&nbsp;-*returns*: *f* applied multiple times to *i* 

`for n f i`  
Calls *n* times *f* with a counter and the state of the loop  
&nbsp;&nbsp;-*n*: a Numeral difining how often the loop should be called  
&nbsp;&nbsp;-*f*: a Functional that takes a Numeral and a second Argument  
&nbsp;&nbsp;-*i*: a Functional that repressents the inital state of the loop  
&nbsp;&nbsp;-*returns*: *f* applied n times to *i* with a counter

&nbsp;&nbsp;**public childs**  
&nbsp;&nbsp;`R r c n f i`  
&nbsp;&nbsp;This function will be called from for with the Y-Combinator  
&nbsp;&nbsp;&nbsp;&nbsp;-*r*: this should be `Y R` for the recursion  
&nbsp;&nbsp;&nbsp;&nbsp;-*c*: a Numeral that stores the current number of iteration  
&nbsp;&nbsp;&nbsp;&nbsp;-*n*: a Numeral difining how often the loop should be called  
&nbsp;&nbsp;&nbsp;&nbsp;-*f*: a Functional that takes a Numeral and a second Argument  
&nbsp;&nbsp;&nbsp;&nbsp;-*i*: a Functional that repressents the inital state of the loop  
&nbsp;&nbsp;&nbsp;&nbsp;-*returns*: *f* applied n times to *i* with a counter
