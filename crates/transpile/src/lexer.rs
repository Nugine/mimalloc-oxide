use chumsky::prelude::*;

type Extra<'s> = extra::Err<Rich<'s, char, SimpleSpan>>;

pub fn lexer<'s>() -> impl Parser<'s, &'s str, Vec<Token<'s>>, Extra<'s>> {
    Token::lexer()
        .padded_by(one_of(" \t").ignored().repeated())
        .repeated()
        .collect()
}

#[derive(Debug, Clone, PartialEq)]
pub enum Token<'s> {
    // Directive(Directive),
    Keyword(Keyword),
    Ident(Ident<'s>),
    Int(u64),
    LitStr(&'s str),
    LitChar(char),
    Punct(Punct),
    Newline,
}

impl<'a> Token<'a> {
    pub fn lexer<'s>() -> impl Parser<'s, &'s str, Token<'s>, Extra<'s>> {
        // let directive = Directive::lexer().map(Token::Directive);

        let keyword = Keyword::lexer().map(Token::Keyword);

        let punct = Punct::lexer().map(Token::Punct);

        let int = text::int::<'s, &'s str, _, Extra<'s>>(10)
            .from_str::<u64>()
            .unwrapped()
            .map(Token::Int);

        let lit_str = just('"')
            .ignore_then(none_of('"').repeated().to_slice())
            .then_ignore(just('"'))
            .map(Token::LitStr);

        let lit_char = just('\'')
            .ignore_then(none_of('\'').repeated().to_slice())
            .then_ignore(just('\''))
            .try_map(|s, span| match s {
                r"\n" => Ok('\n'),
                r"\r" => Ok('\r'),
                r"\t" => Ok('\t'),
                r"\'" => Ok('\''),
                r#"\""# => Ok('"'),
                r"\\" => Ok('\\'),
                s if s.len() == 1 => Ok(s.chars().next().unwrap()),
                _ => Err(Rich::custom(span, "invalid character literal")),
            })
            .map(Token::LitChar);

        let ident = Ident::lexer().map(Token::Ident);

        let newline = text::newline().to(Token::Newline);

        keyword
            .or(ident)
            .or(int)
            .or(lit_str)
            .or(lit_char)
            .or(punct)
            .or(newline)
    }

    pub fn ident<'s>(name: &'s str) -> Self
    where
        's: 'a,
    {
        Token::Ident(Ident { name })
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct Ident<'s> {
    name: &'s str,
}

impl<'a> Ident<'a> {
    fn lexer<'s>() -> impl Parser<'s, &'s str, Self, Extra<'s>>
    where
        's: 'a,
    {
        text::ascii::ident().map(|name| Ident { name })
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Keyword {
    Struct,
    TypeDef,
    Const,
}

impl Keyword {
    fn lexer<'s>() -> impl Parser<'s, &'s str, Self, Extra<'s>> {
        choice((
            just("struct").to(Keyword::Struct),
            just("typedef").to(Keyword::TypeDef),
            just("const").to(Keyword::Const),
        ))
    }
}

// #[derive(Debug, Clone)]
// pub enum Directive {
//     If,
//     Elif,
//     Else,
//     Endif,
//     Ifdef,
//     Ifndef,
//     Define,
//     Include,
// }

// impl Directive {
//     fn lexer<'s>() -> impl Parser<'s, &'s str, Self, Extra<'s>> {
//         just("#").ignore_then(choice((
//             just("if").to(Directive::If),
//             just("elif").to(Directive::Elif),
//             just("else").to(Directive::Else),
//             just("endif").to(Directive::Endif),
//             just("ifdef").to(Directive::Ifdef),
//             just("ifndef").to(Directive::Ifndef),
//             just("define").to(Directive::Define),
//             just("include").to(Directive::Include),
//         )))
//     }
// }

#[derive(Debug, Clone, PartialEq)]
pub enum Punct {
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Semi,
    Colon,
    Hash,
    Backslash,
    And,
    Or,
    Not,
    Xor,
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    Assign,
    Comma,
    Dot,
    Less,
    Greater,
    Tilde,
    Question,
}

impl Punct {
    fn lexer<'s>() -> impl Parser<'s, &'s str, Self, Extra<'s>> {
        choice((
            just('(').to(Punct::LParen),
            just(')').to(Punct::RParen),
            just('{').to(Punct::LBrace),
            just('}').to(Punct::RBrace),
            just('[').to(Punct::LBracket),
            just(']').to(Punct::RBracket),
            just(';').to(Punct::Semi),
            just(':').to(Punct::Colon),
            just('#').to(Punct::Hash),
            just('\\').to(Punct::Backslash),
            just('&').to(Punct::And),
            just('|').to(Punct::Or),
            just('!').to(Punct::Not),
            just('^').to(Punct::Xor),
            just('+').to(Punct::Add),
            just('-').to(Punct::Sub),
            just('*').to(Punct::Mul),
            just('/').to(Punct::Div),
            just('%').to(Punct::Rem),
            just('=').to(Punct::Assign),
            just(',').to(Punct::Comma),
            just('.').to(Punct::Dot),
            just('<').to(Punct::Less),
            just('>').to(Punct::Greater),
            just('~').to(Punct::Tilde),
            just('?').to(Punct::Question),
        ))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_lexer() {
        let lexer = lexer();
        let p = |s| lexer.parse(s).unwrap();

        dbg!(p(r#"
            #ifndef _DEFAULT_SOURCE
            #define _DEFAULT_SOURCE 
            #endif
        "#));

        dbg!(p(r#"
            #if 0
            #define A (1)
            #endif
        "#));

        dbg!(p(r#"
            #if 0
            #elif (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__)
                #define mi_decl_nodiscard __attribute__((warn_unused_result))
            #elif defined(_HAS_NODISCARD)
                #define mi_decl_nodiscard _NODISCARD
            #elif 0
            #else
                #define mi_decl_nodiscard
            #endif
        "#));
    }
}
