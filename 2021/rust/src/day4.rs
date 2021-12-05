use crate::input;
use anyhow::Result;

fn test_input() -> &'static str {
    "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7"
}

#[derive(Debug)]
struct RandomNumbers(Vec<u8>);

#[derive(Debug)]
struct BingoField {
    fields: [u8; 25],
    marked: [bool; 25],
}

impl BingoField {
    fn row_marked(&self) -> Option<usize> {
        for row in 0..5 {
            let base_index = row * 5;
            if (base_index..base_index + 5).all(|i| self.marked[i]) {
                return Some(row);
            }
        }
        None
    }

    fn col_marked(&self) -> Option<usize> {
        for col in 0..5 {
            if (0..5).all(|row| self.marked[row * 5 + col]) {
                return Some(col);
            }
        }
        None
    }

    fn apply_number(&mut self, n: u8) {
        for (i, m) in self.fields.iter().enumerate() {
            if n == *m {
                self.marked[i] = true;
            }
        }
    }

    fn unmarked_sum(&self) -> i64 {
        self.fields
            .iter()
            .copied()
            .zip(self.marked)
            .filter_map(|(n, marked)| if !marked { Some(n as i64) } else { None })
            .sum()
    }
}

fn parse_input(testing: bool) -> Result<(RandomNumbers, Vec<BingoField>)> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(4)?
    };
    let mut lines = input.lines();

    // let mut lines = test_input().lines();

    let numbers = lines
        .next()
        .expect("first line")
        .split(',')
        .map(|part| part.parse::<u8>().expect("parse number"))
        .collect::<Vec<_>>();

    let mut fields = Vec::new();

    while lines.next().is_some() {
        let rows = (0..5)
            .flat_map(|_| {
                let row = lines.next().expect("row");
                let numbers = row
                    .split(' ')
                    .filter_map(|ea| {
                        if ea.is_empty() {
                            None
                        } else {
                            ea.parse::<u8>().ok()
                        }
                    })
                    .collect::<Vec<_>>();
                numbers
            })
            .collect::<Vec<_>>();

        fields.push(BingoField {
            fields: rows.try_into().unwrap(),
            marked: Default::default(),
        });
    }

    Ok((RandomNumbers(numbers), fields))
}

pub fn part1(testing: bool) -> Result<()> {
    let (numbers, mut fields) = parse_input(testing)?;

    let mut winning_board = None;
    let mut winning_number = None;

    'outer: for n in numbers.0 {
        println!("playing number n");
        for (i, field) in fields.iter_mut().enumerate() {
            field.apply_number(n);

            if let Some(row) = field.row_marked() {
                println!("field {} has full row {}", i, row);
                winning_number = Some(n);
                winning_board = Some(i);
                break 'outer;
            }
            if let Some(col) = field.col_marked() {
                println!("field {} has full col {}", i, col);
                winning_number = Some(n);
                winning_board = Some(i);
                break 'outer;
            }
        }
    }

    if let (Some(field), Some(n)) = (winning_board, winning_number) {
        let sum = fields[field].unmarked_sum();
        println!("result {}", sum * n as i64);
    }

    Ok(())
}

pub fn part2(testing: bool) -> Result<()> {
    let (numbers, mut fields) = parse_input(testing)?;

    let mut last_winning_board = None;
    let mut last_number = None;

    for n in numbers.0 {
        for field in fields.iter_mut() {
            field.apply_number(n);
        }

        let (mut fields_winning, fields_left): (Vec<_>, Vec<_>) =
            fields.into_iter().partition(|field| {
                if field.row_marked().is_some() {
                    last_number = Some(n);
                    return true;
                }

                if field.col_marked().is_some() {
                    last_number = Some(n);
                    return true;
                }
                false
            });

        if fields_left.is_empty() && !fields_winning.is_empty() {
            last_winning_board = Some(fields_winning.remove(0));
        }
        fields = fields_left
    }

    if let (Some(field), Some(n)) = (last_winning_board, last_number) {
        let sum = field.unmarked_sum();
        println!("result {}", sum * n as i64);
    }

    Ok(())
}
