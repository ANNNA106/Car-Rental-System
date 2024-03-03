# Car Rental System

This is a car rental system that manages car rentals, customer data, and employee data.

## Files

- `cars.cpp`: This file contains the complete implementation of the car rental system.
- `car_data.csv`: This file contains the data of the available cars.
- `customer_data.csv`: This file contains the data of the customers.
- `employee_data.csv`: This file contains the data of the employees.

## Usage

To use this car rental system, follow these steps:

1. Compile and run the `cars.cpp` file.
2. Make sure the `car_data.csv`, `customer_data.csv`, and `employee_data.csv` files are in the same directory as the executable.
3. Follow the instructions provided by the program to manage car rentals, customer data, and employee data.
4. The Manager's password is `CS253`.
5. To save all the changes made in the data base, go to home page and select `Save`. After saving, you may press ctrl+C to exit the system.

## Working

1. When the user (employee/customer) rents a car, they are asked for how many days they wish to keep the car. While returning they are asked for how many days did they actually keep it. The fine is imposed based on the difference of these days (100*number of extra days). 
2. When the manager removes a car, any user (employee/customer) does not have that car rented with them anymore.
3. When the manager removes a user, all the cars rented by them go unrented.

## Assumption

1. The user's name and password must not contain spaces
2. 

