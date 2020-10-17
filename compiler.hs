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

import Control.Concurrent (threadDelay)

import Lexer
import Parser
import Interpreter

showState :: EvalTree -> IO ()
showState = putStrLn . printResult

interpret :: EvalTree -> IO ()
interpret t = do showState t
                 putStrLn (show t)
                 threadDelay 500 -- ms
                 if (not (isDone t))
                 then putStrLn "\n-- NextStep --" >> interpret (runStep t)
                 else putStrLn ("-- done. --\n" ++ printResult t)

main = do source <- getContents
          let tokens = lexer source
          putStrLn ("Lexer:\n" ++ concat (map show tokens))
          putStrLn ""

          let ast = parser tokens
          putStrLn ("Parser:\n" ++ show ast)
          putStrLn ""

          putStrLn "-- Initial State --"
          interpret (toEvalTree [] ast)

