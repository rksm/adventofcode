extern crate regex;
use regex::Regex;
use std::collections::HashMap;
use std::fs;

#[derive(Debug)]
struct Claim {
    id: String,
    x: usize,
    y: usize,
    w: usize,
    h: usize,
}

type ClaimID = String;

fn claim_parser() -> impl Fn(&str) -> Claim {
    let re =
        Regex::new(r"^#(?P<id>[0-9]+) @ (?P<x>[0-9]+),(?P<y>[0-9]+): (?P<w>[0-9]+)x(?P<h>[0-9]+)$")
            .unwrap();
    move |line| {
        let cap = re.captures(line).unwrap();
        let id = cap["id"].to_owned();
        let x = cap["x"].parse::<usize>().unwrap();
        let y = cap["y"].parse::<usize>().unwrap();
        let w = cap["w"].parse::<usize>().unwrap();
        let h = cap["h"].parse::<usize>().unwrap();
        Claim { id, x, y, w, h }
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct Canvas(Vec<Vec<Vec<ClaimID>>>);

impl std::fmt::Debug for Canvas {
    fn fmt(&self, fmt: &mut std::fmt::Formatter) -> std::result::Result<(), std::fmt::Error> {
        // let Canvas(rows) = self;
        // for row in rows {
        //     for col in row {
        //         let n = col.iter().count();
        //         fmt.write_fmt(format_args!("{}", n))?;
        //     }
        //     fmt.write_str("\n")?;
        // }
        // return Ok(());
        for CanvasIteratorItem {
            x,
            y,
            last_in_row,
            claims,
        } in self.into_iter()
        {
            let printed = match claims.last() {
                None => "-",
                Some(id) => id,
            };
            fmt.write_fmt(format_args!("{:4}", printed))?;
            //fmt.write_fmt(format_args!("{}", n))?;
            if last_in_row {
                fmt.write_str("\n")?;
            }
        }
        return Ok(());
    }
}

impl<'a> IntoIterator for &'a Canvas {
    //type IntoIter = CanvasIterator<'a>;
    type Item = CanvasIteratorItem<'a>;
    type IntoIter = CanvasIterator<'a>;
    fn into_iter(self) -> CanvasIterator<'a> {
        CanvasIterator {
            x: 0,
            y: 0,
            canvas: self,
        }
    }
}

impl Canvas {
    fn new(w: usize, h: usize) -> Canvas {
        Canvas((0..h).into_iter().map(|_| vec![Vec::new(); w]).collect())
    }

    fn set(&mut self, x: usize, y: usize, claim_id: &ClaimID) {
        let Canvas(rows) = self;
        // rows[y][x].push(claim_id.clone());
        rows[y][x].push(claim_id.clone());
    }

    fn set_row(&mut self, y: usize, from: usize, to: usize, claim_id: &ClaimID) {
        for x in from..to {
            self.set(x, y, claim_id)
        }
    }

    fn set_rect(
        &mut self,
        from_y: usize,
        to_y: usize,
        from_x: usize,
        to_x: usize,
        claim_id: &ClaimID,
    ) {
        for y in from_y..to_y {
            self.set_row(y, from_x, to_x, claim_id);
        }
    }

    fn set_claim(&mut self, claim: &Claim) {
        let Claim { id, x, y, w, h } = claim;
        self.set_rect(*y, y + h, *x, x + w, id);
    }

    fn covered_double(&self) -> u32 {
        let mut result = 0;
        let Canvas(rows) = self;
        for row in rows {
            for col in row {
                let n = col.iter().count();
                if n > 1 {
                    result = result + 1;
                }
            }
        }
        result
    }
}

struct CanvasIterator<'a> {
    y: usize,
    x: usize,
    canvas: &'a Canvas,
}

struct CanvasIteratorItem<'a> {
    claims: &'a Vec<ClaimID>,
    x: usize,
    y: usize,
    last_in_row: bool,
}

impl<'a> Iterator for CanvasIterator<'a> {
    type Item = CanvasIteratorItem<'a>;

    fn next(&mut self) -> Option<CanvasIteratorItem<'a>> {
        let CanvasIterator {
            x,
            y,
            canvas: Canvas(rows),
        } = self;
        if *y >= rows.len() {
            return None;
        }
        let row = &rows[*y];
        if *x >= row.len() {
            if *y == rows.len() - 1 {
                return None;
            }
            *y = *y + 1;
            *x = 0;
        }
        let last_in_row = *x == (row.len() - 1);
        let claims = &row[*x];
        *x = *x + 1;
        return Some(CanvasIteratorItem {
            x: *x,
            y: *y,
            last_in_row,
            claims,
        });
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

pub fn run(input: String) {
    let parser = claim_parser();
    let mut canvas = Canvas::new(1000, 1000);
    for (i, line) in input.split_terminator("\n").enumerate() {
        let claim = parser(line);
        canvas.set_claim(&claim);
    }
    //println!("{:#?}", canvas);

    println!("Result day 3: {}", 1000 * 1000);
    // how many inches covered by more than one patch?
    println!("Result day 3: {}", canvas.covered_double());

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // what is are the only patches that don't ovelap? (there is only one)
    let mut max_overlap_by_claim_id: HashMap<&ClaimID, u32> = HashMap::new();

    for CanvasIteratorItem {
        x,
        y,
        last_in_row,
        claims,
    } in canvas.into_iter()
    {
        let n = claims.len();
        for claim_id in claims {
            max_overlap_by_claim_id
                .entry(claim_id)
                .and_modify(|count| *count = std::cmp::max(*count, n as u32))
                .or_insert(n as u32);
        }
    }

    let safe: Vec<&String> = max_overlap_by_claim_id
        .iter()
        .filter(|(id, n)| **n == 1)
        .map(|(id, n)| *id)
        .collect();
    println!("{:#?}", safe);
}

#[test]
fn it_works() {
    let input1 = "#1 @ 3,11: 5x10";
    let claim1 = claim_parser()(input1);
    let input2 = "#2 @ 3,5: 2x10";
    let claim2 = claim_parser()(input2);
    let mut canvas = Canvas::new(30, 30);

    canvas.set_claim(&claim1);
    canvas.set_claim(&claim2);

    for CanvasIteratorItem {
        x,
        y,
        last_in_row,
        claims,
    } in canvas.into_iter()
    {
        print!(
            "{} ",
            match claims.last() {
                Some(id) => id,
                None => "-",
            }
        );
        if last_in_row {
            println!();
        }
    }

    println!("{:#?}", canvas);
    println!("{:#?}", canvas.covered_double());
    assert_eq!(canvas.covered_double(), 8);
}
