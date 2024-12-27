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
