# Vehicle Rental System (C++)

A console-based vehicle rental management system written in C++. It lets an operator manage vehicles and users, book trips based on vehicle availability, record odometer readings, and calculate fares. Records are saved in text files so data remains available after the application closes.

## Features

- Add, view, and edit vehicles.
- Register users with a name, contact number, and optional email.
- Book trips for a selected user, date range, and vehicle type.
- Show only vehicles that are available for the requested dates.
- Start a trip by recording the starting odometer reading.
- Complete a trip and calculate its fare from kilometres travelled and the vehicle's price per kilometre.
- Store vehicles, users, and trips in local text files.
- Validate common input errors and handle application errors with exceptions.

## Technology

- **Language:** C++
- **Minimum standard:** C++11
- **Also tested with:** C++14, C++17, and C++20
- **Standard library features:** `vector`, `string`, file streams, string streams, templates, exceptions, and runtime casts
- **Suggested editor:** Visual Studio Code
- **Compiler:** `g++`

## Project structure

Keep all project source files in the same folder:

```text
VehicleRentalSystem/
├── application.cpp
├── application.h
├── database.cpp
├── database.h
├── database_templates.cpp
├── date.cpp
├── date.h
├── error.cpp
├── main.cpp
├── serializable.cpp
├── storable.cpp
├── string_helper.cpp
├── string_helper.h
├── table.cpp
├── table.h
├── trip.cpp
├── trip.h
├── user.cpp
├── user.h
├── vehicle.cpp
└── vehicle.h
```

The application creates these data files in the **current working directory** when needed:

- `vehicles.txt` — vehicle records
- `users.txt` — user records
- `trips.txt` — trip records

Each record is stored on one line, with fields separated by semicolons (`;`). Do not put semicolons inside text fields.

> Some implementation files (`serializable.cpp`, `storable.cpp`, `error.cpp`, `table.cpp`, and `database_templates.cpp`) are included by other source files in this project. Follow the build command below rather than compiling every `.cpp` file separately.


## How to use the application

The application is operated through a numbered console menu:

| Menu option | Action |
|---:|---|
| 1 | Add a vehicle |
| 2 | View a vehicle by registration number |
| 3 | Edit a vehicle's price per kilometre |
| 4 | Add a user |
| 5 | Book a trip |
| 6 | View a trip by ID |
| 7 | Start a trip |
| 8 | Complete a trip and calculate the fare |
| 9 | Exit |

When the program asks you to continue, press **Enter**.

### Typical workflow

1. Add a vehicle and provide its registration number, type, seating capacity, company, price per kilometre, and PUC expiry date.
2. Add a user with a name and contact number. Email is optional.
3. Book a trip using the user's contact number, start and end dates, and requested vehicle type.
4. Choose one of the available vehicles displayed by the application.
5. Start the trip by entering the starting odometer reading.
6. Complete the trip by entering the ending odometer reading. The application displays the fare.
7. Exit and restart the application to check that records persist.

### Vehicle types

The program defines these vehicle types:

| Value | Type |
|---:|---|
| 1 | Bike |
| 2 | Car |
| 3 | Towera |

The type names follow the project's code (`bike`, `car`, and `towera`).

## Fare calculation

The fare is calculated using:

```text
distance travelled = ending odometer reading - starting odometer reading
fare = distance travelled × vehicle price per kilometre
```

For example, if the starting reading is `1000`, the ending reading is `1100`, and the price is `10.5` per kilometre:

```text
distance travelled = 1100 - 1000 = 100 km
fare = 100 × 10.5 = 1050
```

The trip is marked as completed after completion. Completing an already completed trip returns its previously calculated fare.

## How vehicle availability works

When booking a trip, the application checks vehicles of the requested type against existing trips that have **not** been completed. A vehicle is treated as busy if an existing trip overlaps the requested date range. Only vehicles considered available are offered for selection.

## Architecture and OOP concepts

The project separates the user interface, data access, storage, and data models:

```text
main.cpp
  └── Application       Menus, input, and console display
        └── Database    Record lookup and coordination
              └── Table<T>  In-memory records and text-file persistence
                    └── Vehicle / User / Trip
                          └── Storable → Serializable
```

Key concepts demonstrated:

- **Classes and objects:** `Vehicle`, `User`, `Trip`, `Date`, and other classes model the system.
- **Inheritance:** `Vehicle`, `User`, and `Trip` derive from `Storable`.
- **Abstract classes and pure virtual functions:** `Serializable` defines `toString()`; `Storable` adds `setDataFrom()`.
- **Polymorphism:** records are stored through `Storable*` pointers, allowing different record types to share a common interface.
- **Templates:** `Table<T>` provides a reusable table implementation for different record types.
- **Operator overloading:** `Date` supports comparisons and adding days.
- **Exception handling:** custom errors such as `NoSuchRecordError`, `IOError`, and `MemoryError` report failures.
- **File handling:** records are loaded from and saved to text files.
- **Pointers and `dynamic_cast`:** the database identifies record types and connects trips to their vehicle and user records.
- **Encapsulation and `const` correctness:** data access is controlled through class interfaces and const-qualified methods.

## Data persistence format

The text files use one semicolon-separated record per line. A trip record follows this format:

```text
tripId;vehicleId;userId;startDate;endDate;startKm;endKm;fare;completed
```

Example:

```text
1;1;1;5/1/2030;7/1/2030;1000;1100;1050;1
```

The final field indicates whether the trip is completed (`1`) or not (`0`). Trips store vehicle and user IDs in the file; when data is loaded, the database uses those IDs to reconnect each trip to its vehicle and user objects.

## Important implementation details

- Dates use the `d/m/yyyy` format, for example `5/1/2030`.
- Missing data files are created automatically.
- Damaged data lines are skipped during loading.
- The string-splitting helper preserves an empty final field, allowing users to have no email address.
- Updates use a backup/restore approach if writing the updated table to disk fails.
- The application uses ANSI escape sequences for console screen positioning and clearing. A modern terminal, such as the VS Code integrated terminal, is recommended.
- The project guide reports successful test runs with `g++` under C++11, C++14, C++17, and C++20, plus AddressSanitizer/UBSan testing.


## Ideas for future improvements

- Add a menu to list all vehicles.
- Add trip deletion while designing a safer ID-generation strategy.
- Replace the semicolon-separated format with a more robust format such as JSON or CSV.
- Replace manually managed record pointers with smart pointers such as `std::unique_ptr`.
- Add automated tests for date parsing, trip overlap checks, and fare calculation.


