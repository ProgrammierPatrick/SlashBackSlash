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
