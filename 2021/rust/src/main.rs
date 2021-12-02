use anyhow::Result;

mod day1;
mod day2;
mod input;

fn main() -> Result<()> {
    env_logger::init();

    let args = std::env::args().collect::<Vec<_>>();
    let cmd = args.get(1);

    match cmd.map(|cmd| cmd as &str) {
        Some("day1") => {
            day1::part1()?;
            day1::part2()?;
        }
        Some("day2") => {
            day2::part1()?;
            day2::part2()?;
        }
        _ => return Err(anyhow::anyhow!("unknown command")),
    }

    Ok(())
}
