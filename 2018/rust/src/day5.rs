use crate::util;

use std::time::{Duration, Instant};

pub fn get_input() {
    match util::get_input(5, "data/day5.txt") {
        Err(err) => println!("Error! {:#?}", err),
        Ok(_) => println!("got input"),
    }
}

fn reactive(a: &u8, b: &u8) -> bool {
    a != b && a.eq_ignore_ascii_case(&b)
}

pub fn no_reactives(right: &[u8]) -> usize {
    let mut left: Vec<u8> = Vec::new();

    for r in right {
        match left.last() {
            None => left.push(*r),
            Some(l) => {
                if reactive(l, &r) {
                    left.pop();
                } else {
                    left.push(*r)
                }
            }
        }
    }

    // println!("{:#?}", String::from_utf8(left.clone()).unwrap());

    left.len()
}

pub fn run(input: String) -> () {
    let now = Instant::now();
    let input: Vec<u8> = input.into();
    let result_1 = no_reactives(&input);
    println!("{:?}", Instant::now().duration_since(now));
    println!("1: {:#?}", result_1);

    let now = Instant::now();
    // let alphabet: Vec<u8> = (65..91).collect();
    let mut alphabet: [u8; 26] = Default::default();
    let x: Vec<u8> = (65..91).collect();
    alphabet.copy_from_slice(&x);

    let inputs: Vec<Vec<u8>> = alphabet
        .iter()
        .map(|c| input.clone().into_iter().filter(|ea| !ea.eq_ignore_ascii_case(c)).collect())
        .collect();

    // println!("{:#?}", input);

    let mut counts: [usize; 26] = Default::default();
    for i in 0..alphabet.len() {
        let c = alphabet[i];
        let input = &inputs[i];
        counts[i] = no_reactives(input);
    }
    println!("{:?}", Instant::now().duration_since(now));
    let result_2 = counts.iter().fold(std::usize::MAX, |last, ea| std::cmp::min(last, *ea));
    println!("{:#?}", result_2);



    // println!("{:#?}", inputs);

    // for char in alphabet.into_iter() {
    //     inputs[]
    // }

    // for x in alphabet.into_iter() {
    //     println!("{:#?}", *x as char);
    // }

    // println!("---------------");

    // for x in 65..91 {
    //     let x: u8 = x;
    //     println!("{:#?}", x as char);
    // }
}
