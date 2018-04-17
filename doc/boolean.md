# Boolean

## General use
```<bool> t f```  
Booleans are functionals, which can control the execution flow of programs  
&nbsp;&nbsp;-*t*: this functional will be returned if `<bool>` is true  
&nbsp;&nbsp;-*f*: this functional will be returned if `<bool>` is false  
&nbsp;&nbsp;-*returns*: one of the functionals *t* or *f*

## Values
```true t f```  
Returns the first Argument  
&nbsp;&nbsp;-*t*: this functional will be returned  
&nbsp;&nbsp;-*f*: this functional will be ignored  
&nbsp;&nbsp;-*returns*: the functional *t*

```false t f```  
Returns the second Argument  
&nbsp;&nbsp;-*t*: this functional will be ignored  
&nbsp;&nbsp;-*f*: this functional will be returned  
&nbsp;&nbsp;-*returns*: the functional *f*

## Functions
```not b```  
Negates the Argument    
&nbsp;&nbsp;-*b*: this boolean will be negated  
&nbsp;&nbsp;-*returns*: the negation of *b*

```and a b```  
Conjungates the Arguments  
&nbsp;&nbsp;-*a*: the first boolean to conjungate  
&nbsp;&nbsp;-*b*: the second boolean to conjungate  
&nbsp;&nbsp;-*returns*: the conjugation of *a* and *b*

```or a b```  
Disjungates the Arguments  
&nbsp;&nbsp;-*a*: the first boolean to disjungate  
&nbsp;&nbsp;-*b*: the second boolean to disjungate  
&nbsp;&nbsp;-*returns*: the disjunction of *a* and *b*

```xor a b```  
Exclusivly disjungates the Arguments  
&nbsp;&nbsp;-*a*: the first boolean to exclusivly disjungate  
&nbsp;&nbsp;-*b*: the second boolean to exclusivly disjungate  
&nbsp;&nbsp;-*returns*: the exclusiv disjunction of *a* and *b*

## Cast to
`<N-B> n`  
Casts a Numeral to a Boolean  
&nbsp;&nbsp;-*n*: the Numeral that should be casted  
&nbsp;&nbsp;-*returns*: true if *n* is not 0 else false

`<I-B> i`  
Casts a Integer to a Boolean  
&nbsp;&nbsp;-*i*: the Integer that should be casted  
&nbsp;&nbsp;-*returns*: true if *i* is not 0 else false

## Cast from
`<B-N> b`  
Casts a Boolean to a Numeral  
&nbsp;&nbsp;-*b*: the Boolean that should be casted  
&nbsp;&nbsp;-*returns*: 1 if *b* is true else 0

`<B-I> b`  
Casts a Boolean to a Integer  
&nbsp;&nbsp;-*b*: the Boolean that should be casted  
&nbsp;&nbsp;-*returns*: 1 if *b* is true else 0

