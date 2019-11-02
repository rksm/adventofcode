#![feature(toowned_clone_into)]
#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(unused_imports)]

mod day1;
mod day2;
mod day3;
mod day4;
mod day5;
mod day9;
mod util;

fn input(day: u32) -> String {
    let f = format!("data/day{}.txt", day);
    std::fs::read_to_string(f).expect("input not there?")
}

fn main() {
    let args: Vec<String> = std::env::args().collect();

    match args[1].as_ref() {
        "1" => day1::run(input(1)),
        "2" => day2::run(input(2)),
        "3" => day3::run(input(3)),
        "4" => day4::run(input(4)),
        "5" => day5::run(input(5)),
        "9" => day9::run(input(9)),
        _ => println!("No input"),
    }
}
