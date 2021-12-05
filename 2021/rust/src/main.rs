use anyhow::Result;

mod day1;
mod day2;
mod day3;
mod day4;
mod day5;
mod input;

fn main() -> Result<()> {
    env_logger::init();

    let args = std::env::args().collect::<Vec<_>>();
    let cmd = args.get(1).map(|it| it as &str);
    let testing = args.get(2).map(|it| it == "testing").unwrap_or(false);

    match (cmd, testing) {
        (Some("day1"), testing) => {
            day1::part1(testing)?;
            day1::part2(testing)?;
        }
        (Some("day2"), testing) => {
            day2::part1(testing)?;
            day2::part2(testing)?;
        }
        (Some("day3"), testing) => {
            day3::part1(testing)?;
            day3::part2(testing)?;
        }
        (Some("day4"), testing) => {
            day4::part1(testing)?;
            day4::part2(testing)?;
        }
        (Some("day5"), testing) => {
            day5::part1(testing)?;
            day5::part2(testing)?;
        }
        _ => return Err(anyhow::anyhow!("unknown command")),
    }

    Ok(())
}
