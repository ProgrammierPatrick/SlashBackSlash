use logos::Logos;

#[derive(Logos, Debug, PartialEq)]
pub enum Token<'a> {
    #[token("import")]
    Import,
    #[token("test_case")]
    TestCase,
    #[regex(r"[^\#\\/\s\(\)]+", |lex| lex.slice())]
    Lable(&'a str),
    #[regex(r"\\[^\#\\/\s\(\)]+", |lex| &(lex.slice()[1..]))]
    Arg(&'a str),
    #[regex(r"/[^\#\\/\s\(\)]+", |lex| &(lex.slice()[1..]))]
    Def(&'a str),
    #[token("(")]
    Open,
    #[token(")")]
    Close,
    #[error]
    #[regex(r"\s*(\#.*)?", logos::skip)]
    Error,
}

impl<'a> Token<'a>{
    pub fn is_import(&self) -> bool {
        match self {
            Token::Import => true,
            _ => false,
        }
    }
    pub fn is_test_case(&self) -> bool {
        match self {
            Token::TestCase => true,
            _ => false,
        }
    }
    pub fn is_lable(&self) -> bool {
        match self {
            Token::Lable(_) => true,
            _ => false,
        }
    }
    pub fn is_arg(&self) -> bool {
        match self {
            Token::Arg(_) => true,
            _ => false,
        }
    }
    pub fn is_def(&self) -> bool {
        match self {
            Token::Def(_) => true,
            _ => false,
        }
    }
    pub fn is_open(&self) -> bool {
        match self {
            Token::Open => true,
            _ => false,
        }
    }
    pub fn is_close(&self) -> bool {
        match self {
            Token::Close => true,
            _ => false,
        }
    }
    
    pub fn is_error(&self) -> bool {
        match self {
            Token::Error => true,
            _ => false,
        }
    }
}
