use anyhow::Result;

use crate::input;

#[derive(Debug)]
enum SubmarineCommand {
    Forward(i64),
    Up(i64),
    Down(i64),
}

impl SubmarineCommand {
    fn parse(input: &str) -> Vec<Self> {
        let mut results = Vec::new();

        for line in input.lines() {
            let line = line.trim();
            if line.is_empty() {
                continue;
            }
            if let Some((cmd, val)) = line.split_once(' ') {
                let cmd = match (cmd, val.parse()) {
                    ("forward", Ok(n)) => Self::Forward(n),
                    ("up", Ok(n)) => Self::Up(n),
                    ("down", Ok(n)) => Self::Down(n),
                    _ => continue,
                };
                results.push(cmd);
            }
        }
        results
    }
}

#[derive(Debug, Default)]
struct SubmarinePosition {
    horizontal: i64,
    depth: i64,
    aim: i64,
}

impl SubmarinePosition {
    fn new() -> Self {
        Self::default()
    }

    fn apply_part1(&mut self, cmd: &SubmarineCommand) {
        match cmd {
            SubmarineCommand::Forward(n) => self.horizontal += n,
            SubmarineCommand::Up(n) => self.depth -= n,
            SubmarineCommand::Down(n) => self.depth += n,
        }
    }

    fn apply_part2(&mut self, cmd: &SubmarineCommand) {
        match cmd {
            SubmarineCommand::Forward(n) => {
                self.horizontal += n;
                self.depth += self.aim * n;
            }
            SubmarineCommand::Up(n) => {
                self.aim -= n;
            }
            SubmarineCommand::Down(n) => {
                self.aim += n;
            }
        }
    }
}

fn test_input() -> &'static str {
    "forward 5
down 5
forward 8
up 3
down 8
forward 2"
}

pub fn part1(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(2)?
    };

    let commands = SubmarineCommand::parse(&input);

    let mut pos = SubmarinePosition::new();
    for cmd in commands {
        pos.apply_part1(&cmd);
    }

    println!("part 1: {}", pos.horizontal * pos.depth);

    Ok(())
}

pub fn part2(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(2)?
    };

    let commands = SubmarineCommand::parse(&input);

    let mut pos = SubmarinePosition::new();
    for cmd in commands {
        pos.apply_part2(&cmd);
    }

    println!("part 2: {}", pos.horizontal * pos.depth);

    Ok(())
}
