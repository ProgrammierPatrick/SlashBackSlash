#[macro_use]
extern crate log;
mod lexer;
mod parser;
use lexer::Token;
use parser::parse;
use logos::Logos;

fn main() {
    env_logger::init();
    let x = "import er test_case x /foo (\\bar \\baz baz bar) \\#asd asd \\ er e\n/x";
    let mut tokens = Token::lexer(x).peekable();
    let ast = parse(&mut tokens);
    println!("{ast:?}");
}
