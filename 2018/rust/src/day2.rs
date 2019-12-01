pub fn run(input: String) {
    let mut twos = 0;
    let mut threes = 0;
    for line in input.split_whitespace() {
        let mut map = std::collections::HashMap::<char, u32>::new();
        for ch in line.chars() {
            let counter = map.entry(ch).or_insert(0);
            *counter = *counter + 1;
        }
        let mut has_two = false;
        let mut has_three = false;
        for val in map.values() {
            if *val == 2 {
                has_two = true;
            }
            if *val == 3 {
                has_three = true;
            }
            if has_two && has_three {
                break;
            }
        }
        if has_two {
            twos = twos + 1;
        }
        if has_three {
            threes = threes + 1;
        }
    }
    println!("Result day 2: score: {}", twos * threes);
}
