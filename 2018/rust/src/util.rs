use cookie::Cookie;
use reqwest::{self, header, ClientBuilder, Url};
use std::str::FromStr;

use std::fmt::Display;
use std::fs::File;
use std::io::Write;
use std::path::Path;


const SESSION: &'static str = env!("ADVENT_OF_CODE_SESSION");

pub fn get_input<P: Display + AsRef<Path>>(day: u8, path: P) -> Result<(), Box<dyn std::error::Error>> {
    let cookie = Cookie::build("session", SESSION)
    .domain("adventofcode.com")
    .path("/")
    .secure(false)
    .finish();

    let url_string = format!("https://adventofcode.com/2018/day/{}/input", day);
    let url: Url = url_string
        .parse()
        .map_err(|err| format!("could not parse url: {}", err))?;
    let mut req = reqwest::Request::new(reqwest::Method::GET, url);

    let headers = req.headers_mut();
    let cookie_str = cookie.encoded().to_string();
    println!("{:#?}", cookie_str);

    let header_val = header::HeaderValue::from_str(&cookie_str)
        .map_err(|err| format!("error setting cookie value: {}", err))?;
    let header_key = header::HeaderName::from_str("COOKIE")
        .map_err(|err| format!("error setting cookie valunam: {}", err))?;
    headers.insert(header_key, header_val);

    let client = reqwest::Client::new();
    let mut response = client
        .execute(req)
        .map_err(|err| format!("error sending request: {}", err))?;
    let content: String = response
        .text()
        .map_err(|err| format!("error reading request: {}", err))?;

    let bytes = content.trim().as_bytes();
    println!("writing content of size {} bytes to {}", bytes.len(), path);

    let mut f: File = File::create(path).map_err(|err| format!("error opening file: {}", err))?;
    f.write_all(bytes)
        .map_err(|err| format!("error writing file: {}", err))?;

    Ok(())
}
