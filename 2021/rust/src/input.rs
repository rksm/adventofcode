use anyhow::Result;
use std::path::PathBuf;

fn fetch_input(day: u16) -> Result<String> {
    let cookie = std::fs::read_to_string("data/cookie.txt")?;
    let url = format!("https://adventofcode.com/2021/day/{}/input", day);
    let content = ureq::get(&url)
        .set("Cookie", cookie.trim())
        .call()?
        .into_string()?;
    let file = PathBuf::from(format!("data/day{}.txt", day));
    std::fs::create_dir_all(file.parent().unwrap())?;
    std::fs::write(file, &content)?;
    Ok(content)
}

pub fn load(day: u16) -> Result<String> {
    let file = PathBuf::from(format!("data/day{}.txt", day));
    if file.exists() {
        Ok(std::fs::read_to_string(file)?)
    } else {
        fetch_input(day)
    }
}
