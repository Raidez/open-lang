# Open (Open source, Programming language, Elegant syntax, Never-ending)

This programming language don't exist yet.

## Preface

I started this project to create my own programming language with 3 main goals in mind:
- low level (memory management, pointers, etc.) because it's fun
- good performance (compiled language), who knows, maybe it will be used for game development or something else
- elegant syntax (easy to read and write), goodbye to C++ and it's weird syntax

## Open source

## Programming language

## Elegant syntax

```
// This is a comment

/* This is a
multi-line comment */

/// This is a documentation comment
/// @param value The value to add
/// @return The result of the addition
func Foo(value: int) -> int {
    return 42 + value;
}

/**** Entry point of the program ****/

/* The entry point style is defined in the project settings (toml file).
You can choose between C-style (argc, argv) or Swift-style (args). */

func Main() -> int {
    Print("Hello, World!");
    return 0;
}

// Same as above but with parameters in C-style
func Main(argc: int, argv: *char[]) -> int {
    Print("Hello, World!");
    return 0;
}

// Same as above but with parameters in Swift-style
func Main(args: <string>) -> int {
    Print("Hello, World!");
    return 0;
}

/**** Variables ****/

// Declare a variable with a type
let a: int = 5;

// But you can also declare a variable without a type, the compiler will infer the type
let b = 10;

// And finally you can declare multiple variables at once
let c, d, e = 20, 30, 50;

// Convention: use camelCase for variable names

/**** Mutability ****/

// A variable declared with `let` is immutable, you can't change its value
let f = 100;
f = 200; // This will cause a compile error

// To declare a mutable variable, you have to use `var`
var g = 300;
g = 400; // This is fine

// Because it's mutable, you can declare a variable without initializing it, but you have to specify its type
var h: int;
h = 500;

// Prefer to use `let` whenever possible, it makes your code safer and easier to reason about

/**** Constants ****/

// Constants are declared outside of any function and are immutable, they must be initialized at declaration
const PI: float = 3.14159;

// Convention: use UPPER_CASE for variable names

/**** Data types ****/

// The language has the following primitive data types:

// int: signed integer type (32 bits by default in project settings)
let i = 40; // Positive integer
let j = -42; // Negative integer
let k: int8 = 127; // You can also specify the size of the integer (int8, int16, int32, int64)

// uint: unsigned integer type (32 bits by default in project settings)
let l = 0; // Zero is allowed
let m = -42; // Compile error, unsigned integers can't be negative
let n: uint8 = 255; // You can also specify the size of the unsigned integer (uint8, uint16, uint32, uint64)

// float: floating point type (64 bits by default in project settings)
let o = 3.14; // Positive float
let p = -3.14; // Negative float
let q: float32 = 3.14; // You can also specify the size of the float (float8, float16, float32, float64)

// bool: boolean type (fixed 8 bits)
let r = true;
let s: bool = false; // You can also specify the type explicitly

// char: character type (32 bits by default in project settings)
let t = 'a'; // Use single quotes for characters
let u: char = '😃'; // Also accept emoji (utf-8 use 32 bits)

// string: string type (32 bits by default in project settings)
let v: string = "Hello, World!"; // Use double quotes for strings

/**** Arrays ****/

// You can declare an array with a fixed size
let w: int[3] = [1, 2, 3];
let x: bool[] = [true, false, true]; // You can also declare an array without size, the compiler will infer the size
var y: string[2] = {"Hello"}; // You can also initialize all elements with the same value in braces

// You can't declare an array without specifying its type
let z = [1, 2, 3]; // Compile error, the compiler can't infer the type of the array

// You can also declare an array without initializing it, but you have to specify its type and size
let aa: int[3]; // So it's hold 3 integers, but they are uninitialized (so they have a random value)

// Array indexes start at 0 and go up to size - 1
Print(w[0]); // Access the first element of the array
Print(w[1]); // Access the second element of the array
Print(w[10]); // Compile error, you can't access an element outside of the array bounds (same for negative indexes)

// You can change the value of an element of an array declared with `var`
y[1] = "World"; // Change the value of the first element of the array
w[0] = 10; // Compile error, you can't change the value of an element of an array declared with `let`

/**** Vectors ****/

// Vectors are similar to arrays but they can grow and shrink dynamically (so declare with 'var' instead of 'let')
var ab: <int> = [1, 2, 3]; // So you don't specify size, you just use angle brackets to specify the type of the elements

ab.Push(4); // Add an element to the end of the vector
ab.Pop(); // Remove the last element of the vector

ab.Push(true); // Compile error, you can't add a boolean to a vector of integers

// Immutable vectors are declared with `let` (and not really useful because you can't add or remove elements from them)
let ac: <int> = [1, 2, 3];
ac.Push(4); // Compile error, you can't add an element to a vector declared with `let`

/**** Pointers ****/

// Pointers are variables that store the memory address of another variable, they are declared with an asterisk before the type
// To get the memory address of a variable, you have to use the ampersand operator before the variable name
let ad: *int = &a; // 'ad' store the memory address of 'a' (an hexadecimal number)

// Pointers are not made to be use directly
Print(ad); // Print the memory address of 'a' -> 0x7ffee3b8c8a4 or something like that

// To get the value of a variable pointed by a pointer, you have to use the dereference operator before the pointer name
Print(*ad);

// Whatever the pointer is mutable or not, you can change the value of the variable pointed if it is mutable
var ae = 42;
let af: *int = &ae;
*af = 100; // Change the value of 'ae' to 100

let ag = 42;
let ah: *int = &ag;
*ah = 100; // Compile error, you can't change the value of 'ag'

// Mutable pointers is used when the pointer itself can changer the variable pointed
let ai = 42;
var aj: *int = &ai; // 'aj' is a mutable pointer to 'ai'
let ak = 100;
aj = &ak; // Change the pointer to point to 'ak' instead of 'ai'
Print(*aj); // Print the value of 'ak' -> 100

// Some mindfuck, a pointer can point to another pointer, and so on, and so on...
let al = 42;
let am: *int = &al; // 'am' point to 'al'
let an: **int = &am; // 'an' point to 'am'
Print(**an); // Print the value of 'al' -> 42, using double dereference operator

// Pointers are weird creatures, they can be used to store unknown memory addresses types with '*unknown' type
// But you can't dereference them because the compiler doesn't know the type of the variable pointed
let ao: *unknown = &al; // 'ao' point to 'al' but the compiler doesn't know the type of 'al'

Print(*ao); // Compile error, you can't dereference a pointer to unknown type

// So you must cast the pointer to the correct type before dereferencing it
Print(*(ao as *int));

// And finally, a pointer can point to nothing
let ap: *int = null; // 'ap' point to nothing
Print(*ap); // Compile error, you can't dereference a pointer to null

/**** Casting ****/

// You can cast a value to another type with the 'as' keyword
42 as float; // Cast an integer to a float
3.14 as int; // Cast a float to an integer (the decimal part is truncated)
true as int; // Cast a boolean to an integer (true = 1, false = 0)
'a' as int; // Cast a character to an integer (the ASCII value of the character)
74 as bool; // Cast an integer to a boolean (0 = false, any other value = true)

// Be careful, the final type depends on the value
let aq = 300 as int8; // Compile error, 300 is too big for an int8 (max value is 127)
let ar: float = 42 as float; // This is fine, 42 can be represented as a float
let as: int = 3.14; // Compile error, you can't assign a float to an integer without casting it first (also 'as' is a reserved keyword)

/**** Notation ****/

// You can use different notations to represent numbers in the source code
let at = 0xFF; // Hexadecimal notation
let au = 0b101010; // Binary notation
let av = 0o77; // Octal notation
let aw = 1_000_000; // You can use underscores to separate digits for readability
let ax = 1.5e3; // Scientific notation (1.5 * 10^3 = 1500)

/**** Operations ****/

// The language has the following operators:
2 + 2; // Addition
2 - 2; // Subtraction
2 * 2; // Multiplication
2 / 2; // Division (returns an integer)
2.0 / 2.0; // Division (returns a float)
2 / 2.0; // Division (returns a float)
2.0 / 2; // Division (returns a float)
2 % 2; // Modulo (remainder of the division)
2 ** 2; // Exponentiation (2 to the power of 2)

// The language has the following assignment operators:
var ay = 2; // Assignment
ay += 2; // Addition assignment
ay -= 2; // Subtraction assignment
ay *= 2; // Multiplication assignment
ay /= 2; // Division assignment

// The language has the following bitwise operators:
2 & 2; // Bitwise AND
2 | 2; // Bitwise OR
2 << 2; // Bitwise left shift
2 >> 2; // Bitwise right shift
2 ^ 2; // Bitwise XOR
~2; // Bitwise NOT

// The language has the following comparison operators:
2 == 2; // Equal to
2 != 2; // Not equal to
2 > 2; // Greater than
2 < 2; // Less than
2 >= 2; // Greater than or equal to
2 <= 2; // Less than or equal to

// The language has the following logical operators:
true && false; // Logical AND
true || false; // Logical OR
true ^^ false; // Logical XOR
!false; // Logical NOT

/**** Control flow (conditionals) ****/

// You can branch your code with if statements, else if statements and else statements
if true {
    Print("Hello, World!");
} else {
    Print("Goodbye, World!");
}

if a > b {
    Print("a is greater than b");
} else if a < b {
    Print("a is less than b");
} else {
    Print("a is equal to b");
}

// You can also use a match pattern to branch your code based on the value of a variable
match a {
    1 => Print("a is 1"),
    2 => Print("a is 2"),
    _ => Print("a is something else"), // The underscore is a wildcard that matches anything
}

// You can also use a range in a match pattern
match a {
    1..10 => Print("a is between 1 and 10 (exclusive)"),
    10..=20 => Print("a is between 10 and 20 (inclusive)"),
    42 => Print("a is 42"),
    _ => Print("a is something else"),
}

// You can also use a match pattern to assign a value to a variable based on the value of another variable
let az = match a {
    1 => "a is 1",
    2 => "a is 2",
    _ => "a is something else", // The underscore is a wildcard that matches anything
};

// You can also use a ternary operator to assign a value to a variable based on a condition (parentheses are optional)
let ba = (a > b) ? "a is greater than b" : "a is less than or equal to b";

/**** Control flow (loops) ****/

// Infinite loop
loop {
    Print("Hello, World!");
    break; // You can use the break statement to exit the loop
}

// You can use 'continue' statement to skip the rest of the loop and go to the next iteration
// And 'break' statement to exit the loop
loop {
    if a > b {
        continue;
    } else {
        break;
    }
}

// While loop, using a condition
var bb = 0;
while bb < 10 {
    bb += 1; // Increment ba by 1
    Print(bb);
}

// Do while loop, don't exist

// For loop, using a range (0 to 9, 10 excluded)
for i in 0..10 {
    Print(i); // Print the value of i from 0 to 9
}

// Same as above but including the last value (10 included)
for i in 0..=10 {
    Print(i); // Print the value of i from 0 to 10
}

// For each loop, using an array
let bc: int[3] = [1, 2, 3];
for i in bc {
    Print(i); // Print the value of each element of the array
}

// Also works with vectors
var bd: <int> = [1, 2, 3];
for i in bd {
    Print(i); // Print the value of each element of the vector
}

// Also works with string
let be: string = "Hello, World!";
for i in be {
    Print(i); // Print the value of each character of the string
}

// For each loop don't works on tuples, structs and enums because they are not iterable

/**** Functions ****/

// Functions are declared with the `func` keyword, followed by the function name, parameters (if any) and return type (if any)

// Function without parameters and without return type
func BarA() {
    Print("Hello, World!");
}

// Function with return type
func BarB() -> int {
    return 42;
}

// Function with parameters and return type
func BarC(value: int) -> int {
    return 42 + value;
}

// You can also specify default values for parameters, so you can call the function without passing any arguments
func BarE(value: int = 42) -> int {
    return 42 + value;
}

// Convention: use PascalCase for function names (same as structs, enums)

/**** Tuples ****/

// Tuples are a fixed-size collection of different types
let bf: (int, float, string) = (42, 3.14, "Hello, World!");

// Tuples can be mutable also
var bk: (int, float, string) = (42, 3.14, "Hello, World!");

// Like always, you can declare without type
let bg = (42, 3.14, "Hello, World!");

// To access the elements of a tuple, use the dot notation with the index
Print(bf.0); // Access the first element of the tuple
Print(bf.1); // Access the second element of the tuple
Print(bf.5); // Compile error, you can't access an element outside of the tuple bounds

// It's very useful to define function that return multiple values
func BarD() -> (int, float, string) {
    return (42, 3.14, "Hello, World!");
}

// You can destructure a tuple into multiple variables
let (bh, bi, bj) = BarD();

/**** Structures ****/

struct Dog {
    name: string;
    age: int;
}

let dog: Dog = Dog { name: "Buddy", age: 3 };

/**** Extends ****/

extend Dog {
    func New(name: string, age: int) -> Dog {
        return Dog { name: name, age: age };
    }

    func Hello(self) {
        Print("Hello, my name is {} and I am {} years old.", self.name, self.age);
    }
}

let dog2: Dog = Dog:New("Max", 5); // Static method to create a new Dog instance
dog2.Hello(); // Call the Hello method on the Dog instance

/**** Interfaces ****/

interface Animal {
    func Speak(self) -> string;
}

struct Cat {
    name: string;
    age: int;
}

extend Cat implements Animal {
    func Speak(self) -> string {
        return "Meow!";
    }
}

/**** Enums ****/

enum Example {
    A,
    B,
    C,
}

bitflag FileRights {
    NONE, // => 0
    READ, // => 1
    WRITE, // => 2
    EXECUTE, // => 4
}

let rights = FileRights.READ | FileRights.WRITE; // => 3

enum Color {
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
}

/**** Type aliases ****/

type MyInt = int;

type SpeakType = func() -> string;

struct Parrot {
    name: string;
    age: int;
    speak: SpeakType;
}

extend Parrot {
    func Speak(self) -> string {
        return self.speak();
    }
}

/**** Operator overload ****/

struct Vector2 {
    x: float;
    y: float;
}

extend Vector2 {

    func +(self, other: Vector2) -> Vector2 {
        return Vector2 { x: self.x + other.x, y: self.y + other.y };
    }

    func -(self, other: Vector2) -> Vector2 {
        return Vector2 { x: self.x - other.x, y: self.y - other.y };
    }

    func *(self, scalar: float) -> Vector2 {
        return Vector2 { x: self.x * scalar, y: self.y * scalar };
    }

    func *(self, other: Vector2) -> float {
        return self.x * other.x + self.y * other.y; // Dot product
    }

    func /(self, scalar: float) -> Vector2 {
        return Vector2 { x: self.x / scalar, y: self.y / scalar };
    }
}

/**** Extern functions ****/

// You can declare an extern function to use a call from a C library
// Libraries are linked in the project settings (toml file)

// string -> *char

// Extern declaration function
extern func fopen(path: string, mode: string) -> *unknown;

// Extern declaration block
extern {
    func printf(format: string, ...) -> int;
}
```

## Never-ending
