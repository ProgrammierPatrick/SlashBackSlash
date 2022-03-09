# Numeral

## General use
`<num> f x`  
Numerals represent numbers. They can be used to aplly functions repeatedly  
&nbsp;&nbsp;-*f*: a functional with one argument that should be applied `<num>` times  
&nbsp;&nbsp;-*x*: a functional *f* should be applied to  
&nbsp;&nbsp;-*returns*: the result of applying *f* `<num>` times to *x*

## Values
--todo

## Arithmetic
`+ n` `inc n`  
Increases the value of the input  
&nbsp;&nbsp;-*n*: a numeral that should be increased  
&nbsp;&nbsp;-*returns*: *n* + 1  
&nbsp;&nbsp;-*note*: you can write `n + m` to add n and m

`dec n`  
Decreases the value of the input  
&nbsp;&nbsp;-*n*: a numeral that should be decreased  
&nbsp;&nbsp;-*returns*: *n* - 1; if *n* is 0 returns 0

`add n m`  
Adds the values of the inputs  
&nbsp;&nbsp;-*n*: a numeral that should be the first summand  
&nbsp;&nbsp;-*m*: a numeral that should be the second summand  
&nbsp;&nbsp;-*returns*: *n* + *m*  
&nbsp;&nbsp;-*note*: `n + m` and `add n m` are equivalent

`sub n m`  
Subtracts the values of the inputs  
&nbsp;&nbsp;-*n*: a numeral that should be the minuend  
&nbsp;&nbsp;-*m*: a numeral that should be the subtrahend  
&nbsp;&nbsp;-*returns*: *n* - *m*; if *m* is greater than *n* returns 0

`mul n m`  
Multiplies the values of the inputs  
&nbsp;&nbsp;-*n*: a numeral that should be the first factor  
&nbsp;&nbsp;-*m*: a numeral that should be the second factor  
&nbsp;&nbsp;-*returns*: *n* * *m*

`div n m`  
Floor divides the values of the inputs  
&nbsp;&nbsp;-*n*: a numeral that should be the dividend  
&nbsp;&nbsp;-*m*: a numeral that should be the divisor  
&nbsp;&nbsp;-*returns*: &lfloor;*n* / *m*&rfloor;

`sdiv n m`  
Ceil divides the values of the inputs  
&nbsp;&nbsp;-*n*: a numeral that should be the dividend  
&nbsp;&nbsp;-*m*: a numeral that should be the divisor  
&nbsp;&nbsp;-*returns*: &lceil;*n* / *m*&rceil;

`mod n m`  
Calculates the modulo of the inputs  
&nbsp;&nbsp;-*n*: a numeral that should be the dividend  
&nbsp;&nbsp;-*m*: a numeral that should be the divisor  
&nbsp;&nbsp;-*returns*: *n* mod *m*

## Comparison  
`ìs0 n`  
Tests if argument is 0  
&nbsp;&nbsp;-*n*: the numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is 0 then true else false

`even n`
Tests if argument is even
&nbsp;&nbsp;-*n*: the numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is even then true else false

`odd n`
Tests if argument is even
&nbsp;&nbsp;-*n*: the numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is odd then true else false

`ge n m`  
Tests if first argument is greater or equal to second argument  
&nbsp;&nbsp;-*n*: the first numeral that should be checked  
&nbsp;&nbsp;-*m*: the second numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is greater or equal *m* then true else false

`gt n m`  
Tests if first argument is greater then second argument  
&nbsp;&nbsp;-*n*: the first numeral that should be checked  
&nbsp;&nbsp;-*m*: the second numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is greater *m* then true else false

`le n m`  
Tests if first argument is less or equal to second argument  
&nbsp;&nbsp;-*n*: the first numeral that should be checked  
&nbsp;&nbsp;-*m*: the second numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is less or equal *m* then true else false

`lt n m`  
Tests if first argument is less then second argument  
&nbsp;&nbsp;-*n*: the first numeral that should be checked  
&nbsp;&nbsp;-*m*: the second numeral that should be checked  
&nbsp;&nbsp;-*returns*: if *n* is less *m* then true else false
