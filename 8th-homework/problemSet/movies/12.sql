SELECT title
FROM movies
WHERE id IN(
        SELECT movie_id
        FROM stars
            JOIN people ON people.id = stars.person_id
        WHERE name LIKE "Johnny Depp"
    INTERSECT
        SELECT movie_id
        FROM stars
            JOIN people ON people.id = stars.person_id
        WHERE name LIKE "Helena Bonham Carter")
;