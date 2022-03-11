# Common Functions
## Definitions
`id`, `_` := `(\x x)`  
`Y` := `(\f (\x f (x x)) (\x f (x x)))`  

## Correctness
### z.z. `id x` = `x`
`id x`  
`(\x x) x`  
`x`  

### z.z. `Y f` = `f (Y f)`
`Y f`  
`(\f (\x f (x x)) (\x f (x x))) f`  
`(\x f (x x)) (\x f (x x))`  
`f ((\x f (x x)) (\x f (x x)))`  
`f ((\f (\x f (x x)) (\x f (x x))) f)` 
`f (Y f)`  

# Logic
## Definitions
`true` :=  `(\t \f t)`  
`false` :=  `(\t \f f)`  
`and` := `(\a \b a b a)`  
`or` :=  `(\a \b a a b)`  
`not` := `(\a a false true)`  
`xor` := `(\a \b a (not b) b)`
## Correctness
there are only finite combinations to test

# Tuples
## Definitions
`tup` := `(\a \b \f f a b)`  
`first` := `(\t t true)`  
`second` := `(\t t false)`  
## Correctness
### z.z. `first (tup a b)` = `a`
`first (tup a b)`  
`(\t t true) (tup a b)`  
`tup a b true`  
`(\a \b \f f a b) a b true`  
`true a b`  
`(\t \f t) a b`  
`a`  
### z.z. `second (tup a b)` = `b`
`second (tup a b)`  
`(\t t false) (tup a b)`  
`tup a b false`  
`(\a \b \f f a b) a b false`  
`false a b`  
`(\t \f f) a b`  
`b`  

# Numbers

## Definitions
`0` := `(\f \x x)`  
`inc`,`+` := `(\n \f \x f (n f x))` := *n* + *1*  
`add` := `(\a \b a inc b)`  
`dec` := `(\n second(n(\p tup (inc (first p)) (first p))(tup 0 0)))`
`sub` :=  `(\a \b b dec a)`
## Correctness
### z.z. `add a b` = *a* + *b*
#### IA *a* = *0*
`add 0 b`
`(\a \b a inc b) 0 b`  
`0 inc b` 
`(\f \x x) inc b`  
`b`  
#### IS *a* => *a* + *1*
`add (inc a) b`
`(\a \b a inc b) (inc a) b`  
`inc a inc b`  
`(\n \f \x f (n f x)) a inc b`  
`inc (a inc b)`  
`inc ((\a \b a inc b) a b)`  
`inc (add a b)`  
### z.z. `inc n (\p tup (inc (first p)) (first p))(tup 0 0)` = `tup (inc n) n`
#### IA `n` = `0`
`inc n (\p tup (inc (first p)) (first p))(tup 0 0)`  
`(\n \f \x f (n f x)) n (\p tup (inc (first p)) (first p))(tup 0 0)`  
`(\p tup (inc (first p)) (first p)) (0 (\p tup (inc (first p)) (first p)) (tup 0 0))`  
`(\p tup (inc (first p)) (first p)) ((\f \x x) (\p tup (inc (first p)) (first p)) (tup 0 0))`  
`(\p tup (inc (first p)) (first p)) (tup 0 0)`
`tup (inc (first (tup 0 0))) (first (tup 0 0))`
`tup (inc 0) 0`
#### IS `inc n` => `inc (inc n)`
`inc (inc n) (\p tup (inc (first p)) (first p)) (tup 0 0)`  
`(\n \f \x f (n f x)) (inc n) (\p tup (inc (first p)) (first p)) (tup 0 0)`  
`(\p tup (inc (first p)) (first p)) (inc n (\p tup (inc (first p)) (first p)) (tup 0 0))`  
`(\p tup (inc (first p)) (first p)) (tup (inc n) n)`  
`tup (inc (first (tup (inc n) n))) (first (tup (inc n) n))`  
`tup (inc (inc n)) (inc n)`  
### z.z. `dec (inc n)` = `n`
`dec (inc n)`  
`(\n second(n(\p tup (inc (first p)) (first p))(tup 0 0))) (inc n)`  
`second(inc n(\p tup (inc (first p)) (first p))(tup 0 0))`  
`second(tup (inc n) n)`  
`n`  
### z.z. `dec 0` = `0`
`dec 0`  
`(\n second(n(\p tup (inc (first p)) (first p))(tup 0 0))) 0`  
`second(0(\p tup (inc (first p)) (first p))(tup 0 0))`  
`second(tup 0 0)`  
`0`  
### z.z. `sub (add a b) b` = `a`
#### IA `b` = `0`
`sub (add a 0) 0`  
`sub a 0`  
`(\a \b b dec a) a 0`  
`0 dec a`  
`a`  
### IS `b` => `inc b`
`sub (add a (inc b)) (inc b)`  
`(\a \b b dec a) (add a (inc b)) (inc b)`  
`inc b dec (add a (inc b))`  
`(\n \f \x f (n f x)) b dec (add a (inc b))`  
`dec (b dec (add a (inc b)))`  
`dec ((\a \b b dec a) (add a (inc b)) b)`  
`dec (sub (add a (inc b)) b)`   
`dec (sub (inc (add a b)) b)`  
`dec (sub (add (inc a) b) b)`  
`dec (inc a)`   
`a`  

### z.z. `sub a (add a b)` = `0`
#### IA `b` = `0` 
`sub a (add a 0)`  
`sub a a`  
`sub (add 0 a) a`  
`0`  
#### IS `b` => `inc b`
`sub a (add a (inc b))`  
`sub a (inc (add a b))`  
`(\a \b b dec a) a (inc (add a b))`  
`inc (add a b) dec a`  
`(\n \f \x f (n f x)) (add a b) dec a`  
`dec ((add a b) dec a)`  
`dec ((\a \b b dec a) a (add a b))`  
`dec (sub a (add a b))`  
`dec 0`  
`0`  
