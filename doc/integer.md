# Integer

## General use
`<int> p n b`  
Integer containing a positive and negative part. In Normal Form one of these parts is 0  
&nbsp;&nbsp;-*p*: a Numeral representing the positive part  
&nbsp;&nbsp;-*n*: a Numeral representing the negative part  
&nbsp;&nbsp;-*b*: a Boolean for selecting a part  
&nbsp;&nbsp;-*returns*: if *b* is true *p* else *n*

## Constructor
`_I p b`  
Creates a new Integer in Normal Form  
&nbsp;&nbsp;-*p*: a Numeral representing the positive part  
&nbsp;&nbsp;-*n*: a Numeral representing the negative part  
&nbsp;&nbsp;-*returns*: the Integer *p* - *n*

## Values
--todo

## Arithmetic
`add_I i j`  
Adds the values of the inputs  
&nbsp;&nbsp;-*i*: a Integer that should be the first summand  
&nbsp;&nbsp;-*j*: a Integer that should be the second summand  
&nbsp;&nbsp;-*returns*: *i* + *j*  

`sub_I i j`  
Subtracts the values of the inputs  
&nbsp;&nbsp;-*i*: a Integer that should be the minuend  
&nbsp;&nbsp;-*j*: a Integer that should be the subtrahend  
&nbsp;&nbsp;-*returns*: *i* - *j*

`mul_I i j`  
Multiplies the values of the inputs  
&nbsp;&nbsp;-*i*: a Integer that should be the first factor  
&nbsp;&nbsp;-*j*: a Integer that should be the second factor  
&nbsp;&nbsp;-*returns*: *n* * *m*

`div_I n m`  
Divides the values of the inputs and rounds toward 0  
&nbsp;&nbsp;-*n*: a Integer that should be the dividend  
&nbsp;&nbsp;-*m*: a Integer that should be the divisor  
&nbsp;&nbsp;-*returns*: *n* / *m* rounded toward 0

`sdiv_I n m`  
Divides the values of the inputs away from 0  
&nbsp;&nbsp;-*n*: a Integer that should be the dividend  
&nbsp;&nbsp;-*m*: a Integer that should be the divisor  
&nbsp;&nbsp;-*returns*: *n* / *m* rounded away from 0

## Comparison
`ìs0_I i`  
Tests if argument is 0  
&nbsp;&nbsp;-*i*: the Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is 0 then true else false

`pos_I i`  
Tests if argument is greater than 0  
&nbsp;&nbsp;-*i*: the Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is greater than 0 then true else false

`neg_I i`  
Tests if argument is less than 0  
&nbsp;&nbsp;-*i*: the Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is less than 0 then true else false

`ge_I i j`  
Tests if first argument is greater or equal to second argument  
&nbsp;&nbsp;-*i*: the first Integer that should be checked  
&nbsp;&nbsp;-*j*: the second Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is greater or equal *j* then true else false

`gt_I i j`  
Tests if first argument is greater then second argument  
&nbsp;&nbsp;-*i*: the first Integer that should be checked  
&nbsp;&nbsp;-*j*: the second Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is greater *j* then true else false

`le_I i j`  
Tests if first argument is less or equal to second argument  
&nbsp;&nbsp;-*i*: the first Integer that should be checked  
&nbsp;&nbsp;-*j*: the second Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is less or equal *j* then true else false

`lt_I i j`  
Tests if first argument is less then second argument  
&nbsp;&nbsp;-*i*: the first Integer that should be checked  
&nbsp;&nbsp;-*j*: the second Integer that should be checked  
&nbsp;&nbsp;-*returns*: if *i* is less *j* then true else false

## Cast to
`<B-I> b`  
Casts a Boolean to a Integer  
&nbsp;&nbsp;-*b*: the Boolean that should be casted  
&nbsp;&nbsp;-*returns*: 1 if *b* is true else 0

`<N-I> i`  
Casts a Numeral to a Integer  
&nbsp;&nbsp;-*n*: the Numeral that should be casted  
&nbsp;&nbsp;-*returns*: an Integer with the same value as *n*

## Cast from
`<I-B> n`  
Casts a Integer to a Boolean  
&nbsp;&nbsp;-*n*: the Integer that should be casted  
&nbsp;&nbsp;-*returns*: true if *n* is not 0 else false

`<I-N> n`  
Casts a Integer to a Numeral  
&nbsp;&nbsp;-*i*: the Integer that should be casted  
&nbsp;&nbsp;-*returns*: the value of the integer if it is positive else 0
