# Event Scheduler

The **Advanced Scheduler** is a comprehensive C++ program designed to manage venues, congregations, and events efficiently. It supports functionalities like adding and removing venues, scheduling events, and managing reservations with strict validation rules.

## Features

- **Venue Management**:
  - Add, delete, and display venues.
  - Reserve and free venues for specific congregations.
  - Show reserved venues for a given congregation.

- **Congregation Management**:
  - Add, delete, and display congregations.
  - Validate congregation dates to ensure they are in the future.

- **Event Scheduling**:
  - Add and delete events with detailed validation for time and venue availability.
  - Display events scheduled at a venue for a specific date.
  - Show a congregation's event calendar across a reserved venue.

- **Validation and Error Handling**:
  - Ensures valid dates, times, and logical overlaps for events and reservations.
  - Provides error messages for invalid inputs or conflicting schedules.

## Installation

1. Clone the repository or download the source file `adv_scheduler.cpp`.
2. Compile the program using a C++ compiler (e.g., g++):

   ```bash
   g++ adv_scheduler.cpp -o adv_scheduler

Input Format
------------

The program operates via standard input. Commands must follow the specified format. Each command should be entered as a single line.

### Example Commands

1.  **Add a Congregation**:

    plaintext

    Copy code

    `addCongregation "TechFest" "Conference" "2025-01-01" "2025-01-05"`

2.  **Delete a Congregation**:

    plaintext

    Copy code

    `deleteCongregation "TechFest"`

3.  **Add a Venue**:

    plaintext

    Copy code

    `addVenue "Grand Hall" "123 Main St:Cityville:StateX:12345:CountryY" 500`

4.  **Reserve a Venue**:

    plaintext

    Copy code

    `reserveVenue "Grand Hall" "CountryY" "TechFest"`

5.  **Add an Event**:

    plaintext

    Copy code

    `addEvent "TechFest" "Grand Hall" "CountryY" "2025-01-03" "10:00" "12:00" "Opening Ceremony"`

6.  **Show Events**:

    plaintext

    Copy code

    `showEvents "Grand Hall" "CountryY" "2025-01-03"`

7.  **End Program**:

    plaintext

    Copy code

    `End`

Output Format
-------------

-   `0`: Success.
-   `-1`: Error with a descriptive message.

Key Considerations
------------------

-   Date format: `YYYY-MM-DD`.
-   Time format: `HH:MM` (24-hour clock, multiples of 15 minutes).
-   Venue capacity must be a positive integer.
-   Events in the same venue must have a minimum 30-minute gap.

Dependencies
------------

-   Standard C++ libraries:
    -   `<iostream>`
    -   `<vector>`
    -   `<string>`
    -   `<sstream>`
    -   `<ctime>`

Future Enhancements
-------------------

-   Support for recurring events.
-   Advanced conflict resolution for overlapping schedules.
-   Integration with a database for persistent storage.
