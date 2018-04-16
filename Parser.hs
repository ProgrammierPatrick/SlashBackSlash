module Parser (AST(..), parser) where

import Lexer
import Control.Monad.State

-- Uppercase: AST nodes
data AST = Var String 
         | Let String AST AST
         | App AST    AST
         | Abs String AST
         | Empty

instance Show AST where
    show ast = showTree 0 ast where
        padding i = concat (replicate i "   ")
        showTree i (Var token)           = padding i ++ show (VAR token)
        showTree i (Let token ast1 ast2) = padding i ++ "LET " ++ show (VAR token) ++ "\n" ++ showTree (i+1) ast1 ++ "\n" ++ showTree (i+1) ast2
        showTree i (App ast1  ast2)      = padding i ++ "APP\n" ++ showTree (i+1) ast1 ++ "\n" ++ showTree (i+1) ast2
        showTree i (Abs token ast)       = padding i ++ "ABS " ++ show (VAR token) ++ "\n" ++ showTree (i+1) ast

parser :: [Token] -> AST
parser tokens = evalState (parseExpr (head tokens)) tokens
-- parser list = Let "abc" (App (Abs "def" (Var "ghi")) (Var "jkl"))

nextToken :: State [Token] Token
nextToken = do (t:ts) <- get
               if t == EOF then return EOF
               else put ts >> return (head ts)

currentToken :: State [Token] Token
currentToken = do t <- get
                  return (head t)

--------------- EXPR -----------
parseExpr :: Token -> State [Token] AST
parseExpr t = case t of
    (VAR str) -> parseApp t
    LPAR      -> parseApp t
    RPAR      -> error "Got ) at start of <EXPR>, should be / \\ ( or Var"
    SLASH     -> parseLet t
    BSLASH    -> parseAbs t
    EOF       -> error "Got EOF in <EXPR>"

---------------- LET ---------------
parseLet :: Token -> State [Token] AST
parseLet SLASH = do var  <- (nextToken >>= parseVar)
                    val  <- (currentToken >>= parseVal)
                    t <- currentToken
                    if t == EOF then error "Missing Expression after <LET>-Statemet."
                    else do expr <- parseExpr t
                            return (Let var val expr)
parseLet t = error ("Expected / (for <LET>), but got " ++ show t)

----------------- VAR -----------------
parseVar :: Token -> State [Token] String
parseVar (VAR str) = nextToken >> return str
parseVar t         = error ("Expector Var (for <VAR>), but got " ++ show t)

------------------- VAL ---------------------
parseVal :: Token -> State [Token] AST
parseVal (VAR str) = parseVar (VAR str) >>= (\x -> return (Var x))
parseVal LPAR      = do ast <- (nextToken >>= parseExpr)
                        t <- currentToken
                        if t /= RPAR then error ("Exepected ) (for <VAL>), but got " ++ show t)
                          else (nextToken >> return ast)
parseVal t         = error ("Expected Var or ( (for <VAL>), but got " ++ show t)

-------------------- APP ------------------------
isAppStart :: Token -> Bool
isAppStart (VAR str) = True
isAppStart LPAR      = True
isAppStart t         = False

parseAppAST :: AST -> Token -> State [Token] AST
parseAppAST ast t = if isAppStart t
                    then do val <- parseVal t
                            currentToken >>= parseAppAST (App ast val)
                    else return ast

parseApp :: Token -> State [Token] AST
parseApp t = do val <- parseVal t
                t <- currentToken
                if isAppStart t 
                  then do val2 <- parseVal t
                          currentToken >>= parseAppAST (App val val2)
                  else return val

----------------------- ABS ---------------------------
parseAbs :: Token -> State [Token] AST
parseAbs BSLASH = do var <- (nextToken >>= parseVar)
                     expr <- (currentToken >>= parseExpr)
                     return (Abs var expr)
parseAbs t = error ("Expected \\ (for <ABS>), but got " ++ show t)

