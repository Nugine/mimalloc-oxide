use std::process::ExitCode;

use chumsky::Parser as _;

fn main() -> ExitCode {
    let src_path = std::env::args().nth(1).unwrap();
    let src = std::fs::read_to_string(src_path).unwrap();

    let lexer = transpile::lexer::lexer();
    match lexer.parse(&src).into_result() {
        Ok(tokens) => {
            for token in &tokens {
                println!("{:?}", token);
            }

            ExitCode::SUCCESS
        }
        Err(errs) => {
            for err in errs {
                eprintln!("{:?}", err);
                let range = err.span().into_range();
                eprintln!("---");
                eprintln!("{:?}", &src[range.clone()]);

                eprintln!("---");
                let rng = range.start - 10..range.end + 10;
                eprintln!("{}", &src[rng]);

                eprintln!("---");
                let rng = range.start - 100..range.end + 100;
                eprintln!("{}", &src[rng]);

                eprintln!("---");
            }
            ExitCode::FAILURE
        }
    }
}
