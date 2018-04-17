# Control Structure

## General
Control Structures control the execution flow of programs

## Functions
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
&nbsp;**public childs**  
&nbsp;`R r b f i`  
&nbsp;This function will be called from while with the Y-Combinator  
&nbsp;&nbsp;&nbsp;-*r*: this should be `Y R` for the recursion  
&nbsp;&nbsp;&nbsp;-*b*: a Functional that takes one Argument and returns a Boolean  
&nbsp;&nbsp;&nbsp;-*f*: a Functional that takes one Argument that should be applied to *i*  
&nbsp;&nbsp;&nbsp;-*i*: a Functional that repressents the state of the loop  
