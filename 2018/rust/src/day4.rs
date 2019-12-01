extern crate chrono;
extern crate regex;
//use chrono::prelude::*;
use chrono::{NaiveDateTime, Timelike};
use regex::Regex;
use std::collections::HashMap;
use std::fs;

const MINS_IN_A_DAY: usize = 60 * 24;
type MinutesInDay = [u32; MINS_IN_A_DAY];

#[derive(Debug)]
struct Event {
    time: NaiveDateTime,
    event: String,
}

impl std::fmt::Display for Event {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "[{}] {}", self.time, self.event)
    }
}

struct Guard {
    id: String,

    minutes_asleep_each_day: MinutesInDay,
    recorded_sleep: Vec<Asleep>,
    actions: Vec<Action>,
}

impl Guard {
    fn add_action(&mut self, action: Action) {
        self.actions.push(action);
    }

    fn add_sleep(&mut self, from: NaiveDateTime, to: NaiveDateTime) {
        self.recorded_sleep.push(Asleep { from, to });
    }

    fn analyze_actions(&mut self) {
        let Guard {
            id,
            actions,
            recorded_sleep,
            ..
        } = self;
        for (i, action) in actions.iter().enumerate() {
            if let Action::FallsAsleep(time) = action {
                let next_action = &actions[i + 1];
                let asleep = Asleep {
                    from: action.time().clone(),
                    to: next_action.time().clone(),
                };
                recorded_sleep.push(asleep);
            }
        }
        self.update_recorded_sleep();
    }

    fn update_recorded_sleep(&mut self) {
        let mins = &mut self.minutes_asleep_each_day;
        self.recorded_sleep.iter().for_each(|asleep| {
            asleep.add_to_asleep_minutes(mins);
        });
    }

    fn with_id(id: &str) -> Guard {
        Guard {
            id: id.to_string(),
            minutes_asleep_each_day: [0; MINS_IN_A_DAY],
            recorded_sleep: Vec::new(),
            actions: Vec::new(),
        }
    }

    fn min_asleep_sum(&self) -> u32 {
        self.minutes_asleep_each_day[0..59].iter().sum()
    }

    fn in_which_min_asleep_most(&self) -> (u32, u32) {
        let mut max_min: u32 = 0;
        let mut max_asleep_time: u32 = 0;
        for (i, min_val) in self.minutes_asleep_each_day.iter().enumerate() {
            if *min_val > max_asleep_time {
                max_asleep_time = *min_val;
                max_min = i as u32;
            }
        }
        (max_min, max_asleep_time)
    }

    fn print_minutes_asleep(&self) {
        for h in 0..24 {
            print!("{:<3}", h);
            for min in 0..60 {
                print!("{}", self.minutes_asleep_each_day[h * 60 + min]);
            }
            print!("\n");
        }
    }

    fn print_actions(&self) {
        println!("Actions of guard {}", self.id);
        for a in self.actions.iter() {
            println!("{}", a);
        }
        println!("\n");
    }
}

#[derive(Debug)]
pub struct Asleep {
    pub from: chrono::NaiveDateTime,
    pub to: chrono::NaiveDateTime,
}
impl Asleep {
    fn add_to_asleep_minutes(&self, minutes_asleep: &mut MinutesInDay) {
        let Asleep { from, to } = self;
        let mins_remaining = to.signed_duration_since(from.clone()).num_minutes();
        let start_min = from.hour() * 60 + from.minute();
        for min in start_min..(start_min + mins_remaining as u32) {
            let min_in_day = (min as usize % MINS_IN_A_DAY) as usize;
            minutes_asleep[min_in_day] += 1;
        }
    }
}

impl std::fmt::Display for Asleep {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        let Asleep { from, to } = self;
        write!(
            f,
            "sleep {} {} ({} minutes)",
            from,
            to,
            to.signed_duration_since(from.clone()).num_minutes()
        )
    }
}

#[derive(Debug)]
enum Action {
    WakeUp(NaiveDateTime),
    FallsAsleep(NaiveDateTime),
    StartShift(String, NaiveDateTime),
    EndShift(NaiveDateTime),
}

