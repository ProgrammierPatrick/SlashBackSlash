### Boolean
General use: ```<bool> t f```
Booleans are functionals, which can control the execution flow of programs
-*t*: this functional will be returned if `<bool>` is true
-*f*: this functional will be returned if `<bool>` is false
-*returns*: one of the functionals *t* or *f*

```true t f```
Returns the first Argument
-*t*: this functional will be returned
-*f*: this functional will be ignored
-*returns*: the functional *t*

```false t f```
Returns the second Argument
-*t*: this functional will be ignored
-*f*: this functional will be returned
-*returns*: the functional *f*

```not b```
Negates the Argument
-*b*: this boolean will be negated
-*returns*: the negation of *b*

```and a b```
Conjungates the Arguments
-*a*: the first boolean to conjungate
-*b*: the second boolean to conjungate
-*returns*: the conjugation of *a* and *b*

```or a b```
Disjungates the Arguments
-*a*: the first boolean to disjungate
-*b*: the second boolean to disjungate
-*returns*: the disjunction of *a* and *b*

```xor a b```
Exclusivly disjungates the Arguments
-*a*: the first boolean to exclusivly disjungate
-*b*: the second boolean to exclusivly disjungate
-*returns*: the exclusiv disjunction of *a* and *b*


