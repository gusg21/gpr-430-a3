# Constructing Strings Efficiently

Prefer:
```c++
std::sstream build;
while (getline(input, line)) {
  build << get_int_or_float_as_string(line);
}

return build.str();
```

to:
```c++
std::string build;
while (getline(input, line)) {
  build += get_int_or_float_as_string(line);
}

return build;
```

## Why?
`std::string` has to keep its underlying memory contiguous, like `std::vector` does.
Concatenations are often expensive because they can involve a reallocation when
concatenating past the end of a buffer.

`std::stringstream` doesn't have this requirement -- it can store a list of strings,
and then do one big allocation + copy when you call `.str()`.

Note this is true in a lot of programming languages -- for this reason, most
standard libraries come with a `stringstream` equivalent, like Java's and C#'s
respective `StringBuilder` classes.