impl Action {
    fn time(&self) -> &chrono::NaiveDateTime {
        match self {
            Action::WakeUp(time) => time,
            Action::FallsAsleep(time) => time,
            Action::StartShift(_, time) => time,
            Action::EndShift(time) => time,
        }
    }
}

impl std::fmt::Display for Action {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        let mut found_id = "";
        let (time, event) = match self {
            Action::WakeUp(time) => (time, "wakes"),
            Action::FallsAsleep(time) => (time, "sleeps"),
            Action::StartShift(id, time) => {
                found_id = id;
                (time, "starts")
            }
            Action::EndShift(time) => (time, "ends"),
        };
        write!(f, "[{}] {} ({})", time, event, found_id)
    }
}

pub fn run(input: String) {
    let event_re =
        Regex::new(r"\[(?P<date>\d{4}-\d{2}-\d{2} \d{2}:\d{2})\] (?P<event>.*)").unwrap();
    let begin_re = Regex::new(r"Guard #(?P<id>\d+) begins shift").unwrap();

    // 1. Parse individual events and create Actions for them.
    let mut actions: Vec<Action> = input
        .split_terminator("\n")
        .map(|line| {
            let caps: regex::Captures = event_re.captures(line).unwrap();
            let date_string = caps.name("date").unwrap().as_str();
            let event = caps.name("event").unwrap().as_str();
            let time = NaiveDateTime::parse_from_str(date_string, r"%Y-%m-%d %H:%M").unwrap();
            match event {
                "falls asleep" => Action::FallsAsleep(time),
                "wakes up" => Action::WakeUp(time),
                _ => {
                    let begin_shift_capture: regex::Captures = begin_re.captures(event).unwrap();
                    let id = begin_shift_capture.name("id").unwrap().as_str().to_string();
                    Action::StartShift(id, time)
                }
            }
        })
        .collect();

    // 2. Sort by event time, assumes none overlapping guard actions (each guard
    // works alone)
    //actions.sort_by(|a1, a2| a1.time().partial_cmp(&a2.time()).unwrap());
    actions.reverse();

    // 3. Chunk by guards and create end marker (not an event in the orig data)
    let mut guards = HashMap::<String, Guard>::new();
    let mut last_seen_id = String::new();
    //let mut guards = Vec::<Guard>::new();
    while let Some(action) = actions.pop() {
        if let Action::StartShift(id, time) = &action {
            if let Some(g) = guards.get_mut(&last_seen_id) {
                g.add_action(Action::EndShift(time.clone()));
            }

            last_seen_id.clear();
            last_seen_id.push_str(id);
            guards
                .entry(last_seen_id.clone())
                .or_insert(Guard::with_id(id));
        }
        if let Some(g) = guards.get_mut(&last_seen_id) {
            g.add_action(action);
        }
    }

    // 4. Compute asleep times for each guard
    guards.values_mut().for_each(|g| {
        g.analyze_actions();
        g.print_actions();
    });

    let sleepy_guard1 = guards
        .values()
        .max_by(|g1, g2| g1.min_asleep_sum().cmp(&g2.min_asleep_sum()))
        .unwrap();

    println!(
        "part 1: {} {}",
        sleepy_guard1.id,
        sleepy_guard1.in_which_min_asleep_most().0
    );

    let sleepy_guard2 = guards
        .values()
        .max_by(|g1, g2| {
            g1.in_which_min_asleep_most()
                .1
                .cmp(&g2.in_which_min_asleep_most().1)
        })
        .unwrap();

    println!(
        "part 2: {} {}",
        sleepy_guard2.id,
        sleepy_guard2.in_which_min_asleep_most().0
    );
}

#[cfg(test)]
mod tests {
    use crate::day4::*;
    use chrono::{NaiveDateTime, Timelike};
    use std::str::FromStr;
    use std::string::ToString;

    fn date(str: &str) -> NaiveDateTime {
        NaiveDateTime::parse_from_str(str, "%Y-%m-%d %H:%M:%S").unwrap()
    }

    #[test]
    fn record_sleep() {
        let mut g = Guard::with_id("foo");
        g.add_sleep(date("2018-10-10 01:20:00"), date("2018-10-11 02:25:00"));
        g.update_recorded_sleep();
        g.print_minutes_asleep();
    }

}
