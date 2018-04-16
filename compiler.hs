{-
Grammar:

<PROGRAM>  ::= <EXPR>
<VAL>      ::= <VAR> | ( <EXPR> )
<EXPR>     ::= <LET> | <APP> | <ABS>
<LET>      ::= / <VAR> <VAL> <EXPR>
<APP>      ::= <VAL> | <VAL> <APP>
<ABS>      ::= \ <VAR> <EXPR>
<VAR>      ::= <Word without / \ ( )>

            FIRST     FOLLOW
<PROGRAM> | / \ V ( | #
<VAL>     | V (     | # / \ V (
<EXPR>    | / \ V ( | ) #
<LET>     | /       | / \ V (
<APP>     | # V (   | ) #
<ABS>     | \       | ) #
<VAR>     | V       | # / \ V (



-}

import Lexer
import Parser

main = do source <- getContents
          let tokens = lexer source
          putStrLn (concat (map show tokens))
          let ast = parser tokens
          putStrLn (show ast)


