use std::{io::BufRead, ops::Neg};

use crate::input;
use anyhow::Result;

fn test_input() -> &'static str {
    "0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2"
}

struct Line {
    start: (i64, i64),
    end: (i64, i64),
}

impl std::fmt::Debug for Line {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "Line({}/{} -> {}/{})",
            self.start.0, self.start.1, self.end.0, self.end.1
        )
    }
}

impl Line {
    fn from_str(input: &str) -> Result<Self> {
        let (start, end) = input.split_once(" -> ").expect("split line with ->");
        let (start_x, start_y) = start.split_once(',').expect("split start");
        let (end_x, end_y) = end.split_once(',').expect("split end");

        let start = (start_x.parse()?, start_y.parse()?);
        let end = (end_x.parse()?, end_y.parse()?);

        Ok(Self { start, end })
    }

    fn min_max_x(&self) -> (i64, i64) {
        (self.start.0.min(self.end.0), self.start.0.max(self.end.0))
    }

    fn min_max_y(&self) -> (i64, i64) {
        (self.start.1.min(self.end.1), self.start.1.max(self.end.1))
    }

    fn is_horizontal(&self) -> bool {
        self.start.0 == self.end.0
    }

    fn is_vertical(&self) -> bool {
        self.start.1 == self.end.1
    }
}

struct Diagram {
    width: i64,
    height: i64,
    markers: Vec<i64>,
}

impl Diagram {
    fn new(width: i64, height: i64) -> Self {
        dbg!(width);
        dbg!(height);
        Self {
            width,
            height,
            markers: vec![0; (width * height) as usize],
        }
    }

    fn draw(&self) {
        for y in 0..self.height {
            for x in 0..self.width {
                let i = (y * self.width + x) as usize;
                print!("{} ", self.markers[i]);
            }
            println!();
        }
    }

    /// Bresenham line drawing:
    /// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    fn apply(&mut self, line: &Line) {
        log::debug!("apply line {:?}", line);

        let mut x0 = line.start.0;
        let mut y0 = line.start.1;
        let x1 = line.end.0;
        let y1 = line.end.1;

        let dx = (x1 - x0).abs();
        let sx = if x0 < x1 { 1 } else { -1 };
        let dy = -((y1 - y0).abs());
        let sy = if y0 < y1 { 1 } else { -1 };
        let mut err = dx + dy;
        loop {
            let i = (y0 * self.width + x0) as usize;
            log::debug!("{}/{} {}", x0, y0, i);
            self.markers[i] += 1;

            // plot(x0, y0);
            if x0 == x1 && y0 == y1 {
                break;
            }
            let e2 = 2 * err;
            if e2 >= dy {
                err += dy;
                x0 += sx;
            }
            if e2 <= dx {
                err += dx;
                y0 += sy;
            }
        }

        log::debug!("");
    }
}

pub fn solve(input: &str, only_straight: bool) -> Result<()> {
    let mut width = 0;
    let mut height = 0;

    let lines = input
        .lines()
        .map(|line| {
            let line = Line::from_str(line).unwrap();

            let (_, line_max_x) = line.min_max_x();
            let (_, line_max_y) = line.min_max_y();

            width = width.max(line_max_x + 1);
            height = height.max(line_max_y + 1);

            line
        })
        .collect::<Vec<_>>();

    let mut diagram = Diagram::new(width, height);

    for line in &lines {
        if !only_straight || (line.is_horizontal() || line.is_vertical()) {
            diagram.apply(line);
        }
    }

    if log::log_enabled!(log::Level::Debug) {
        diagram.draw();
    }

    let overlaps = diagram.markers.iter().copied().filter(|n| *n >= 2).count();
    println!("overlaps = {}", overlaps);

    Ok(())
}

pub fn part1(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(5)?
    };
    solve(&input, true)?;
    Ok(())
}

pub fn part2(testing: bool) -> Result<()> {
    let input = if testing {
        test_input().to_string()
    } else {
        input::load(5)?
    };
    solve(&input, false)?;

    Ok(())
}
