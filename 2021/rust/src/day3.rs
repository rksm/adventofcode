use std::ops::Not;

use crate::input;
use anyhow::Result;
use bitvec::prelude::*;

fn test_input() -> &'static str {
    "00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010"
}

struct ReportNumber {
    raw: Vec<char>,
}

impl ReportNumber {
    fn new(raw: &str) -> Self {
        Self {
            raw: raw.chars().collect(),
        }
    }

    // fn value(&self) -> u32 {
    //     u32::from_str_radix(&self.raw, 2).expect("parse number")
    // }

    fn values(&self) -> BitVec<Msb0, u8> {
        self.raw.iter().map(|c| *c != '0').collect()
    }

    fn len(&self) -> usize {
        self.raw.len()
    }
}

pub fn part1(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(3)?
    };

    let mut lines = input.lines().peekable();
    let n = lines.clone().count();
    let first_line = lines.peek().unwrap();
    let mut columns = (0..first_line.len())
        .map(|_| bitvec![0; n])
        .collect::<Vec<_>>();

    for (i, line) in lines.enumerate() {
        for (j, c) in line.chars().enumerate() {
            let col = &mut columns[j];
            col.set(i, c == '1')
        }
    }

    let gamma_rate = columns
        .into_iter()
        .map(|col| col.count_ones() > col.len() / 2)
        .collect::<BitVec<Msb0, u32>>();
    let epsilon_rate = !gamma_rate.clone();

    println!("gamma {} {}", gamma_rate, gamma_rate.clone().load::<u32>());
    println!("epsilon {} {}", epsilon_rate, epsilon_rate.load::<u32>());

    println!("{}", gamma_rate.load::<u32>() * epsilon_rate.load::<u32>());

    Ok(())
}

pub fn part2(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(3)?
    };

    let mut numbers1 = input
        .lines()
        .map(|line| {
            line.chars()
                .map(|c| c == '1')
                .collect::<BitVec<Msb0, u32>>()
        })
        .collect::<Vec<_>>();
    let mut numbers2 = numbers1.clone();

    let n = numbers1[0].len();

    for i in 0..n {
        let (ones, zeros): (Vec<_>, Vec<_>) = numbers1.into_iter().partition(|n| n[i]);
        numbers1 = if ones.len() >= zeros.len() {
            ones
        } else {
            zeros
        };
        if numbers1.len() == 1 {
            break;
        };
    }

    for i in 0..n {
        let (ones, zeros): (Vec<_>, Vec<_>) = numbers2.into_iter().partition(|n| n[i]);
        numbers2 = if zeros.len() <= ones.len() {
            zeros
        } else {
            ones
        };

        // println!("{}", i);
        // for n in numbers2.iter() {
        //     println!("{}", n);
        // }

        if numbers2.len() == 1 {
            break;
        };
    }

    assert_eq!(numbers1.len(), 1);
    assert_eq!(numbers2.len(), 1);

    let n1 = &numbers1[0];
    let n2 = &numbers2[0];

    println!("{}", n1.load::<u32>() * n2.load::<u32>());

    Ok(())
}
