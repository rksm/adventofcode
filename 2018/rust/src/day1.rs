pub fn run(input: String) {
    let mut sum = 0;
    for (i, line) in input.split_whitespace().enumerate() {
        let num = line
            .parse::<i32>()
            .expect(&format!("cannot read line {}", i));
        sum += num
    }
    println!("Result day 1: {}", sum);
}
