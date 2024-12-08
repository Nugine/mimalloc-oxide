use crate::lexer::Ident;
use crate::lexer::Punct;
use crate::lexer::Token;

use chumsky::input::ValueInput;
use chumsky::prelude::*;

type Extra<'s> = extra::Err<Rich<'s, Token<'s>, SimpleSpan>>;

pub fn parser<'s, I>() -> impl Parser<'s, I, Vec<Item<'s>>, Extra<'s>> + Clone
where
    I: ValueInput<'s, Token = Token<'s>, Span = SimpleSpan>,
{
    recursive(|items| {
        let ifdef = IfDef::parser(items).map(Item::IfDef);
        let define = Define::parser::<I>().map(Item::Define);
        let newline = just::<_, I, Extra<'s>>(Token::Newline).to(Item::Newline);

        let item = ifdef.or(define).or(newline);

        // let item = todo::<I, Item<'s>, Extra<'s>>();
        item.repeated().collect()
    })
}

#[derive(Debug, Clone)]
pub enum Item<'s> {
    IfDef(IfDef<'s>),
    Define(Define<'s>),
    Newline,
}

// #[derive(Debug, Clone)]
// pub struct Function<'s> {
//     pub decl: Declare<'s>,
//     pub args: Vec<Declare<'s>>,
//     pub body: Option<Vec<Item<'s>>>,
// }

// #[derive(Debug, Clone)]
// pub struct Declare<'s> {
//     pub name: Ident<'s>,
// }

#[derive(Debug, Clone)]
pub struct IfDef<'s> {
    pub negated: bool,
    pub ident: Ident<'s>,
    pub if_: Vec<Item<'s>>,
    pub else_: Vec<Item<'s>>,
}

impl<'a> IfDef<'a> {
    pub fn parser<'s, I, P>(items: P) -> impl Parser<'s, I, Self, Extra<'s>> + Clone
    where
        's: 'a,
        I: ValueInput<'s, Token = Token<'s>, Span = SimpleSpan>,
        P: Parser<'s, I, Vec<Item<'s>>, Extra<'s>> + Clone,
    {
        let ident = select! { Token::Ident(ident) => ident };

        let ifdef = directive("ifdef")
            .ignore_then(ident)
            .then(items.clone())
            .then(directive("else").ignore_then(items.clone()).or_not())
            .then_ignore(directive("endif"))
            .map(|((ident, if_), else_)| IfDef {
                negated: false,
                ident,
                if_,
                else_: else_.unwrap_or_default(),
            });

        let ifndef = directive("ifndef")
            .ignore_then(ident)
            .then(items.clone())
            .then(directive("else").ignore_then(items).or_not())
            .then_ignore(directive("endif"))
            .map(|((ident, if_), else_)| IfDef {
                negated: true,
                ident,
                if_,
                else_: else_.unwrap_or_default(),
            });

        ifdef.or(ifndef)
    }
}

#[derive(Debug, Clone)]
pub struct Define<'s> {
    pub ident: Ident<'s>,
    pub tokens: Vec<Token<'s>>,
}

impl<'a> Define<'a> {
    pub fn parser<'s, I>() -> impl Parser<'s, I, Self, Extra<'s>> + Clone
    where
        's: 'a,
        I: ValueInput<'s, Token = Token<'s>, Span = SimpleSpan>,
    {
        let ident = select! { Token::Ident(ident) => ident };
        directive("define")
            .ignore_then(ident)
            .then(
                any()
                    .and_is(just(Token::Newline).not())
                    .repeated()
                    .collect(),
            )
            .map(|(ident, tokens)| Self { ident, tokens })
    }
}

fn directive<'s, I>(name: &'s str) -> impl Parser<'s, I, Token<'s>, Extra<'s>> + Clone
where
    I: ValueInput<'s, Token = Token<'s>, Span = SimpleSpan>,
{
    just(Token::Punct(Punct::Hash)).ignore_then(just(Token::ident(name)))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_ifdef() {
        let src = r#"
            #ifndef _DEFAULT_SOURCE
            #define _DEFAULT_SOURCE 
            #endif
        "#;

        let lexer = crate::lexer::lexer();
        let tokens = lexer.parse(src).into_result().unwrap();

        let parser = parser();
        let items = parser.parse(tokens.as_slice()).into_result().unwrap();
        dbg!(items);
    }
}
