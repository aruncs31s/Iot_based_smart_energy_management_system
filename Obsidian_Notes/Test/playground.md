
```bash

echo " Hello World!"
read a
echo $a
```


```rust
use std::io::{self, Write};
fn main() {
    print!("Please enter something: ");
    io::stdout().flush().unwrap(); // Ensure the prompt is displayed
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    println!("You entered: {}", input.trim());
```

```rust
fn main(){
   let mut line = String::new();
   println!("Enter your name :");
   let b1 = std::io::stdin().read_line(&mut line).unwrap();
   println!("Hello , {}", line);
   println!("no of bytes read , {}", b1);
}
```