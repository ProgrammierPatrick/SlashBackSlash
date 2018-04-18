module Interpreter (EvalTree, toEvalTree, runStep, isDone, printResult) where

import Control.Monad.State

import Parser

type Env = [(String, EvalTree)]
data EvalTree = EVar Env String
              | EApp Env EvalTree EvalTree
              | EAbs Env String EvalTree

instance Show EvalTree where
  show t = showTree 0 t where
    pad 0                       = ""
    pad i                       = "  " ++ pad (i-1)
    showEnv i _ = "[]"
--    showEnv  i []               = "[]"
--    showEnv  i (e:env)          = "[\n" ++ showEnv' (i+2) (e:env)
--    showEnv' i []               = pad (i-1) ++ "]\n" ++ tail (pad (i-1))
--    showEnv' i ((s,t):env)      = pad i ++ "'" ++ s ++ "' =>\n" ++ showTree (i+1) t ++ "\n" ++ showEnv' i env
    showTree i (EVar env str)   = pad i ++ "eVAR " ++ showEnv i env ++ " '" ++ str ++ "'"
    showTree i (EApp env t1 t2) = pad i ++ "eAPP " ++ showEnv i env ++ "\n" ++ showTree (i+1) t1 ++ "\n" ++ showTree (i+1) t2
    showTree i (EAbs env str t) = pad i ++ "eABS " ++ showEnv i env ++ " '" ++ str ++ "'\n" ++ showTree (i+1) t


toEvalTree :: Env -> AST -> EvalTree
toEvalTree env ast = case ast of
  (Var str)           -> EVar env str
  (Let str ast1 ast2) -> toEvalTree ((str,toEvalTree env ast1) : env) ast2
  (App ast1 ast2)     -> EApp env (toEvalTree env ast1) (toEvalTree env ast2)
  (Abs str ast)       -> EAbs env str (toEvalTree env ast)
  Empty               -> error "Empty should not be needed anymore"

isInEnv :: Env -> String -> Bool
isInEnv []          str = False
isInEnv ((s,t):env) str = if s == str then True else isInEnv env str

getEnv :: Env -> String -> EvalTree
getEnv []          str = error("Variable " ++ str ++ " in not in environment.")
getEnv ((s,t):env) str = if s == str then t else getEnv env str

pushEnv :: Env -> EvalTree -> EvalTree
pushEnv env (EVar e str)   = EVar (env ++ e) str
pushEnv env (EApp e t1 t2) = EApp (env ++ e) (pushEnv env t1) (pushEnv env t2) 
pushEnv env (EAbs e str t) = EAbs (env ++ e) str (pushEnv env t)

runStep :: EvalTree -> EvalTree
runStep (EVar env str)   = getEnv env str
runStep (EApp env t1 t2) = case t1 of
                             EAbs e2 str t -> pushEnv ((str, t2):e2) t
                             _             -> EApp env (runStep t1) t2
runStep (EAbs env str t) = EAbs env str t -- do nothing

isDone (EAbs env str t) = True
isDone _                = False

printResult :: EvalTree -> String
-- printResult (EVar e str) = if isInEnv e str then printResult (getEnv e str) else str
printResult (EVar e str) = str
printResult (EApp e t1 t2) = "(" ++ printResult t1 ++ " " ++ printResult t2 ++ ")"
printResult (EAbs e str t) = "(\\" ++ str ++ " " ++ printResult t ++ ")"
