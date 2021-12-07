use crate::input;
use anyhow::Result;
use itertools::Itertools;

fn test_input() -> &'static str {
    "3,4,3,1,2"
}

#[derive(Debug)]
struct LanternFish {
    age: u64,
}

impl LanternFish {
    fn new(n: u64) -> Self {
        Self { age: n }
    }

    fn day_passed(&mut self, new_fish: &mut Vec<LanternFish>) {
        if self.age == 0 {
            self.age = 6;
            new_fish.push(LanternFish::new(8))
        } else {
            self.age -= 1;
        }
    }
}

#[derive(Debug, Default)]
struct Fishs {
    age: u64,
    fishs: Vec<LanternFish>,
}

impl Fishs {
    fn len(&self) -> usize {
        self.fishs.len()
    }

    fn split(self, n: usize) -> (Self, Self) {
        let Self { age, mut fishs } = self;
        let other_fishs = fishs.split_off(n);
        (
            Self { age, fishs },
            Self {
                age,
                fishs: other_fishs,
            },
        )
    }
}

impl std::fmt::Display for Fishs {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "After {:>4} days: {}",
            self.age,
            self.fishs.iter().map(|f| f.age).join(",")
        )
    }
}

pub fn solve_attempt1(input: &str, days: usize) -> Result<()> {
    let mut fishs = Fishs {
        age: 0,
        fishs: input
            .split(',')
            .filter_map(|digit| {
                let digit = digit.trim();
                if digit.is_empty() {
                    None
                } else {
                    digit.parse().ok()
                }
            })
            .map(LanternFish::new)
            .collect(),
    };

    let mut growth = Vec::new();
    for day in 0..days {
        println!("day {}: {} {}", day, fishs.fishs.len(), fishs);
        // println!("{}", fishs);
        growth.push(fishs.fishs.len());
        let mut new_fishs = Vec::new();
        for fish in &mut fishs.fishs {
            fish.day_passed(&mut new_fishs);
        }
        fishs.fishs.extend(new_fishs);
        fishs.age += 1;
    }

    println!("{}", fishs.fishs.len());

    let plotter = poloto::plot("growth", "day", "fish")
        .line(
            "n",
            growth
                .iter()
                .enumerate()
                .map(|(i, &n)| [i as i128, n as i128]),
        )
        .ymarker(0)
        .move_into();

    let plot = poloto::disp(|a| poloto::simple_theme(a, plotter)).to_string();
    let path = std::env::home_dir().unwrap().join("Desktop/fishs.svg");
    std::fs::write(path, plot)?;

    Ok(())
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#[derive(Debug)]
struct FishPopulation<const MAXAGE: usize> {
    age: usize,
    fishs_by_age: [i64; MAXAGE],
}

impl<const MAXAGE: usize> FishPopulation<MAXAGE> {
    fn day_passed(&mut self) {
        let giving_birth = self.fishs_by_age[0];

        for i in 1..MAXAGE {
            self.fishs_by_age[i - 1] = self.fishs_by_age[i];
        }
        self.fishs_by_age[MAXAGE - 1] = 0;

        self.fishs_by_age[6] += giving_birth;
        self.fishs_by_age[8] += giving_birth;
    }

    fn fishs(&self) -> i64 {
        self.fishs_by_age.iter().sum()
    }
}

impl<const MAXAGE: usize> std::fmt::Display for FishPopulation<MAXAGE> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "After {:>4} days: ", self.age)?;
        for i in 0..MAXAGE {
            write!(f, "{} ", self.fishs_by_age[i])?;
        }
        write!(f, "({})", self.fishs())
    }
}

pub fn solve_attempt2(input: &str, days: usize) -> Result<()> {
    let mut fishs = FishPopulation {
        age: 0,
        fishs_by_age: [0; 9],
    };

    println!("{}", fishs.fishs_by_age.len());
    for digit in input.split(',') {
        let digit: usize = digit.trim().parse()?;
        fishs.fishs_by_age[digit] += 1;
    }

    for day in 0..days {
        println!("day {}: {}", day, fishs);
        fishs.day_passed();
    }

    Ok(())
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

pub fn part1(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(6)?
    };

    solve_attempt1(&input, 19)?;

    Ok(())
}

pub fn part2(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(6)?
    };

    solve_attempt2(&input, 257)?;

    Ok(())
}
