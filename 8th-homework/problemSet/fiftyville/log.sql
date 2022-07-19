-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.
SELECT id, description
FROM crime_scene_reports
WHERE year="2021" AND month="7" AND day="28";
-- the id is 295
-- the description is Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- id: 295
SELECT activity, minute
FROM bakery_security_logs
WHERE year="2021"
    AND month="7"
    AND day="28"
    AND hour="10";
    -- AND minute="14";
-- +----------+--------+
-- | activity | minute |
-- +----------+--------+
-- | entrance | 8      |
-- | entrance | 14     |
-- | exit     | 16     |
-- | exit     | 18     |
-- | exit     | 18     |
-- | exit     | 19     |
-- | exit     | 20     |
-- | exit     | 21     |
-- | exit     | 23     |
-- | exit     | 23     |
-- | exit     | 35     |
-- | entrance | 42     |
-- | entrance | 44     |
-- | entrance | 55     |
-- +----------+--------+

SELECT transcript
FROM interviews
WHERE year="2021"
    AND month="7"
    AND day="28"
    AND transcript LIKE "%bakery%";
-- | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |

SELECT name, minute, activity
FROM people
    JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE year="2021"
    AND month="7"
    AND day="28"
    AND hour="10"
    AND minute>="10"
    AND minute<="20";
-- +---------+--------+----------+
-- |  name   | minute | activity |
-- +---------+--------+----------+
-- | Sophia  | 14     | entrance |
-- | Vanessa | 16     | exit     |
-- | Bruce   | 18     | exit     |
-- | Barry   | 18     | exit     |
-- | Luca    | 19     | exit     |
-- | Sofia   | 20     | exit     |
-- +---------+--------+----------+

SELECT name
FROM people
    JOIN bank_accounts ON people.id=bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number=atm_transactions.account_number
WHERE year="2021"
    AND month="7"
    AND day="28"
    AND transaction_type LIKE "withdraw"
    AND atm_location LIKE "Leggett Street";
    -- AND hour="10"
-- +---------+
-- |  name   |
-- +---------+
-- | Bruce   |*
-- | Diana   |
-- | Brooke  |
-- | Kenny   |
-- | Iman    |
-- | Luca    |*
-- | Taylor  |
-- | Benista |
-- +---------+

SELECT id, hour, minute
FROM flights
WHERE year="2021"
    AND month="7"
    AND day="29"
ORDER BY hour, minute;

-- +----+------+--------+
-- | id | hour | minute |
-- +----+------+--------+
-- | 36 | 8    | 20     |
-- | 43 | 9    | 30     |
-- | 23 | 12   | 15     |
-- | 53 | 15   | 20     |
-- | 18 | 16   | 0      |
-- +----+------+--------+

SELECT name
FROM people
    JOIN passengers ON people.passport_number=passengers.passport_number
WHERE flight_id = "36";
-- +--------+
-- |  name  |
-- +--------+
-- | Doris  |
-- | Sofia  |
-- | Bruce  |*
-- | Edward |
-- | Kelsey |
-- | Taylor |
-- | Kenny  |
-- | Luca   |*
-- +--------+

SELECT name, duration
FROM people
    JOIN phone_calls ON people.phone_number=phone_calls.caller
WHERE year="2021"
    AND month="7"
    AND day="28"
    -- AND day="29"
    -- this should be 28
    AND duration<"60"
ORDER BY duration;

-- +------------+----------+
-- |    name    | duration |
-- +------------+----------+
-- | Jacqueline | 32       |
-- | Ryan       | 35       |
-- | Jerry      | 45       |
-- | Christina  | 46       |
-- | Jonathan   | 47       |
-- | Deborah    | 48       |
-- | Brenda     | 57       |
-- +------------+----------+

SELECT name
FROM people
    JOIN passengers ON people.passport_number=passengers.passport_number
WHERE flight_id = "36"
    INTERSECT
SELECT name
FROM people
    JOIN bank_accounts ON people.id=bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number=atm_transactions.account_number
WHERE year="2021"
    AND month="7"
    AND day="28"
    AND transaction_type LIKE "withdraw"
    AND atm_location LIKE "Leggett Street"
    INTERSECT
SELECT name
FROM people
    JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE year="2021"
    AND month="7"
    AND day="28"
    AND hour="10"
    AND minute>="10"
    AND minute<="20"
    INTERSECT
SELECT name
FROM people
    JOIN phone_calls ON people.phone_number=phone_calls.caller
WHERE year="2021"
    AND month="7"
    AND day="28"
    -- AND day="29"
    -- this should be 28
    AND duration<"60";

-- +-------+
-- | name  |
-- +-------+
-- | Bruce |
-- +-------+

SELECT destination_airport_id
FROM flights
WHERE year="2021"
    AND month="7"
    AND day="29"
ORDER BY hour, minute
LIMIT 1;
-- +------------------------+
-- | destination_airport_id |
-- +------------------------+
-- | 4                      |
-- +------------------------+

SELECT city
FROM airports
WHERE id = 4;

-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+

SELECT name
FROM people
    JOIN phone_calls ON people.phone_number=phone_calls.receiver
WHERE year="2021"
    AND month="7"
    AND day="28"
    -- AND day="29"
    -- this should be 28
    AND duration<"60"
    AND caller=(
        SELECT phone_number
        FROM people
        WHERE name="Bruce"
        );
-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+