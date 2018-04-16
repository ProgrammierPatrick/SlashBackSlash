module Lexer (Token(..), lexer) where

-- CAPS: Tokens
data Token = VAR String | LPAR | RPAR | SLASH | BSLASH | EOF deriving Eq
instance Show Token where
    show (VAR str) = "\'" ++ str ++ "\' "
    show LPAR = "( "
    show RPAR = ") "
    show SLASH = "/ "
    show BSLASH = "\\ "
    show EOF = "#"

toToken :: Char -> Token
toToken '(' = LPAR
toToken ')' = RPAR
toToken '/' = SLASH
toToken '\\' = BSLASH
toToken c = VAR [c]

isWhitespace :: Char -> Bool
isWhitespace ' ' = True
isWhitespace '\n' = True
isWhitespace '\r' = True
isWhitespace '\t' = True
isWhitespace c = False

isVariableChar :: Char -> Bool
isVariableChar c = if isWhitespace c then False else
                  if c == '(' then False else
                  if c == ')' then False else
                  if c == '/' then False else
                  if c == '\\' then False else
                  True

lexer :: String -> [Token]
lexer []      = [EOF]
lexer (c:str) = if isWhitespace c then lexer str else
                if isVariableChar c then
                    (VAR (takeWhile isVariableChar (c:str))) : lexer (dropWhile isVariableChar (c:str)) else
                (toToken c) : lexer str
