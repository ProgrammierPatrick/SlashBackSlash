use std::iter::Peekable;
use log::{debug, error};
use crate::lexer::Token;
use logos::Logos;


#[derive(Debug)]
pub enum AST<'a> {
    Val{
        name: &'a str,
    },
    Block{
        imports: Vec<&'a str>,
        lets: Vec<(&'a str, AST<'a>)>,
        args: Vec<&'a str>,
        exprs: Vec<AST<'a>>,
    }
}

fn parse_block<'a, 'b, I>(mut tokens: &'b mut Peekable<I>)->(Option<AST<'a>>, &'b mut Peekable<I>)
where I: Iterator<Item = Token<'a>>
{
    let mut imports = Vec::default();
    loop{
        let (import, tokens_) = parse_import(tokens);
        tokens = tokens_;
        let Some(import) = import else {break};
        imports.push(import);
    }
    let mut lets = Vec::default();
    loop{
        let (let_, tokens_) = parse_let(tokens);
        tokens = tokens_;
        let Some(let_) = let_ else {break};
        lets.push(let_);
    }
    let mut args = Vec::default();
    loop{
        let (arg, tokens_) = parse_arg(tokens);
        tokens = tokens_;
        let Some(arg) = arg else {break};
        args.push(arg);
    }
    let mut exprs = Vec::default();
    loop{
        let (expr, tokens_) = parse_expr(tokens);
        tokens = tokens_;
        let Some(expr) = expr else {break};
        exprs.push(expr);
    }
    (Some(AST::Block{imports, lets, args, exprs}), tokens)
}

fn parse_expr<'a, 'b, I>(tokens: &'b mut Peekable<I>)->(Option<AST<'a>>, &'b mut Peekable<I>)
where I: Iterator<Item = Token<'a>>
{
    let (res, tokens) = parse_val(tokens);
    if let Some(val) = res {
        return (Some(val), tokens)
    };
    let Some(token) = tokens.next_if(|token| {
        debug!("Found token {token:?}");
        token.is_open()
    })  else {
        debug!("token isn't import");
        return (None, tokens);
    };
    let (res, tokens) = parse_block(tokens);
    let Some(Token::Close) = tokens.next() else {
        panic!("expect )")
    };
    (res, tokens)
}

fn parse_val<'a, 'b, I>(tokens: &'b mut Peekable<I>)->(Option<AST<'a>>, &'b mut Peekable<I>)
where I: Iterator<Item = Token<'a>>
{
    debug!("Parse Val");
    let token = tokens.next_if(|token| {
        debug!("Found token {token:?}");
        token.is_lable()
    });
    let Some(Token::Lable(name)) = token else {
        debug!("{token:?} isn't a val");
        return (None, tokens);
    };
    debug!("found var {name}");
    (Some(AST::Val{name}), tokens)
}

fn parse_import<'a, 'b, I>(tokens: &'b mut Peekable<I>)->(Option<&'a str>, &'b mut Peekable<I>)
where I: Iterator<Item = Token<'a>>
{
    debug!("Parse Import");
    let Some(_) = tokens.next_if(|token| {
        debug!("Found token {token:?}");
        token.is_import()
    }) else {
        debug!("token isn't import");
        return (None, tokens);
    };
    let token = tokens.next();
    let Some(Token::Lable(import)) = token else {
        error!("Didn't expect {token:?}");
        panic!("unexpected token");
    };
    debug!("found import {import}");
    (Some(import), tokens)
}

fn parse_let<'a, 'b, I>(tokens: &'b mut Peekable<I>)->(Option<(&'a str, AST<'a>)>, &'b mut Peekable<I>)
where I: Iterator<Item = Token<'a>>
{
    debug!("Parse Let");
    let token = tokens.next_if(|token| {
        debug!("Found token {token:?}");
        token.is_def()
    });
    let Some(Token::Def(name)) = token else {
        debug!("{token:?} isn't a let");
        return (None, tokens);
    };
    let (res, tokens) = parse_expr(tokens);
    let Some(val) = res else {
        error!("Didn't expect {res:?}");
        panic!("Wrong Expression");
    };
    (Some((name, val)), tokens)
}

fn parse_arg<'a, 'b, I>(tokens: &'b mut Peekable<I>)->(Option<&'a str>, &'b mut Peekable<I>)
where I: Iterator<Item = Token<'a>>
{
    debug!("Parse Let");
    let token = tokens.next_if(|token| {
        debug!("Found token {token:?}");
        token.is_arg()
    });
    let Some(Token::Arg(name)) = token else {
        debug!("{token:?} isn't a let");
        return (None, tokens);
    };
    (Some(name), tokens)
}

pub fn parse<'a, I>(tokens: &mut I)->Box<AST<'a>>
where I: Iterator<Item = Token<'a>> {
    todo!()
}
