use anyhow::Result;

use crate::input;

pub fn test_input() -> &'static str {
    "199
200
208
210
200
207
240
269
260
263"
}

pub fn parse(input: &str) -> Vec<i32> {
    input
        .trim()
        .lines()
        .filter_map(|line| line.parse().ok())
        .collect()
}

pub fn part1(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(1)?
    };

    let mut increases = 0;
    let mut last_measurement = None;
    for measurement in parse(&input) {
        if let Some(last_measurement) = last_measurement {
            if measurement > last_measurement {
                increases += 1;
            }
        }
        last_measurement = Some(measurement);
    }

    println!("part 1: {} increases", increases);

    Ok(())
}

pub fn part2(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(1)?
    };

    let measurements = parse(&input);
    let measurments2 = measurements.iter().skip(1).collect::<Vec<_>>();

    let mut increases = 0;
    for (a, b) in (&measurements).windows(3).zip((&measurments2).windows(3)) {
        let a: i32 = a.iter().sum();
        let b: i32 = b.iter().copied().sum();
        if b > a {
            increases += 1;
        }
    }

    println!("part 2: {:?}", increases);

    Ok(())
}
