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
`sub` := `(\a \b b dec a)`  
`mul` := `(\a \b \f a (b f))`  
`is0` := `(\n n (\x false) true)`  
`ge` := `(\n \m is0 (sub m n))`  
`le` := `(\n \m ge m n)`  
`gt` := `(\n \m not (le n m))`  
`lt` := `(\n \m gt m n)`  
`eq` := `eq (\n \m and (\ge n m) (\le n m))`  
`even` := `(\n n not true)`  
`odd` := `(\n n not false)`
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
### z.z. `add a b f x` = `a f (b f x)`
#### IA `a` = `0`
`add 0 b f x`  
`(\a \b a inc b) 0 b f x`  
`0 inc b f x`  
`b f x`  
`0 f (b f x)`  
#### IS `a` => `inc a`
`add (inc a) b f x`  
`inc (add a b) f x`  
`inc ((\a \b a inc b) a b) f x`  
`inc (a inc b) f x`  
`(\n \f \x f (n f x)) (a inc b) f x`  
`f ((a inc b) f x)`  
`f (add a b f x)`  
`f (a f (b f x))`  
`(\n \f \x f (n f x)) a f (b f x)`  
`inc a f (b f x)`  

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

### z.z. `mul a b` = *a* * *b*
#### IA *a* = *0*
`mul 0 b f x`  
`(\a \b \f a (b f)) 0 b f x`  
`0 (b f) x`  
`(\f \x x) (b f) x`  
`x`  
`(\f \x x) f x`  
`0 f x`  
#### IA *a* => *a* + *1*
`mul (inc a) b f x`  
`(\a \b \f a (b f)) (inc a) b f x`  
`inc a (b f) x`  
`(\n \f \x f (n f x)) a (b f) x`  
`b f (a (b f) x)`  
`b f ((\a \b \f a (b f)) a b f x)`  
`b f (mul a b f x)`  
`add b (mul a b) f x`  

### z.z. `is0 0` = `true`
`is0 0`  
`(\n n (\x false) true) 0`  
`0 (\x false) true`  
`true`  

### z.z. `is0 (inc n)` = `false`
`is0 (inc n)`  
`(\n n (\x false) true) (inc n)`  
`inc n (\x false) true`  
`(\n \f \x f (n f x)) n (\x false) true`  
`(\x false) (n (\x false) true)`   
`false`  

### z.z. `ge (add a b) a` = `true`
`ge (add a b) a`  
`(\n \m is0 (sub m n)) (add a b) a`  
`is0 (sub a (add a b))`  
`is0 0`  
`true`
### z.z. `ge a (add a (inc b))` = `false`
`ge a (add a (inc b))`  
`(\n \m is0 (sub m n)) a (add a (inc b))`  
`is0 (sub (add a (inc b)) a)`  
`is0 (inc b)`  
`false`  
### `le`,`gt`,`lt`
simple variants of `ge`
### z.z. `eq n n` = `true`
`eq n n`  
`(\n \m and (\ge n m) (\le n m)) n n`  
`and (\ge n n) (\le n n)`  
`and true true`  
`true`  

### z.z. `eq n (add n (inc m))` = `false`  
`eq n (add n (inc m))`  
`(\n \m and (\ge n m) (\le n m)) n (add n (inc m))`  
`and (\ge n (add n (inc m))) (\le n (add n (inc m)))`  
`and false true`  
`false`  

### z.z. `eq (add m (inc n)) m` = `false`  
`eq (add m (inc n)) m`  
`(\n \m and (\ge n m) (\le n m)) (add m (inc n)) m`  
`and (\ge (add m (inc n)) m) (\le (add m (inc n)) m)`  
`and true fasle`  
`fasle`  

### z.z. `even (mul 2 n)` = `true` 
#### `n` = `0`
`even (mul 2 0)`  
`even 0`  
`(\n n not true) 0`  
`0 not true`  
`true`
#### `n` => `inc n`  
`even (mul 2 (inc n))`  
`even (add 2 (mul 2 n))`   
`(\n n not true) (add 2 (mul 2 n))`  
`add 2 (mul 2 n) not true`  
`2 not (mul 2 n not true)`  
`2 not ((\n n not true)(mul 2 n))`  
`2 not (even (mul 2 n))`  
`2 not true`  
`1 not false`  
`true`  
### z.z. `even (inc (mul 2 n))` = `false` 
`even (inc (mul 2 n))`   
`(\n n not true) (inc (mul 2 n))`   
`inc (mul 2 n) not true `  
`(\n \f \x f (n f x)) (mul 2 n) not true`  
`not (mul 2 n not true)`  
`not ((\n n not true) (mul 2 n))`  
`not true`   
`false`  
### `odd`
analog
